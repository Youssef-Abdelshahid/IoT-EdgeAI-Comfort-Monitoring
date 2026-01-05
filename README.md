# IoT Edge AI Comfort Monitoring

This project is an **IoT-based temperature and humidity monitoring system** with **edge AI** for comfort classification. It uses an **ESP32** microcontroller and a **DHT22 sensor** to measure indoor environmental conditions and automatically classify them as **Comfortable, Uncomfortable, or Warm**. Data is transmitted to a **ThingSpeak dashboard** for real-time visualization.

---

## Features

- Measures **temperature** and **humidity** in real-time  
- Performs **AI inference on the ESP32** (Edge AI)  
- Classifies comfort levels automatically  
- Uploads data and predictions to **ThingSpeak** for cloud visualization  
- Works independently of continuous internet access  

---

## Hardware

- **ESP32 microcontroller**  
- **DHT22 temperature and humidity sensor**  

---

## Software

- **Arduino IDE**  
- **Adafruit DHT library**  
- Embedded ML model for comfort classification (.h file)  
- ThingSpeak cloud platform for visualization  

---

## How It Works

1. ESP32 reads temperature and humidity from DHT22  
2. Embedded ML model classifies comfort level  
3. ESP32 sends data and AI predictions to ThingSpeak  
4. ThingSpeak dashboard updates in real-time  

---

## Applications

- Smart home and office climate monitoring  
- Greenhouse and agriculture monitoring  
- Server room environment tracking  
- Classroom comfort optimization  

---

## License

This project is for educational and personal use.  
