# ESP32 OPC UA Sensor Hub

## Projektbeschreibung
Dieses Projekt verwendet einen ESP32, um Sensordaten über OPC UA verfügbar zu machen. Der Sensor-Hub unterstützt:

- **Temperatursensor (TMP36)**: Misst die Umgebungstemperatur in Grad Celsius.
- **Beschleunigungssensor (MPU6050)**: Misst die Beschleunigung in den Achsen X, Y und Z in mm/s².

Die Sensordaten werden über einen OPC UA-Server zur Verfügung gestellt, der auf dem ESP32 läuft. Nutzer können sich über einen OPC UA-Client mit dem Server verbinden und die Daten auslesen.

## Funktionen
- **OPC UA-Server**: Bereitstellung der Sensordaten.
- **WiFi-Konfiguration über Webserver**: Einfaches Konfigurieren der WLAN-Verbindung ohne Code-Änderungen.

## Hardwareanforderungen
- ESP32-Board
- TMP36-Temperatursensor
- MPU6050-Beschleunigungssensor
- 24V-Stromversorgung mit Spannungswandler auf 3.3V/5V
- Verbindungsdrähte

## Softwareanforderungen
- [PlatformIO](https://platformio.org/) oder Arduino IDE
- Installierte Bibliotheken:
  - `Arduino_OPC_UA`
  - `Adafruit_MPU6050`
  - `Adafruit_Sensor`

## Verkabelung
- **TMP36**:
  - VCC: 3.3V
  - GND: GND
  - Signal: GPIO34 (A0)
- **MPU6050**:
  - VCC: 3.3V
  - GND: GND
  - SDA: GPIO21
  - SCL: GPIO22

## OPC UA-Variablen

| Variable         | Node ID             | Typ       | Beschreibung                     |
|------------------|---------------------|-----------|----------------------------------|
| Temperature      | `1:Temperature`    | `Float`   | Umgebungstemperatur in Grad Celsius |
| AccelerationX    | `1:AccelerationX`  | `Float`   | Beschleunigung in X-Richtung (mm/s²) |
| AccelerationY    | `1:AccelerationY`  | `Float`   | Beschleunigung in Y-Richtung (mm/s²) |
| AccelerationZ    | `1:AccelerationZ`  | `Float`   | Beschleunigung in Z-Richtung (mm/s²) |

## Nutzung
### 1. Projekt auf den ESP32 hochladen
- Lade das Projekt mit PlatformIO oder der Arduino IDE auf den ESP32.

### 2. WiFi konfigurieren
- Starte den ESP32.
- Verbinde dich mit dem Access Point `ESP32_Config`.
- Öffne einen Browser und gehe zu `192.168.4.1`.
- Gib SSID und Passwort deines Netzwerks ein.

### 3. Mit OPC UA-Client verbinden
- Starte einen OPC UA-Client (z. B. UaExpert).
- Verbinde dich mit dem Server: `opc.tcp://<ESP32-IP>:4840`
- Greife auf die Variablen zu.

## Lizenz
Dieses Projekt steht unter der MIT-Lizenz. Siehe [LICENSE](LICENSE) für weitere Informationen.

---
Falls du Fragen oder Probleme hast, melde dich gerne!

