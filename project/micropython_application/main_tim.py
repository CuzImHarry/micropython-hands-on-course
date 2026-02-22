# ============================================================
# main_tim.py — Timer-Based Detection Loop
# ============================================================
# Alternative to main.py that uses machine.Timer for periodic
# inference instead of a blocking time.sleep() loop.
# The timer fires a callback every DETECTION_INTERVAL seconds.
# ============================================================

import array
import time
from machine import Timer

import config
import api_client
import wifi


IMAI_DATA_OUT_SYMBOLS = ['unlabeled', 'empty_box', 'table']


# ---- Global state (shared between setup and timer callback) ----
_model = None
_enqueue_buffer = None
_output_buffer = None
_input_dim = 0
_output_dim = 0
_cycle = 0


def run_inference():
    """
    Run the AI model on raw radar data and return (label, confidence).
    The model was trained on raw 128-sample radar vectors — no FFT needed.
    """
    global _model, _enqueue_buffer, _output_buffer, _input_dim, _output_dim

    # Access the global radar object initialised in boot.py
    import __main__
    radar = getattr(__main__, 'radar', None)

    if radar is None:
        print("[Inference] ERROR: Radar object not found.")
        return "error", 0.0

    while True:
        # 1. Get raw ADC samples from radar (128 floats)
        raw_data = radar.get_raw_samples()

        if raw_data is None:
            print("[Inference] No data from radar, retrying...")
            time.sleep_ms(50)
            continue

        # 2. Normalize raw samples to [0, 1] and copy into enqueue buffer
        #    12-bit ADC range is 0-4095; DeepCraft expects values in [0, 1]
        for i in range(min(_input_dim, len(raw_data))):
            _enqueue_buffer[i] = float(raw_data[i]) / 4095.0

        # 3. Enqueue data to the model's sliding window
        _model.enqueue(_enqueue_buffer)

        # 4. Dequeue to check if inference is ready
        result = _model.dequeue(_output_buffer)

        # result == 0 means the stride is complete and prediction is ready
        if result == 0:
            max_idx = 0
            max_val = _output_buffer[0]
            for i in range(1, _output_dim):
                if _output_buffer[i] > max_val:
                    max_val = _output_buffer[i]
                    max_idx = i

            if max_idx < len(IMAI_DATA_OUT_SYMBOLS):
                label = IMAI_DATA_OUT_SYMBOLS[max_idx]
            else:
                label = "unknown"

            # Clear output buffer to avoid stale data on next dequeue
            for i in range(_output_dim):
                _output_buffer[i] = 0.0

            return label, max_val

        elif result == -2:
            print("[Inference] ERROR: Memory allocation error inside model.")
            return "error", 0.0

        # result == -1: sliding window not full yet, keep feeding frames


def timer_callback(timer):
    """
    Called by machine.Timer every DETECTION_INTERVAL seconds.
    Runs one inference cycle and sends the result to the server.
    """
    global _cycle
    _cycle += 1
    print("--- Cycle {} ---".format(_cycle))

    try:
        # 1. Ensure Wi-Fi
        wifi.ensure_connected()

        # 2. Run inference
        label, confidence = run_inference()
        print("[Inference] {} ({:.1f}%)".format(label, confidence * 100))

        # 3. Send to server
        api_client.send_detection(label, confidence)

    except Exception as e:
        print("[Main] Error in cycle {}:".format(_cycle), e)

    print()


def main():
    global _model, _enqueue_buffer, _output_buffer, _input_dim, _output_dim

    print("\n[Main] Starting timer-based detection loop")
    print("  Server :", config.API_BASE)
    print("  Device :", config.DEVICE_ID)
    print("  Interval:", config.DETECTION_INTERVAL, "s\n")

    # --- Initialize DeepCraft Model ---
    print("[Main] Initialising DEEPCRAFT model...")
    import deepcraft_model
    _model = deepcraft_model.DEEPCRAFT()
    _model.init()

    _input_dim  = _model.get_model_input_dim()
    _output_dim = _model.get_model_output_dim()

    _enqueue_buffer = array.array('f', [0.0] * _input_dim)
    _output_buffer  = array.array('f', [0.0] * _output_dim)

    print(f"[Main] Model initialised!")
    print(f"  -> Input  dimensions : {_input_dim}")
    print(f"  -> Output dimensions : {_output_dim}\n")

    # Initial server check
    wifi.ensure_connected()

    if api_client.health_check():
        print("[Main] Server is reachable!\n")
    else:
        print("[Main] WARNING: Server not reachable — will retry per cycle\n")

    # --- Setup Timer ---
    # Timer period is in milliseconds
    period_ms = config.DETECTION_INTERVAL * 1000
    tim = Timer(0)
    tim.init(period=period_ms, mode=Timer.PERIODIC, callback=timer_callback)

    print("[Main] Timer started — callback every {}s".format(config.DETECTION_INTERVAL))
    print("[Main] MCU is free between callbacks.\n")


# ============================================================
# Entry point
# ============================================================
main()
