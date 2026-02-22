# ============================================================
# main.py — Detection Loop Entry Point
# ============================================================
# Runs after boot.py.  Performs the following cycle:
#   1. Ensure Wi-Fi is connected (reconnect if dropped)
#   2. Run AI inference on raw radar data
#   3. Send detection result to the inventory server
#   4. Sleep and repeat
# ============================================================

import array
import time

import config

import api_client

import wifi


IMAI_DATA_OUT_SYMBOLS = ['unlabeled', 'empty_box', 'table']


def run_inference(model, enqueue_buffer, output_buffer, input_dim, output_dim):
    """
    Run the AI model on raw radar data and return (label, confidence).
    The model was trained on raw 128-sample radar vectors — no FFT needed.
    """
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
        for i in range(min(input_dim, len(raw_data))):
            enqueue_buffer[i] = float(raw_data[i]) / 4095.0

        # 3. Enqueue data to the model's sliding window
        model.enqueue(enqueue_buffer)
        
        # 4. Dequeue to check if inference is ready
        result = model.dequeue(output_buffer)

        # result == 0 means the stride is complete and prediction is ready
        if result == 0:
            max_idx = 0
            max_val = output_buffer[0]
            for i in range(1, output_dim):
                if output_buffer[i] > max_val:
                    max_val = output_buffer[i]
                    max_idx = i
            
            if max_idx < len(IMAI_DATA_OUT_SYMBOLS):
                label = IMAI_DATA_OUT_SYMBOLS[max_idx]
            else:
                label = "unknown"
            
            # Clear output buffer to avoid stale data on next dequeue
            for i in range(output_dim):
                output_buffer[i] = 0.0
                
            return label, max_val
            
        elif result == -2:
            print("[Inference] ERROR: Memory allocation error inside model.")
            return "error", 0.0
            
        # result == -1: sliding window not full yet, keep feeding frames


def main():
    print("\n[Main] Starting detection loop")
    print("  Server :", config.API_BASE)
    print("  Device :", config.DEVICE_ID)
    print("  Interval:", config.DETECTION_INTERVAL, "s\n")
    
    # --- Initialize DeepCraft Model ---
    print("[Main] Initialising DEEPCRAFT model...")
    model = deepcraft_model.DEEPCRAFT()
    model.init()

    input_dim  = model.get_model_input_dim()
    output_dim = model.get_model_output_dim()

    enqueue_buffer = array.array('f', [0.0] * input_dim)
    output_buffer  = array.array('f', [0.0] * output_dim)
    
    print(f"[Main] Model initialised!")
    print(f"  -> Input  dimensions : {input_dim}")
    print(f"  -> Output dimensions : {output_dim}\n")
    
    # Initial server check
    wifi.ensure_connected()

    if api_client.health_check():
        print("[Main] Server is reachable!\n")
    else:
        print("[Main] WARNING: Server not reachable — will retry per cycle\n")

    # Detection loop
    cycle = 0
    while True:
        cycle += 1
        print("--- Cycle {} ---".format(cycle))

        try:
            # 1. Ensure Wi-Fi
            wifi.ensure_connected()

            # 2. Run inference
            label, confidence = run_inference(model, enqueue_buffer, output_buffer, input_dim, output_dim)
            print("[Inference] {} ({:.1f}%)".format(label, confidence * 100))

            # 3. Send to server
            api_client.send_detection(label, confidence)

        except Exception as e:
            print("[Main] Error in cycle {}:".format(cycle), e)

        # 4. Wait
        print()
        time.sleep(config.DETECTION_INTERVAL)


# ============================================================
# Entry point
# ============================================================
main()
