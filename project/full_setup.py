import os
import subprocess
import time
import requests
import socket
import sys

def get_local_ip():
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(("8.8.8.8", 80))
        ip = s.getsockname()[0]
        s.close()
        return ip
    except Exception:
        return "127.0.0.1"

def run_command(command, cwd=None):
    print(f"Executing: {command}")
    return subprocess.run(command, shell=True, cwd=cwd)

def main():
    print("==========================================")
    print("   Edge AI Inventory System Setup Tool")
    print("==========================================")

    # 1. Collect Inputs
    ssid = input("WLAN SSID: ").strip()
    password = input("WLAN Password: ").strip()
    device_name = input("Device Name (e.g., psoc6-01): ").strip()
    location = input("Location (e.g., Warehouse A): ").strip()
    
    default_ip = get_local_ip()
    server_ip = input(f"Server IP [{default_ip}]: ").strip() or default_ip
    server_port = 5000
    api_url = f"http://{server_ip}:{server_port}"

    # 2. Start Backend
    print("\n[1/5] Starting Backend and Frontend servers...")
    backend_path = os.path.join(os.getcwd(), "Serverapp")
    # Using start to run it in a new window so it doesn't block this script
    subprocess.Popen(["cmd", "/c", "setup_and_run.bat"], cwd=backend_path, shell=True)

    # 3. Wait for Backend to be healthy
    print(f"[2/5] Waiting for API to be ready at {api_url}/api/health...")
    max_retries = 30
    healthy = False
    for i in range(max_retries):
        try:
            resp = requests.get(f"{api_url}/api/health", timeout=2)
            if resp.status_code == 200:
                print("      API is healthy!")
                healthy = True
                break
        except Exception:
            pass
        print(f"      Waiting... ({i+1}/{max_retries})")
        time.sleep(2)

    if not healthy:
        print("Error: Backend did not start in time. Please check the backend window.")
        sys.exit(1)

    # 4. Register Device
    print(f"[3/5] Registering device '{device_name}'...")
    try:
        reg_resp = requests.post(
            f"{api_url}/api/devices",
            json={"name": device_name, "location": location}
        )
        if reg_resp.status_code == 201:
            data = reg_resp.json()
            device_id = data["id"]
            api_key = data["api_key"]
            print(f"      Device Registered! ID: {device_id}, API Key: {api_key[:8]}...")
        elif reg_resp.status_code == 409:
            # Device already exists, try to find it
            print("      Device already exists. Fetching existing device data...")
            devices_resp = requests.get(f"{api_url}/api/devices")
            all_devices = devices_resp.json()
            device_data = next((d for d in all_devices if d["name"] == device_name), None)
            if device_data:
                device_id = device_data["id"]
                api_key = device_data["api_key"]
                print(f"      Found existing device! ID: {device_id}, API Key: {api_key[:8]}...")
            else:
                print("Error: Could not retrieve existing device data.")
                sys.exit(1)
        else:
            print(f"Error registering device: {reg_resp.text}")
            sys.exit(1)
    except Exception as e:
        print(f"Request failed: {e}")
        sys.exit(1)

    # 5. Update config.py
    print("[4/5] Updating MicroPython configuration...")
    config_path = os.path.join(os.getcwd(), "micropython_application", "config.py")
    
    config_content = f"""# ============================================================
# Edge AI Inventory — Device Configuration (AUTO-GENERATED)
# ============================================================

# ---- Wi-Fi ----
WIFI_SSID = "{ssid}"
WIFI_PASS = "{password}"
WIFI_TIMEOUT = 15

# ---- Server ----
SERVER_HOST = "{server_ip}"
SERVER_PORT = {server_port}
API_BASE = "http://{{}}:{{}}".format(SERVER_HOST, SERVER_PORT)

# ---- Device Identity ----
DEVICE_ID = {device_id}
API_KEY = "{api_key}"

# ---- Detection Loop ----
DETECTION_INTERVAL = 10

# ---- Hardware (PSoC6) ----
LED_PIN = "P13_7"
"""
    with open(config_path, "w", encoding="utf-8") as f:
        f.write(config_content)
    print(f"      Successfully updated {config_path}")

    # 6. Flash Files
    print("[5/5] Flashing files to device via mpremote...")
    app_dir = os.path.join(os.getcwd(), "micropython_application")
    
    # 6a. Flash lib folder first (if exists)
    lib_dir = os.path.join(app_dir, "lib")
    if os.path.exists(lib_dir):
        print("      Copying lib folder...")
        run_command(f"mpremote cp -r \"{lib_dir}\" \":/\"")

    # 6b. Flash other files
    files = ["config.py", "wifi.py", "api_client.py", "main.py", "boot.py"]
    
    for file in files:
        file_path = os.path.join(app_dir, file)
        if os.path.exists(file_path):
            print(f"      Copying {file}...")
            run_command(f"mpremote cp \"{file_path}\" \":{file}\"")
        else:
            print(f"      Warning: {file} not found in {app_dir}")

    print("\n==========================================")
    print("   Setup Complete! Device is Flashed.")
    print(f"   Dashboard: http://localhost:3000")
    print("==========================================")

if __name__ == "__main__":
    main()
