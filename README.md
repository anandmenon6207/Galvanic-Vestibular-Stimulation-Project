# 🧠 Wireless GVS Simulation System

A wireless control system that demonstrates the principles of galvanic vestibular stimulation (GVS) using an ESP32, op-amp circuitry, and real-time input control.


## 🚀 Overview

This project implements a real-time system that converts user input into a controlled analog signal, which is then used to generate bidirectional current through an op-amp circuit, which furthur allows us to demonstrate the effects of Galvanic Vestibular Stimulation.

### Biological Aspect of GVS

GVS involves applying a small electrical signal near the mastoid areas behind the ears, where the vestibular nerves are located. These nerves carry balance and orientation information from the inner ear to the brain. The electrical signal changes the firing rates of the nerve endings—making one side more active and the other less active. This creates a mismatch in the signals sent to the brain, which it interprets as movement or a change in direction. As a result, the body may feel like it is tilting or shifting, showing how electrical stimulation can influence the balance system.


---
## 🧠 Working Principle

The ESP32 receives control input over WiFi and converts it into an analog voltage using its built-in DAC (GPIO 25).

A reference voltage of 1.65V is used as a virtual ground, representing the neutral state of the system. The op-amp (LM358), configured with negative feedback, continuously adjusts its output to keep its inputs balanced. In doing so, it converts the difference between the input voltage and the reference into a controlled current through the circuit.

A dual supply of ±9V is used to power the op-amp, providing sufficient headroom for the output to swing both above and below the reference level. This allows current to flow in both directions, which is essential for bidirectional operation.

During testing, the direction of this current determines the output:
- One LED lights up when the current flows in one direction 
- The other LED lights up when the current reverses
  
This behavior demonstrates controlled bidirectional current flow, analogous to the operation of a GVS system.

### Current Limits

The current through the LEDs is determined by the voltage difference between the DAC output and the 1.65V reference, divided by the 500Ω resistor:

I = (V_in − 1.65) / 500

Since the ESP32 DAC ranges from 0V to 3.3V, the maximum deviation from the reference is ±1.65V. This limits the current to approximately:

- **Maximum current** ≈ +3.3 mA  
- **Minimum current** ≈ −3.3 mA  

The current is naturally limited by the 500Ω resistor, which prevents excessive current flow through the GVS. Additionally, the op-amp’s feedback mechanism ensures controlled operation, so the system cannot exceed these bounds under normal conditions.


---

## ⚙️ System Architecture
<img width="736" height="338" alt="image" src="https://github.com/user-attachments/assets/fa1297a3-8097-4f85-8bf5-e6174342188a" />


Web UI → Python/Flask (UDP) → WiFi → ESP32 → DAC → Op-Amp → LEDs


- **Input Layer**: Web-based controller UI (served locally)
- **Communication Layer**: UDP over WiFi
- **Embedded System**: ESP32
- **Analog Processing**: Op-amp (LM358)
- **Output Layer**: Bidirectional current


## 🚦 Deployment & Setup

### 1. Hardware & Firmware (ESP32)
- Open `src/receiver.ino` and update the WiFi `ssid` and `password` variables
- Flash to the ESP32 and open Serial Monitor at `115200` baud to retrieve the local IP address
- Ensure your GVS electrodes are connected to the Op-Amp output and the 1.65V reference ground

### 2. Python Command Bridge
- Update the `ESP32_IP` variable inside `brain.py` to match the IP from the Serial Monitor
- Run `brain.py` to start the bridge server

> **Note:** Requires Flask, flask-cors, and pyautogui. Install via `pip install flask flask-cors pyautogui`

### 3. Web Interface
- Update the `BRIDGE_IP` variable inside `index.html` to match the local IP of the machine running `brain.py`
- Open `index.html` in a browser on the same Wi-Fi network
  
## 🔌 Hardware Components

- ESP32
- LM358 Op-Amp
- Resistors (500Ω)
- LEDs (testing purpouses)
- Power Supply (±9V)
- Breadboard & wiring
- ECG patches
- Conductive Gel




## 💻 Software Components
### 🔹 Web Interface
- Browser-based controller for real-time input
- Connects to the Python bridge over local network

### 🔹 Python Command Bridge
- Receives HTTP requests from the Web UI
- Forwards commands as low-latency UDP packets to the ESP32

### 🔹 ESP32 Firmware
- Receives UDP data
- Outputs analog signal via DAC


## 📸 Demo

YOUTUBE: https://www.youtube.com/watch?v=t6M_l-YdFlc


## Key Concepts

- Wireless embedded systems
- UDP communication
- Digital-to-Analog Conversion (DAC)
- Op-amp negative feedback
- Bidirectional current control

---

## ⚠️ Disclaimer

This project is a **simulation and demonstration system** intended for educational purposes.

It is **not a medical device**.  
Any real-world application involving human subjects must be conducted under proper supervision and safety protocols must be strictly adhered to when interfacing with biological systems.




