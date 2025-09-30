# 🌤️ ESP32 Weather Dashboard

This project uses an ESP32 to monitor temperature, humidity, and light levels, and displays the data on a web dashboard. It also infers weather conditions and lights up LEDs accordingly.

## 🔧 Hardware Used
- ESP32 Dev Module
- DHT11 Sensor
- Photoresistor
- RGB LEDs

## 🌐 Features
- Web dashboard with live sensor data
- Date & time via NTP
- Weather condition inference
- LED indicators (Red = Sunny, Green = Cloudy, Blue = Rainy)

## 📁 File Structure
- `main.ino`: Arduino sketch
- `data/index.html`: Web dashboard template
