# ============================================================
# Edge AI Inventory — Device Configuration (AUTO-GENERATED)
# ============================================================

# ---- Wi-Fi ----
WIFI_SSID = "SSID"
WIFI_PASS = "PASSWORD"
WIFI_TIMEOUT = 15  # seconds to wait for connection

# ---- Server ----
SERVER_HOST = "192.168.0.129"
SERVER_PORT = 5000
API_BASE = "http://{}:{}".format(SERVER_HOST, SERVER_PORT)

# ---- Device Identity ----
DEVICE_ID = 4
API_KEY = "your Api-key"

# ---- Detection Loop ----
DETECTION_INTERVAL = 10

# ---- Hardware (PSoC6) ----
LED_PIN = "P13_7"
