<p align="center">
  <img src="./img/micropython_logo.png" alt="MicroPython Logo" height="50"/>
  &nbsp;&nbsp;&nbsp;&nbsp;
  <img src="./img/imagimob_logo.png" alt="Imagimob Logo" height="50"/>
  &nbsp;&nbsp;&nbsp;&nbsp;
  <img src="./img/infineon_logo.png" alt="Infineon Logo" height="50"/>
</p>

# Edge AI Radar Detection
### MicroPython & Edge AI Hands-On Course

Welcome to the **Edge AI Inventory System**, a comprehensive project developed for the MicroPython & Edge AI Hands-On Course. This system demonstrates the power of bringing machine learning to the edge by combining high-performance Infineon hardware with MicroPython and TinyML models.

The project features a real-time monitoring system that uses radar and IMU sensors to classify objects and track inventory, reporting data back to a modern web dashboard.

---

## Key Features

-   **Real-time Object Classification**: Uses DeepCraft AI models on the PSoC6 to identify objects (e.g., drill detection).
-   **Edge-to-Cloud Connectivity**: WiFi-enabled reporting of detection events to a Flask-based backend.
-   **Live Dashboard**: A Next.js frontend for real-time inventory tracking and device management.
-   **Automated Deployment**: A single Python script handles server startup, device registration, configuration, and flashing.
-   **Low Latency**: Local inference on the device ensures immediate response to events.

---

## Hardware Requirements

This project is built around the **Infineon CY8CKIT-062S2-AI** board, featuring:

*   **MCU**: PSoC™ 6 with dual-core architecture.
*   **Sensors**:
    *   60 GHz Radar (BGT60TR13C) for presence and motion detection.
    *   6-axis IMU (BMI270) for motion and vibration analysis.
    *   Barometric Pressure Sensor (DPS368).
    *   MEMS Microphone (IM69D130).
*   **Connectivity**: CYW43439 Wi-Fi® + Bluetooth® Module.

---

## Software Stack

