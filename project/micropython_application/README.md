# MicroPython Application — Edge AI Inventory System

This is the on-device MicroPython firmware that runs on the **Infineon PSoC6 AI Evaluation Kit**. It acquires radar data from the BGT60TR13C sensor, classifies it using a DeepCraft neural network, and reports results to a remote inventory server over Wi-Fi.

---

## File Structure

```
micropython_application/
├── boot.py                 # Runs on power-up: Wi-Fi, imports, radar init
├── main.py                 # Detection loop: inference + server reporting
├── config.py               # All device/network configuration (auto-generated)
├── wifi.py                 # Wi-Fi connection manager
├── api_client.py           # HTTP client for the inventory REST API
├── radar_lib.py            # Radar controller + signal processing utilities
│
├── lib/                    # Low-level radar & DSP libraries
│   ├── BGT60TRXX.py        # BGT60TR13C sensor driver (SPI, FIFO, chirp config)
│   ├── BGT60TRXX_define.py # Register map and constants for the BGT60 sensor
│   ├── dft.py              # Optimised DFT implementation (@viper/@native)
│   ├── dftclass.py         # High-level FFT class wrapping dft.py
│   ├── polar.py            # Polar coordinate conversion utilities
│   └── window.py           # Windowing functions (Hann, Blackman-Harris, etc.)
│
├── BG60/                   # Reference/backup copies of radar driver files
│
└── Models/                 # Trained DeepCraft model variants
    ├── 13_02_small/        # Early small model (13 Feb iteration)
    │   └── deepcraft_model.mpy
    ├── Medium_final/       # Medium-sized final model
    │   └── deepcraft_model.mpy
    └── Model_final_small/  # ★ Final small model (currently deployed)
        └── deepcraft_model.mpy
```

---

## Boot Sequence

MicroPython executes files in a fixed order: `boot.py` → `main.py`.

### `boot.py` — Hardware Initialisation

Runs **once** on every power-up or hard reset. Responsible for:

1. **Garbage collection** — `gc.collect()` to maximise available RAM
2. **Import heavy modules early** — `deepcraft_model`, `wifi`, `radar_lib` (loaded once to avoid repeated allocation)
3. **Wi-Fi connection** — calls `wifi.connect()`, catches timeouts gracefully
4. **Radar sensor init** — creates a global `RadarController` instance and calls `setup()`:
   - Configures the SPI bus (50 MHz, MSB-first)
   - Sets chirp parameters (start frequency, bandwidth, ADC divider, 128 samples/chirp)
   - Resets and initialises the BGT60TR13C sensor

```python
radar = radar_lib.RadarController()
radar.setup()
```

> **Important:** The `radar` object lives in the `__main__` module scope. Later code accesses it via `getattr(__main__, 'radar')`.

### `main.py` — Detection Loop

Runs immediately after `boot.py`. Contains:

1. **Model initialisation** — loads the DeepCraft `.mpy` model, queries input/output dimensions, allocates `array.array('f')` buffers
2. **Detection loop** — repeats every `DETECTION_INTERVAL` seconds:
   - Ensures Wi-Fi is connected (auto-reconnect on drop)
   - Calls `run_inference()` to classify the current radar frame
   - Sends the result to the server via `api_client.send_detection()`

#### `run_inference()` — Data Flow

```
Radar ADC (128 samples) → get_raw_samples() → normalize [0, 1] → enqueue → dequeue → label
```

| Step | Description |
|------|-------------|
| 1 | `radar.get_raw_samples()` triggers a chirp, reads the FIFO, unpacks ADC data |
| 2 | Each sample is divided by `4095.0` to normalize from 12-bit range to `[0, 1]` |
| 3 | `model.enqueue(buffer)` feeds the frame into the model's sliding window |
| 4 | `model.dequeue(output)` returns `0` when the window is full and a prediction is ready |
| 5 | The class with the highest probability is returned as `(label, confidence)` |

**DeepCraft return codes:**
- `0` — prediction ready (output written to buffer)
- `-1` — sliding window not yet full, keep feeding
- `-2` — memory allocation error (fatal)

