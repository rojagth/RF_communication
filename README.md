# RF Communication Project

This project demonstrates RF (Radio Frequency) communication between two microcontrollers: an **Arduino Uno** and a **NodeMCU**, using **nRF24L01** transceiver modules. The Arduino Uno displays messages from the NodeMCU on an LCD display and allows interaction via a button and DHT11 sensor data transfer.

## Project Overview
- **Wireless Communication**: Uses nRF24L01 modules for wireless data transmission.
- **DHT11 Sensor**: Sends temperature and humidity data from the NodeMCU to the Arduino Uno.
- **Button Control**: A button on the NodeMCU toggles an LED on the Arduino Uno.
- **Custom Messages**: Sends custom messages from the NodeMCU's Serial Monitor to be displayed on the Arduino's LCD.

## Components
- **Arduino Uno**
- **NodeMCU**
- **2 x nRF24L01 Transceiver Modules**
- **I2C LCD Display** (connected to Arduino Uno)
- **DHT11 Temperature and Humidity Sensor** (connected to NodeMCU)
- **Push Button** (connected to NodeMCU)
- **LED** (connected to Arduino Uno)

## Circuit Setup
### Arduino Uno:
1. Connect **nRF24L01 CE** to **pin 9** and **CSN** to **pin 10**.
2. Connect **LCD SDA** and **SCL** to Arduino’s **SDA** and **SCL** pins.
3. Connect **LED** to digital pin **8** with a current-limiting resistor.

### NodeMCU:
1. Connect **nRF24L01 CE** to **D2** and **CSN** to **D1**.
2. Connect **DHT11 data pin** to **D3**.
3. Connect **button** to **D4** and ground.

## Software Setup
### Prerequisites
1. Install the following libraries in the Arduino IDE:
   - [RF24 Library](https://github.com/nRF24/RF24)
   - [LiquidCrystal I2C Library](https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library)
   - [DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library)

### Cloning the Repository
```bash
git clone https://github.com/your-username/RF_communication.git