-   **Edge (Firmware)**: [MicroPython](https://www.micropython.org/) — for rapid prototyping and easy integration.
-   **Machine Learning**: [Imagimob DEEPCRAFT™](https://developer.imagimob.com/) — for training and deploying TinyML models.
-   **Backend**: [Flask (Python)](https://flask.palletsprojects.com/) — RESTful API for device and inventory management.
-   **Frontend**: [Next.js (React)](https://nextjs.org/) — Interactive dashboard for data visualization.
-   **Tools**: `mpremote` for device management, Thonny IDE.

---

## How to Reproduce the Project

The project is designed for easy reproduction using an automated setup script.

### 1. Prerequisites
Ensure you have the following installed on your machine:
*   **Python 3.10+**
*   **Node.js 18+**
*   **mpremote**: `pip install mpremote requests`
*   **PSoC6 Device**: Connected via USB and flashed with the Infineon MicroPython interpreter.

### 2. Quick Start (Automated)
The easiest way to get the entire system running:

1.  **Clone the Repository**:
    ```bash
    git clone https://github.com/CuzImHarry/micropython-hands-on-course.git
    cd micropython-hands-on-course/project
    ```

2.  **Run the Setup Tool**:
    ```bash
    python full_setup.py
    ```
    *   Enter your **WiFi Credentials** (SSID/Password) when prompted.
    *   Provide a **Device Name** and **Location**.
    *   The script will:
        *   Start the Flask Backend and Next.js Frontend.
        *   Automatically register your device with the server.
        *   Generate a `config.py` with your unique API Key and Server IP.
        *   Flash all necessary MicroPython files and libraries to the PSoC6.

### 3. Access the System
Once the script completes, open your browser to:
**[http://localhost:3000](http://localhost:3000)**

You will see the live dashboard where your PSoC6 device will appear and start reporting detection events.

---

## Repository Structure

*   [`./project/Serverapp/`](./project/Serverapp/): The backend API and frontend dashboard code.
*   [`./project/micropython_application/`](./project/micropython_application/): Core application logic and hardware drivers for the PSoC6.
*   [`./project/deepcraft_workspace/`](./project/deepcraft_workspace/): DEEPCRAFT™ Studio project files, data, and models.
*   [`./examples/`](./examples/): Hands-on tutorials for basic hardware control (LEDs, Buttons, Sensors).
*   [`./report/`](./report/): Project documentation and final presentation assets.

---

## Resources & Documentation

### Onboarding Material
* [Prerequisites](./prerequisites.pdf)
* [Installation instructions](https://ifx-micropython.readthedocs.io/en/latest/psoc6/installation.html)
* [MicroPython Introduction](./micropython-ai-edge-wshop-tuw_2026-02-09.pdf)
* [Code Examples](./examples/)

### Hardware Product Pages and Docs

#### Infineon CY8CKIT-062S2-AI Board
* [PSOC™ 6 CY8CKIT-062S2-AI Board manual](https://www.infineon.com/assets/row/public/documents/30/44/infineon-cy8ckit-062s2-user-guide-usermanual-en.pdf)
* [PSOC™ 6 CY8CKIT-062S2-AI Board Product Page](https://www.infineon.com/assets/row/public/documents/30/44/infineon-cy8ckit-062s2-user-guide-usermanual-en.pdf)
* [CY8C624ABZI-S2D44 Datasheet](https://www.infineon.com/assets/row/public/documents/30/49/infineon-psoc-6-mcu-cy8c62x8-cy8c62xa-datasheet-datasheet-en.pdf)
* [CYW43439 Wi-Fi® + Bluetooth® Module](https://www.murata.com/products/connectivitymodule/wi-fi-bluetooth/overview/lineup/type1yn)

#### On-Board Peripherals
* [Inertial Measurement Unit (IMU) BMI270](https://www.bosch-sensortec.com/products/motion-sensors/imus/bmi270/)
* [Magnetometer BMM350](https://www.bosch-sensortec.com/products/motion-sensors/magnetometers/bmm350/)
* [Barometric Pressure Sensor DPS368](https://www.infineon.com/part/DPS368)
* [60 GHz Radar Sensor BGT60TR13C](https://www.infineon.com/part/BGT60TR13C)
* [MEMS Digital Microphone IM69D130](https://www.infineon.com/part/IM69D130)

### Related Repositories
* [MicroPython PSOC™ 6 Fork](https://github.com/Infineon/micropython)
* [MicroPython Radar Sensor Driver](https://github.com/Infineon/micropython-radar-bgt60)
* [MicroPython Pressure Sensor Driver](https://github.com/Infineon/micropython-xensiv-dps3xx)
* [MicroPython IMU Driver](https://github.com/ederjc/micropython_bmi270)
* [DEEPCRAFT™ MicroPython Model Converter](https://github.com/Infineon/deepcraft-micropython-converter)
* [Official MicroPython Libraries](https://github.com/micropython/micropython-lib)

### MicroPython Docs
* [MicroPython Official Site](https://www.micropython.org/)
* [MicroPython PSOC6 Documentation](https://ifx-micropython.readthedocs.io/en/latest/)

### Forums and Support
* [MicroPython Discord](https://micropython.org/discord)
* [MicroPython Infineon GitHub Issues](https://github.com/Infineon/micropython/issues)
* [MicroPython Infineon GitHub Discussions](https://github.com/Infineon/micropython/discussions)
* [MicroPython Official GitHub Issues](https://github.com/micropython/micropython/issues)
* [MicroPython Official GitHub Discussions](https://github.com/micropython/micropython/discussions)

### Community Projects
* [Hackster.io](https://www.hackster.io/)
* [Hackster.io Infineon Team](https://www.hackster.io/Infineon_Team)

#### Related Hackster.io Articles
* [Protip on PDM Microphones](https://www.hackster.io/Infineon_Team/silence-meet-signal-a-beginner-s-guide-to-pdm-microphones-dd4667)
* [Protip on Radar Sensor](https://www.hackster.io/537973/explore-the-environment-using-bgt60-radar-and-micropython-c67d6e)
* [Protip on Accelerometer & Gyroscope](https://www.hackster.io/Infineon_Team/accelerometer-gyroscope-with-psoc-6-and-micropython-757eb3)
* [Integration of DEEPCRAFT™ model into MicroPython](https://www.hackster.io/Infineon_Team/edge-ai-made-easy-with-deepcraft-micropython-part-ii-fa5edf)

### Tools
* [Python](https://www.python.org/)
* [Pip](https://pip.pypa.io/en/stable/)
* [Thonny](https://thonny.org/)
* [DEEPCRAFT™ Studio docs](https://developer.imagimob.com/)

---

## The Team

<table align="center">
  <tr>
    <td align="center">
      <br />
      <b>Harald</b><br />
      Embedded Architect (Infineon)
    </td>
    <td align="center">
      <br />
      <b>Manuel</b><br />
      Application Engineer (Imagimob)
    </td>
    <td align="center">
      <br />
      <b>Moritz</b><br />
      Application Engineer (Imagimob)
    </td>
  </tr>
</table>

**Support**: Please [open an issue](https://github.com/Infineon/micropython-hands-on-course/issues) or contact us directly if you have any questions!

---
*Developed as part of the Edge AI University Program.*