---

## Radar Library (`radar_lib.py`)

### `RadarController` Class

Encapsulates the BGT60TR13C radar sensor configuration and data acquisition.

#### Constructor Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `start_freq` | `57_500_000` | Chirp start frequency (57.5 GHz) |
| `bandwidth` | `5_000_000` | Chirp bandwidth (5 MHz) |
| `adc_div` | `60` | ADC clock divider |
| `samples_per_chirp` | `128` | Number of ADC samples per chirp |
| `no_of_chirps` | `1` | Number of chirps per frame |

#### SPI Pin Mapping (PSoC6)

| Signal | Pin |
|--------|-----|
| SCK    | P12_2 |
| MOSI   | P12_0 |
| MISO   | P12_1 |
| CS     | P12_3 |
| RST    | P11_1 |
| IRQ    | P11_0 |

#### Key Methods

| Method | Description |
|--------|-------------|
| `setup()` | Resets and initialises the sensor hardware |
| `get_raw_samples()` | Triggers a frame → reads FIFO → unpacks → returns 128 ADC floats |

#### Utility Functions (Module-Level)

| Function | Description |
|----------|-------------|
| `compute_fft_1d(x)` | Pure-Python recursive Cooley-Tukey FFT (power-of-2 length) |
| `compute_frobenius_norm(data)` | Frobenius norm of a 1D or 2D array (supports complex values) |

> **Note:** These utility functions are not used in the current raw-data model but are available for FFT-based models.

### `lib/` — Low-Level Radar Libraries

| File | Purpose |
|------|---------|
| `BGT60TRXX.py` | Full sensor driver: SPI register access, FIFO read, chirp config, frame triggering, data unpacking, signal processing (`runHighPassFilter`, `fillFFT_data`, `fft_to_dB`, `runAntiCouplingFilter`) |
| `BGT60TRXX_define.py` | Register addresses and bit masks for the BGT60TR13C |
| `dft.py` | Performance-optimised DFT using MicroPython `@viper` and `@native` decorators |
| `dftclass.py` | Object-oriented FFT wrapper (`DFT.FORWARD`, `DFT.INVERSE`) |
| `polar.py` | Cartesian-to-polar conversion for FFT results |
| `window.py` | Windowing functions for spectral analysis |

---

## DeepCraft Model Variants

Three model variants are stored under `Models/`. Each contains a single `deepcraft_model.mpy` file compiled from the DeepCraft cloud platform.

| Model | Directory | Description |
|-------|-----------|-------------|
| Early Small | `13_02_small/` | Initial small model from 13 Feb. Trained on FFT data (65 features). |
| Medium Final | `Medium_final/` | Larger model variant with higher accuracy but more RAM/latency. |
| **Final Small** ★ | `Model_final_small/` | **Currently deployed.** Trained on raw 128-sample radar vectors. Smallest footprint. |

### DeepCraft API

The compiled `.mpy` model exposes the following API:

```python
import deepcraft_model

model = deepcraft_model.DEEPCRAFT()
model.init()

input_dim  = model.get_model_input_dim()   # → 128
output_dim = model.get_model_output_dim()   # → 3

model.enqueue(input_buffer)     # Feed one time step (array.array('f'))
result = model.dequeue(output)  # Check for prediction
```

### Output Labels

The model classifies radar frames into 3 classes:

| Index | Label | Description |
|-------|-------|-------------|
| 0 | `empty_box` | Empty container detected |
| 1 | `table` | Flat surface / table detected |
| 2 | `unlabeled` | No clear classification |

---

## Configuration (`config.py`)

This file is **auto-generated** by `full_setup.py` during device provisioning. All runtime parameters are centralised here.

