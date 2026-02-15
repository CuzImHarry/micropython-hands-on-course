# Edge AI Inventory System — Documentation

> Real-time warehouse inventory management powered by Edge AI classification on PSoC6 devices.

---

## Table of Contents

1. [Architecture Overview](#architecture-overview)
2. [Backend Structure](#backend-structure)
3. [Database Schema](#database-schema)
4. [API Reference](#api-reference)
5. [Frontend Usage](#frontend-usage)
6. [MicroPython Client Example](#micropython-client-example)

---

## Architecture Overview

```
┌──────────────────┐        HTTP/JSON         ┌──────────────────┐
│  PSoC6 Device    │ ──────────────────────►   │  Flask Backend   │
│  (MicroPython)   │   POST /api/detections    │  (port 5000)     │
│  + DeepCraft AI  │   X-API-Key auth          │  SQLite DB       │
└──────────────────┘                           └────────┬─────────┘
                                                        │
                                                        │ REST API
                                                        ▼
                                               ┌──────────────────┐
                                               │  Next.js Frontend│
                                               │  (port 3000)     │
                                               │  Dashboard UI    │
                                               └──────────────────┘
```

**Flow:**
1. A PSoC6 edge device runs an AI model (via DeepCraft) to classify objects.
2. Classification results are sent as HTTP POST requests to the Flask API.
3. The backend stores detections and updates inventory state in SQLite.
4. The Next.js dashboard polls the API every 5 seconds and displays live data.

---

## Backend Structure

```
Serverapp/
├── app.py                 # Flask application factory
├── models.py              # SQLAlchemy models (Device, InventoryItem, DetectionEvent)
├── requirements.txt       # Python dependencies
├── inventory.db           # SQLite database (auto-created)
└── routes/
    ├── __init__.py
    ├── devices.py         # CRUD for device management
    ├── detections.py      # Detection ingestion + inventory upsert
    └── inventory.py       # Inventory queries + dashboard stats
```

### Dependencies

| Package            | Version | Purpose                     |
|--------------------|---------|------------------------------|
| Flask              | 3.1.0   | Web framework                |
| Flask-CORS         | 5.0.1   | Cross-Origin Resource Sharing|
| Flask-SQLAlchemy   | 3.1.1   | ORM for SQLite               |
| marshmallow        | 3.23.2  | Serialization (available)    |
| python-dotenv      | 1.0.1   | Environment variable loading |

### Running the Backend

```bash
cd Serverapp
pip install -r requirements.txt
python app.py
# → Runs on http://0.0.0.0:5000
```

---

## Database Schema

The application uses **SQLite** with three tables:

### `devices`

| Column       | Type         | Constraints               | Description                 |
|-------------|-------------|---------------------------|-----------------------------|
| `id`        | Integer     | PRIMARY KEY, auto-increment| Unique device ID            |
| `name`      | String(100) | NOT NULL, UNIQUE           | Device name (e.g. "psoc6-01")|
| `location`  | String(200) | NOT NULL                   | Physical location           |
| `api_key`   | String(64)  | NOT NULL, UNIQUE           | Auto-generated auth token   |
| `status`    | String(20)  | DEFAULT "online"           | Current status              |
| `last_seen` | DateTime    | DEFAULT now()              | Last heartbeat timestamp    |
| `created_at`| DateTime    | DEFAULT now()              | Registration timestamp      |

### `inventory_items`

| Column       | Type         | Constraints               | Description                 |
|-------------|-------------|---------------------------|-----------------------------|
| `id`        | Integer     | PRIMARY KEY, auto-increment| Unique item ID              |
| `device_id` | Integer     | FK → devices.id, NOT NULL  | Reporting device            |
| `label`     | String(100) | NOT NULL                   | Classification label        |
| `status`    | String(20)  | DEFAULT "detected"         | "detected" or "cleared"     |
| `confidence`| Float       | DEFAULT 0.0                | Model confidence (0.0–1.0)  |
| `updated_at`| DateTime    | DEFAULT now(), auto-update | Last change timestamp       |

### `detection_events`

| Column       | Type         | Constraints               | Description                 |
|-------------|-------------|---------------------------|-----------------------------|
| `id`        | Integer     | PRIMARY KEY, auto-increment| Unique event ID             |
| `device_id` | Integer     | FK → devices.id, NOT NULL  | Reporting device            |
| `label`     | String(100) | NOT NULL                   | Classification label        |
| `confidence`| Float       | NOT NULL                   | Model confidence (0.0–1.0)  |
| `timestamp` | DateTime    | DEFAULT now()              | Detection timestamp         |

### Relationships

```
Device (1) ──► (N) InventoryItem
Device (1) ──► (N) DetectionEvent
```

- One inventory item per unique `(device_id, label)` combination (upserted on each detection).
- Every detection creates a new event in `detection_events` (full history log).

---

## API Reference

**Base URL:** `http://<server-ip>:5000`

### Health Check

| Method | Endpoint       | Auth | Description          |
|--------|---------------|------|----------------------|
| GET    | `/api/health` | None | Returns `{"status": "ok"}` |

---

### Devices

#### `GET /api/devices`

List all registered devices.

**Response:** `200 OK`
```json
[
  {
    "id": 1,
    "name": "psoc6-01",
    "location": "Warehouse A",
    "api_key": "a1b2c3d4...",
    "status": "online",
    "last_seen": "2026-02-15T17:30:00",
    "created_at": "2026-02-15T10:00:00"
  }
]
```

---

#### `GET /api/devices/<id>`

Get a single device by ID.

**Response:** `200 OK` — Single device object (same shape as above).

---

#### `POST /api/devices`

Register a new edge device. Returns the device with a **generated API key** (save this!).

**Request Body:**
```json
{
  "name": "psoc6-01",
  "location": "Warehouse A"
}
```

**Response:** `201 Created`
```json
{
  "id": 1,
  "name": "psoc6-01",
  "location": "Warehouse A",
  "api_key": "e4f5a6b7c8d9...64-char-hex-token",
  "status": "online",
  "last_seen": "2026-02-15T17:30:00",
  "created_at": "2026-02-15T17:30:00"
}
```

**Errors:**
- `400` — Missing `name` or `location`
- `409` — Device name already exists

---

#### `PUT /api/devices/<id>`

Update device name and/or location.

**Request Body:**
```json
{
  "name": "psoc6-01-updated",
  "location": "Warehouse B"
}
```

**Response:** `200 OK` — Updated device object.

---

#### `DELETE /api/devices/<id>`

Delete a device.

**Response:** `200 OK`
```json
{ "message": "Device deleted" }
```

---

### Detections (Main Device Endpoint)

#### `POST /api/detections` ⭐

**This is the primary endpoint your MicroPython device should call.**

Submits a classification result. Automatically:
- Creates a detection event (history log)
- Upserts the inventory item (one per device+label)
- Updates device `last_seen` and sets status to "online"

**Headers:**
```
X-API-Key: <device-api-key>
Content-Type: application/json
```

**Request Body:**
```json
{
  "device_id": 1,
  "label": "empty_box",
  "confidence": 0.95,
  "status": "detected"
}
```

| Field        | Type   | Required | Default      | Description                        |
|-------------|--------|----------|--------------|------------------------------------|
| `device_id` | int    | ✅       |              | ID of the registered device        |
| `label`     | string | ✅       |              | Classification label from AI model |
| `confidence`| float  | ❌       | 0.0          | Model confidence (0.0–1.0)         |
| `status`    | string | ❌       | "detected"   | "detected" or "cleared"            |

**Response:** `201 Created`
```json
{
  "message": "Detection recorded",
  "event": {
    "id": 42,
    "device_id": 1,
    "device_name": "psoc6-01",
    "label": "empty_box",
    "confidence": 0.95,
    "timestamp": "2026-02-15T17:35:00"
  },
  "inventory_item": {
    "id": 7,
    "device_id": 1,
    "device_name": "psoc6-01",
    "device_location": "Warehouse A",
    "label": "empty_box",
    "status": "detected",
    "confidence": 0.95,
    "updated_at": "2026-02-15T17:35:00"
  }
}
```

**Errors:**
- `401` — Missing `X-API-Key` header
- `400` — Missing JSON body or required fields
- `403` — API key doesn't match device
- `404` — Device not found

---

#### `GET /api/detections`

Query detection event history.

**Query Parameters:**
| Param      | Type | Default | Description               |
|-----------|------|---------|---------------------------|
| `device_id`| int  | —       | Filter by device          |
| `label`   | str  | —       | Filter by label           |
| `limit`   | int  | 50      | Max number of results     |

**Response:** `200 OK` — Array of detection event objects.

---

### Inventory

#### `GET /api/inventory`

Get current inventory state.

**Query Parameters:**
| Param      | Type | Default | Description               |
|-----------|------|---------|---------------------------|
| `device_id`| int  | —       | Filter by device          |
| `label`   | str  | —       | Filter by label           |
| `status`  | str  | —       | Filter: "detected"/"cleared"|

**Response:** `200 OK` — Array of inventory item objects.

---

#### `GET /api/inventory/stats`

Aggregate statistics for the dashboard.

**Response:** `200 OK`
```json
{
  "total_items": 12,
  "items_by_label": {
    "empty_box": { "detected": 3, "cleared": 1 },
    "full_box": { "detected": 5, "cleared": 3 }
  },
  "active_devices": 2,
  "total_devices": 3,
  "total_detections": 156,
  "recent_detections": 14
}
```

- `active_devices` = devices seen in the last 5 minutes
- `recent_detections` = detections in the last 1 hour

---

## Frontend Usage

### Tech Stack

- **Next.js 16** with Turbopack
- **TypeScript** with React
- Client-side rendering with `"use client"` components
- CSS-only styling (glassmorphism design, dark theme)

### Running the Frontend

```bash
cd Serverapp/frontend
npm install
npm run dev
# → Runs on http://localhost:3000
```

Set `NEXT_PUBLIC_API_URL` environment variable to point to a different backend (default: `http://localhost:5000`).

### Dashboard Components

The dashboard (`page.tsx`) auto-refreshes every **5 seconds** and consists of:

| Component         | File                    | Description                                      |
|------------------|-------------------------|--------------------------------------------------|
| **StatsBar**     | `StatsBar.tsx`          | 4 KPI cards: Inventory Items, Active Devices, Recent (1h), Total Detections |
| **InventoryGrid**| `InventoryGrid.tsx`     | Grid of cards for each inventory item, showing label, status, device, confidence bar |
| **EventFeed**    | `EventFeed.tsx`         | Live scrolling feed of recent detection events    |
| **DeviceStatus** | `DeviceStatus.tsx`      | List of registered devices with online/offline indicators (5-min threshold) |

### UI Features

- **Live status indicator** — green dot with "Live · Updated HH:MM:SS" in header
- **Error banner** — shows disconnected state with troubleshooting hint
- **Loading shimmer** — skeleton loading states for all components
- **Empty states** — helpful messages when no data exists yet
- **Confidence bars** — visual progress bars for detection confidence

---

## MicroPython Client Example

This example shows how to connect a PSoC6 board (or any MicroPython device) to the inventory API using the `network` and `urequests` modules.

### Prerequisites

1. Register your device via the API first (or use the dashboard):
   ```bash
   curl -X POST http://<server-ip>:5000/api/devices \
     -H "Content-Type: application/json" \
     -d '{"name": "psoc6-01", "location": "Warehouse A"}'
   ```
2. Save the returned `id` and `api_key` values.

### Full Client Code

```python
import network
import urequests
import ujson
import time

# ============================================================
# CONFIGURATION — Update these values for your setup
# ============================================================
WIFI_SSID = "YourWiFiSSID"
WIFI_PASS = "YourWiFiPassword"

SERVER_IP = "192.168.1.100"          # IP of the machine running Flask
SERVER_PORT = 5000
API_BASE = "http://{}:{}".format(SERVER_IP, SERVER_PORT)

DEVICE_ID = 1                         # From POST /api/devices response
API_KEY = "your-64-char-hex-api-key"  # From POST /api/devices response

DETECTION_INTERVAL = 10               # Seconds between detections


# ============================================================
# Wi-Fi Connection
# ============================================================
def connect_wifi():
    """Connect to Wi-Fi and return the interface."""
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)

    if wlan.isconnected():
        print("[WiFi] Already connected:", wlan.ifconfig())
        return wlan

    print("[WiFi] Connecting to", WIFI_SSID, "...")
    wlan.connect(WIFI_SSID, WIFI_PASS)

    # Wait up to 15 seconds for connection
    timeout = 15
    while not wlan.isconnected() and timeout > 0:
        time.sleep(1)
        timeout -= 1
        print("  waiting... ({})".format(timeout))

    if wlan.isconnected():
        print("[WiFi] Connected! IP:", wlan.ifconfig()[0])
    else:
        raise RuntimeError("[WiFi] Failed to connect!")

    return wlan


# ============================================================
# API Client
# ============================================================
def send_detection(label, confidence, status="detected"):
    """
    Send a detection result to the server.

    Args:
        label (str):      Classification label, e.g. "empty_box", "full_box"
        confidence (float): Model confidence between 0.0 and 1.0
        status (str):     "detected" or "cleared"

    Returns:
        dict: Server response or None on failure
    """
    url = API_BASE + "/api/detections"

    headers = {
        "Content-Type": "application/json",
        "X-API-Key": API_KEY,
    }

    payload = ujson.dumps({
        "device_id": DEVICE_ID,
        "label": label,
        "confidence": confidence,
        "status": status,
    })

    try:
        print("[API] POST", url)
        print("  payload:", payload)

        response = urequests.post(url, data=payload, headers=headers)

        print("  status:", response.status_code)

        if response.status_code == 201:
            result = response.json()
            print("  OK:", result.get("message", ""))
            response.close()
            return result
        else:
            print("  ERROR:", response.text)
            response.close()
            return None

    except Exception as e:
        print("[API] Request failed:", e)
        return None


def health_check():
    """Verify the server is reachable."""
    try:
        url = API_BASE + "/api/health"
        response = urequests.get(url)
        ok = response.status_code == 200
        response.close()
        return ok
    except Exception:
        return False


# ============================================================
# Main Loop
# ============================================================
def main():
    # 1. Connect to Wi-Fi
    wlan = connect_wifi()

    # 2. Check server health
    print("\n[Main] Checking server health...")
    if health_check():
        print("[Main] Server is reachable!\n")
    else:
        print("[Main] WARNING: Server not reachable at", API_BASE)
        print("  Will retry on each detection.\n")

    # 3. Detection loop
    #    Replace this with your actual AI model inference
    print("[Main] Starting detection loop...\n")

    while True:
        # ------------------------------------------------
        # >>> YOUR AI MODEL INFERENCE GOES HERE <<<
        # Example: result = model.predict(camera_frame)
        #          label = result.label
        #          confidence = result.confidence
        # ------------------------------------------------

        # Simulated detection (replace with real model output)
        label = "empty_box"
        confidence = 0.92

        # Send to server
        send_detection(label, confidence)

        # Wait before next detection
        time.sleep(DETECTION_INTERVAL)


# ============================================================
# Entry Point
# ============================================================
if __name__ == "__main__":
    main()
```

### Quick Reference

| Action                 | Code                                                   |
|-----------------------|-------------------------------------------------------|
| **Send detection**    | `send_detection("full_box", 0.95)`                     |
| **Mark item cleared** | `send_detection("full_box", 0.95, status="cleared")`   |
| **Health check**      | `health_check()  # returns True/False`                  |

### Troubleshooting

| Issue                    | Solution                                               |
|--------------------------|--------------------------------------------------------|
| Connection refused       | Ensure Flask is running with `host="0.0.0.0"` (not `127.0.0.1`) |
| 401 Unauthorized         | Check `X-API-Key` header matches the device's API key  |
| 403 Forbidden            | API key doesn't match `device_id` — verify both values |
| 404 Not Found            | Device ID doesn't exist — register via `POST /api/devices` first |
| `ENOMEM` / memory error  | Close response objects with `response.close()` after use |
| Wi-Fi won't connect      | Verify SSID/password, check signal strength, try restarting the board |

---

## cURL Examples (Testing)

### Register a device
```bash
curl -X POST http://localhost:5000/api/devices \
  -H "Content-Type: application/json" \
  -d '{"name": "psoc6-01", "location": "Warehouse A"}'
```

### Send a detection
```bash
curl -X POST http://localhost:5000/api/detections \
  -H "Content-Type: application/json" \
  -H "X-API-Key: YOUR_API_KEY_HERE" \
  -d '{"device_id": 1, "label": "empty_box", "confidence": 0.95}'
```

### Query inventory
```bash
curl http://localhost:5000/api/inventory
```

### Get dashboard stats
```bash
curl http://localhost:5000/api/inventory/stats
```

### Get detection history
```bash
curl "http://localhost:5000/api/detections?limit=10"
```
