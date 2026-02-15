import json
import urllib.request
import urllib.error
import sys
import time

BASE_URL = "http://127.0.0.1:5000/api"

def request(method, endpoint, data=None, headers=None):
    url = f"{BASE_URL}{endpoint}"
    if headers is None:
        headers = {}
    
    headers["Content-Type"] = "application/json"
    
    encoded_data = None
    if data:
        encoded_data = json.dumps(data).encode("utf-8")
    
    req = urllib.request.Request(url, data=encoded_data, headers=headers, method=method)
    
    try:
        with urllib.request.urlopen(req) as response:
            status = response.getcode()
            body = response.read().decode("utf-8")
            try:
                json_body = json.loads(body)
            except:
                json_body = body
            return status, json_body
    except urllib.error.HTTPError as e:
        body = e.read().decode("utf-8")
        try:
            json_body = json.loads(body)
        except:
            json_body = body
        return e.code, json_body
    except urllib.error.URLError as e:
        print(f"Failed to connect to {url}: {e}")
        return 0, None

def run_tests():
    print("=== Starting API Tests ===")
    
    # 1. Health Check
    print("\n[1] Testing Health Endpoint...")
    status, body = request("GET", "/health")
    if status == 200:
        print("[PASS] Health check passed")
    else:
        print(f"[FAIL] Health check failed: {status} {body}")
        return

    # 2. Register Device
    print("\n[2] Registering a new device...")
    device_name = f"test-device-{int(time.time())}"
    payload = {"name": device_name, "location": "Test Lab"}
    status, device = request("POST", "/devices", payload)
    
    if status == 201:
        print(f"[PASS] Device registered: ID={device['id']}, API_KEY={device['api_key']}")
        device_id = device['id']
        api_key = device['api_key']
    else:
        print(f"[FAIL] Device registration failed: {status} {device}")
        return

    # 3. List Devices
    print("\n[3] Listing devices...")
    status, devices = request("GET", "/devices")
    if status == 200 and any(d['id'] == device_id for d in devices):
        print(f"[PASS] Device list contains new device.")
    else:
        print(f"[FAIL] Device list verification failed. {status}")

    # 4. Post Detection
    print("\n[4] Posting detection...")
    det_payload = {
        "device_id": device_id,
        "label": "test_object",
        "confidence": 0.98,
        "status": "detected"
    }
    headers = {"X-API-Key": api_key}
    status, det_resp = request("POST", "/detections", det_payload, headers=headers)
    if status == 201:
        print("[PASS] Detection posted successfully.")
    else:
        print(f"[FAIL] Detection post failed: {status} {det_resp}")

    # 5. Check Inventory
    print("\n[5] Checking inventory...")
    status, inventory = request("GET", "/inventory")
    if status == 200:
        found = False
        for item in inventory:
            if item['device_id'] == device_id and item['label'] == "test_object":
                found = True
                print(f"   Found item: {item['label']} (Status: {item['status']})")
                break
        if found:
            print("[PASS] Inventory updated correctly.")
        else:
            print("[FAIL] Created item not found in inventory.")
    else:
        print(f"[FAIL] Inventory check failed: {status}")

    # 6. Check Stats
    print("\n[6] Checking stats...")
    status, stats = request("GET", "/inventory/stats")
    if status == 200:
        print(f"   Total items: {stats.get('total_items')}")
        print(f"   Total detections: {stats.get('total_detections')}")
        print("[PASS] Stats endpoint working.")
    else:
        print(f"[FAIL] Stats check failed: {status}")

    # 7. Clean up (Delete Device)
    print("\n[7] Cleaning up (Deleting Device)...")
    status, _ = request("DELETE", f"/devices/{device_id}")
    if status == 200:
        print("[PASS] Device deleted.")
    else:
        print(f"[FAIL] Device deletion failed: {status}")

    print("\n=== Tests Completed ===")

if __name__ == "__main__":
    run_tests()
