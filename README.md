# ESP32 OPC UA Sensor Hub

## Overview
The **ESP32 OPC UA Sensor Hub** is a versatile IoT project designed for industrial applications. This project uses an ESP32 microcontroller to transmit sensor data (temperature and acceleration) via the **OPC UA protocol**, ensuring secure and scalable communication. The sensor hub integrates:

- **TMP36**: Analog temperature sensor.
- **MPU6050**: 3-axis accelerometer and gyroscope sensor.
- Built-in WiFi configuration via a WebServer.
- Persistent storage of WiFi credentials using ESP32's flash memory.

---

## Features

- **OPC UA Protocol**: Seamless integration into industrial systems.
- **WiFi Configuration**: Configure WiFi credentials dynamically through a WebServer.
- **Persistent Storage**: Store WiFi credentials in ESP32 flash memory for seamless reconnection.
- **Sensor Support**:
  - TMP36 for precise temperature measurements.
  - MPU6050 for 3-axis acceleration.
- **Scalable Design**: Easily extendable to include additional sensors.

---

## Hardware Requirements

- **ESP32 Development Board** (e.g., ESP32-WROOM or ESP32-WROVER).
- **TMP36 Analog Temperature Sensor**.
- **MPU6050 3-axis Accelerometer and Gyroscope**.
- Power Supply: 5V or a DC-DC converter for 24V systems.
- Connecting wires and a breadboard (for prototyping).

---

## Software Requirements

- **Arduino IDE** or **PlatformIO** (for Visual Studio Code).
- Libraries (installed via `platformio.ini`):
  - `WiFi`
  - `WebServer`
  - `Adafruit MPU6050`
  - `Adafruit Unified Sensor`
  - `Arduino_OPC_UA`
  - `Preferences`

---

## Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/sebastian9421/ESP32_OPCUA_SensorHub.git
cd ESP32_OPCUA_SensorHub
```

### 2. Wiring Diagram

#### TMP36 Sensor:
- **VCC**: Connect to 5V or 3.3V.
- **GND**: Connect to ground.
- **OUT**: Connect to GPIO34 (analog input).

#### MPU6050:
- **VCC**: Connect to 3.3V or 5V.
- **GND**: Connect to ground.
- **SDA**: Connect to GPIO21.
- **SCL**: Connect to GPIO22.

### 3. Configure PlatformIO

Ensure your `platformio.ini` contains:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200

lib_deps =
    WiFi
    WebServer
    Adafruit MPU6050
    Adafruit Unified Sensor
    Arduino_OPC_UA
    Preferences
```

### 4. Upload the Code

1. Connect your ESP32 to your computer via USB.
2. Open the project in PlatformIO or Arduino IDE.
3. Compile and upload the code to your ESP32.

### 5. WiFi Configuration

1. If no WiFi credentials are saved, the ESP32 starts as an Access Point (`ESP32_Config`).
2. Connect to this network and navigate to `192.168.4.1` in your browser.
3. Enter your WiFi SSID and password.

### 6. Test OPC UA Communication

1. Use an OPC UA client (e.g., [UaExpert](https://www.unified-automation.com/products/development-tools/uaexpert.html)) to connect to the ESP32.
2. Enter the IP address of the ESP32 and port `4840`.
3. Browse the available nodes to view:
   - `Temperature`
   - `Acceleration`

---

## Troubleshooting

### Common Issues:

1. **WiFi Not Connecting**:
   - Ensure SSID and password are correct.
   - Restart the ESP32 and re-enter credentials if necessary.

2. **OPC UA Client Cannot Connect**:
   - Verify the ESP32 IP address and port.
   - Check firewall or network restrictions.

3. **No Sensor Data**:
   - Ensure TMP36 and MPU6050 are wired correctly.
   - Check the serial monitor for debugging output.

---

## License

This project is licensed under the MIT License. See the LICENSE file for details.

---

## Contributions

Contributions are welcome! Feel free to fork the repository, submit pull requests, or open issues for enhancements and bug fixes.