```python
# ---- Wi-Fi ----
WIFI_SSID = "YourNetwork"
WIFI_PASS = "YourPassword"
WIFI_TIMEOUT = 15               # seconds

# ---- Server ----
SERVER_HOST = "192.168.0.129"
SERVER_PORT = 5000
API_BASE = "http://192.168.0.129:5000"

# ---- Device Identity ----
DEVICE_ID = 4                   # Assigned by server on registration
API_KEY = "abc123..."           # Used in X-API-Key header

# ---- Detection Loop ----
DETECTION_INTERVAL = 10         # seconds between inference cycles

# ---- Hardware ----
LED_PIN = "P13_7"               # On-board LED
```

---

## API Client (`api_client.py`)

Communicates with the inventory server over HTTP using MicroPython's `urequests` module. All requests include an `X-API-Key` header for authentication.

### API Routes

| Method | Endpoint | Description | Body |
|--------|----------|-------------|------|
| `GET` | `/api/health` | Health check — returns 200 if server is up | — |
| `POST` | `/api/detections` | Submit a detection event | `{ device_id, label, confidence, status }` |
| `GET` | `/api/inventory` | Fetch current inventory state | — |

### Functions

#### `health_check() → bool`
Pings the server's health endpoint. Returns `True` if the server responds with HTTP 200.

#### `send_detection(label, confidence, status="detected") → dict | None`
Posts a classification result to the server.

```python
# Example payload sent:
{
    "device_id": 4,
    "label": "empty_box",
    "confidence": 0.87,
    "status": "detected"
}
```

**Response:** returns parsed JSON on `201 Created`, or `None` on failure.

#### `get_inventory() → list`
Fetches the current inventory from the server. Returns a list of item dicts.

### How HTTP Requests Work on MicroPython

MicroPython uses `urequests` (a minimal HTTP client) instead of Python's `requests` library:

```python
import urequests
import ujson

# GET request
response = urequests.get("http://192.168.0.129:5000/api/health")
print(response.status_code)  # 200
response.close()             # Always close to free the socket!

# POST request with JSON body
headers = {
    "Content-Type": "application/json",
    "X-API-Key": "your-api-key"
}
payload = ujson.dumps({"label": "empty_box", "confidence": 0.9})

response = urequests.post(
    "http://192.168.0.129:5000/api/detections",
    data=payload,
    headers=headers
)
print(response.status_code)  # 201
data = response.json()       # Parse JSON response
response.close()
```

> **⚠️ Important:** Always call `response.close()` after reading the response. MicroPython has limited socket resources; unclosed connections will cause memory leaks and eventual crashes.

---

## Wi-Fi Manager (`wifi.py`)

Handles all Wi-Fi operations using MicroPython's `network` module in STA (station) mode.

| Function | Description |
|----------|-------------|
| `connect()` | Connect to the configured network. Raises `RuntimeError` on timeout. |
| `disconnect()` | Deactivate the WLAN interface. |
| `is_connected()` | Returns `True` if currently connected. |
| `get_ip()` | Returns the current IP address string, or `None`. |
| `ensure_connected()` | Auto-reconnects if the connection was dropped. Called every cycle. |

---

## Quick Reference — Full Execution Flow

```
┌─────────────────────────────────────────────────────────────┐
│                        POWER ON                             │
├─────────────────────────────────────────────────────────────┤
│  boot.py                                                    │
│    ├─ gc.collect()                                          │
│    ├─ import deepcraft_model, wifi, radar_lib               │
│    ├─ wifi.connect()                                        │
│    └─ radar = RadarController() → radar.setup()             │
├─────────────────────────────────────────────────────────────┤
│  main.py                                                    │
│    ├─ model = DEEPCRAFT() → model.init()                    │
│    ├─ Allocate input/output buffers (128 × float32)         │
│    └─ LOOP every 10s:                                       │
│         ├─ wifi.ensure_connected()                          │
│         ├─ run_inference()                                  │
│         │    ├─ radar.get_raw_samples()   → 128 ADC values  │
│         │    ├─ normalize → [0, 1]                          │
│         │    ├─ model.enqueue(buffer)                       │
│         │    └─ model.dequeue(output) → (label, confidence) │
│         ├─ api_client.send_detection(label, confidence)     │
│         └─ time.sleep(10)                                   │
└─────────────────────────────────────────────────────────────┘
```
