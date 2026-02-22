# Reproduction Guide: Edge AI Inventory System

This guide explains how to set up and run the complete system, from the Flask backend to the MicroPython edge device.

## Prerequisites

Before starting, ensure you have the following installed:
*   **Python 3.10+** (for Backend and Setup script)
*   **Node.js 18+** (for Next.js Frontend)
*   **mpremote** (`pip install mpremote`)
*   **PSoC6 Device** connected via USB and flashed mpy interpreter

## Quick Start (Recommended)

The easiest way to reproduce the project is using the automated setup script.

1.  **Clone the Repository**:
    ```bash
    git clone https://github.com/CuzImHarry/micropython-hands-on-course.git
    cd micropython-hands-on-course/project
    ```

2.  **Run the Setup Script**:
    ```bash
    python full_setup.py
    ```
    *   **Step 1**: Provide your WLAN SSID and Password when prompted.
    *   **Step 2**: Enter a name and location for your device.
    *   **Step 3**: The script will automatically start the servers, register your device, update the configuration, and flash all files (including libraries) to your PSoC6.

3.  **Access the Dashboard**:
    Open [http://localhost:3000](http://localhost:3000) in your browser to see the live inventory and device status.

## Manual Reproduction

If you prefer to set up the components individually:

### 1. Backend & Frontend
```bash
cd Serverapp
./setup_and_run.bat
```
*   This will create a virtual environment, install Python/Node dependencies, and launch both servers in separate windows.

### 2. MicroPython Device
1.  Navigate to `micropython_application/`.
2.  Update `config.py` with your WiFi credentials and the Server IP.
3.  Register your device via the API (`POST /api/devices`) to get a `DEVICE_ID` and `API_KEY`.
4.  Flash the files:
    ```bash
    mpremote cp -r lib/ :/
    mpremote cp *.py :
    ```

## Project Structure
*   `Serverapp/`: Flask API and Next.js Dashboard.
*   `micropython_application/`: MicroPython code and libraries for PSoC6.
*   `full_setup.py`: The main automation and deployment script.

---
> [!NOTE]
> Ensure your PSoC6 is running a compatible DeepCraft AI model for object classification to see active detection events in the dashboard.
