# ============================================================
# main.py — Detection Loop Entry Point
# ============================================================
# Runs after boot.py.  Performs the following cycle:
#   1. Ensure Wi-Fi is connected (reconnect if dropped)
#   2. Run AI inference (placeholder — integrate DeepCraft)
#   3. Send detection result to the inventory server
#   4. Sleep and repeat
# ============================================================

import time

import config
import wifi
import api_client
import array
from radar_lib import compute_fft_1d, compute_frobenius_norm


IMAI_DATA_OUT_SYMBOLS = ['empty_box', 'table', 'unlabeled']

output_buffer = array.array('f', [0.0] * len(IMAI_DATA_OUT_SYMBOLS))
def run_inference(model, enqueue_buffer, output_buffer, input_dim, output_dim):
    """
    Run the AI model and return (label, confidence).

    >>> REPLACE THIS with your actual DeepCraft model call <<<

    Example with DeepCraft:
        from deepcraft import Model
        model = Model("inventory_classifier")
        result = model.predict(camera.capture())
        return result.label, result.confidence
    """
    global radar
    
    # 1. Get Raw Data
    while True:
        # 1. Get Raw Data
        raw_data = radar.read_fifo()
            
        if not raw_data:
            time.sleep_ms(5)
            continue
            
        # 2. Compute FFT
        # Ensure raw_data length is a power of 2 for this basic FFT algorithm
        fft_data = compute_fft_1d(raw_data)
            
        # 3. Compute Frobenius Norm on the FFT data
        f_norm = compute_frobenius_norm(fft_data)
            
        # 4. Map Features to the Enqueue Buffer
       
        for i in range(min(input_dim, len(fft_data))):
            val = fft_data[i]
            # Convert complex FFT output to absolute magnitude
            enqueue_buffer[i] = (val.real**2 + val.imag**2)**0.5

        # 5. Enqueue data to the model's sliding window
        model.enqueue(enqueue_buffer)
        
        # 6. Dequeue to check if inference is ready
        result = model.dequeue(output_buffer)

        # result == 0 means the stride is complete and prediction is ready
        if result == 0:
            max_idx = 0
            max_val = output_buffer[0]
            for i in range(1, output_dim):
                if output_buffer[i] > max_val:
                    max_val = output_buffer[i]
                    max_idx = i
            
            # Prevent out-of-bounds if model outputs more classes than symbols
            if max_idx < len(IMAI_DATA_OUT_SYMBOLS):
                label = IMAI_DATA_OUT_SYMBOLS[max_idx]
            else:
                label = "unknown"
                
            return label, max_val
            
        elif result == -2:
            print("[Inference] ERROR: Memory allocation error inside model.")
            return "error", 0.0
            
        # If result == -1, the sliding window isn't full/ready yet. 
        # The while loop will naturally continue to grab the next radar frame.
        
    
    

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
