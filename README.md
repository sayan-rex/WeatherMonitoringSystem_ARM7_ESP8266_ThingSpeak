# 🌦️ Weather Monitoring System using ARM7 (LPC2148) + ESP8266 + ThingSpeak

This project demonstrates a low-cost **IoT Weather Monitoring System** using the **LPC2148 ARM7 microcontroller**, **DHT11 sensor**, and **ESP8266 Wi-Fi module**. The system captures real-time **temperature and humidity** and pushes it to the **ThingSpeak IoT cloud**.

---

## 📸 Preview

| Circuit Diagram               | ThingSpeak Dashboard                |
|------------------------------|-------------------------------------|
| ![](docs/circuit_diagram.png) | ![](docs/thingspeak_dashboard.png) |

---

## 🧰 Hardware Required

- LPC2148 ARM7 Microcontroller
- ESP8266 Wi-Fi Module (ESP-01)
- DHT11 Temperature & Humidity Sensor
- AMS1117 3.3V Regulator
- Power supply (5V)
- Jumper Wires / Breadboard

---

## 🔗 ThingSpeak Setup

1. Create a [ThingSpeak account](https://thingspeak.com).
2. Create a new channel with 2 fields:
   - Field1: Temperature
   - Field2: Humidity
3. Note the **Write API Key**.
4. Replace it in `main.c` (`#define API_KEY`).
