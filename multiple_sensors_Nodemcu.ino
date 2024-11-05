#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified();
RF24 radio(D2, D1); // CE, CSN
const byte address[6] = "00001";
const int buttonPin = D3;

const int anemometerPin = D5;
const int mq5Pin = A0;
const int waterLevelPin = D6;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.stopListening();
  
  dht.begin();
  pinMode(buttonPin, INPUT_PULLUP);

  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    while (1);
  }
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  sensors_event_t event;
  bmp.getEvent(&event);
  float pressure = event.pressure;

  int windSpeed = digitalRead(anemometerPin);

  int gasLevel = analogRead(mq5Pin);

  int waterLevel = digitalRead(waterLevelPin);

  char data[128];
  snprintf(data, sizeof(data), "T:%.1f H:%.1f P:%.1f WS:%d Gas:%d WL:%d", t, h, pressure, windSpeed, gasLevel, waterLevel);
  radio.write(data, sizeof(data));
  Serial.print("Sending: ");
  Serial.println(data);
  
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    radio.write(message.c_str(), message.length() + 1);
    Serial.print("Sending custom message: ");
    Serial.println(message);
  }

  if (digitalRead(buttonPin) == LOW) {
    radio.write("Button Pressed", sizeof("Button Pressed"));
    Serial.println("Button Pressed");
    delay(500);
  }

  delay(3000);
}
