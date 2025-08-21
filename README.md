# Weather Monitoring System using ARM7 (LPC2148) + ESP8266 + ThingSpeak

This project demonstrates a low-cost **IoT Weather Monitoring System** using the **LPC2148 ARM7 microcontroller**, **DHT11 sensor**, and **ESP8266 Wi-Fi module**. The system captures real-time **temperature and humidity** and pushes it to the **ThingSpeak IoT cloud** via HTTP.

---

## Hardware Required

- LPC2148 ARM7 Development Board
- ESP8266 Wi-Fi Module (ESP-01)
- DHT11 Temperature & Humidity Sensor
- AMS1117 3.3V Voltage Regulator
- Power supply (5V)
- Jumper Wires / Breadboard
- USB to Serial/UART (for flashing code)

---

## Software & Tools

| Tool            | Purpose                              |
|-----------------|--------------------------------------|
| **Keil uVision5** | Embedded C development IDE          |
| **Flash Magic**  | Programmer for flashing LPC2148 code |
| **Proteus (optional)** | Circuit simulation (for testing)  |

---

## ThingSpeak Setup

1. Create a [ThingSpeak account](https://thingspeak.com).
2. Create a new channel with 2 fields:
   - Field1: Temperature
   - Field2: Humidity
3. Copy your **Write API Key**.
4. Replace it in `main.c` (`#define API_KEY`).

---

## Folder Structure

```bash
WeatherMonitoringSystem/
├── src/
│   ├── main.c
│   ├── uart0.c / uart0.h
│   ├── dht11.c / dht11.h
│   ├── delay.c / delay.h
├── README.md
└── LICENSE
