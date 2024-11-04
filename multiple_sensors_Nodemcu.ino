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

// Create an instance of the BMP180 sensor
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified();
RF24 radio(D2, D1); // CE, CSN
const byte address[6] = "00001";
const int buttonPin = D3;

// Replace these with your anemometer and MQ5 pin definitions
const int anemometerPin = D5; // Digital pin for anemometer
const int mq5Pin = A0; // Analog pin for MQ5 sensor
const int waterLevelPin = D6; // Digital or analog pin for water level sensor

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.stopListening();
  
  dht.begin();
  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize BMP180
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    while (1);
  }
}

void loop() {
  // Read temperature and humidity from DHT11
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Read pressure and altitude from BMP180
  sensors_event_t event;
  bmp.getEvent(&event);
  float pressure = event.pressure; // in hPa

  // Read anemometer value (this is a placeholder, implement according to your sensor)
  int windSpeed = digitalRead(anemometerPin); // This will depend on your specific anemometer implementation

  // Read MQ5 sensor value
  int gasLevel = analogRead(mq5Pin);

  // Read water level sensor value (adjust based on your sensor type)
  int waterLevel = digitalRead(waterLevelPin); // or analogRead

  // Prepare data string
  char data[128];
  snprintf(data, sizeof(data), "T:%.1f H:%.1f P:%.1f WS:%d Gas:%d WL:%d", t, h, pressure, windSpeed, gasLevel, waterLevel);
  radio.write(data, sizeof(data));
  Serial.print("Sending: ");
  Serial.println(data);
  
  // Custom message from Serial Monitor
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    radio.write(message.c_str(), message.length() + 1);
    Serial.print("Sending custom message: ");
    Serial.println(message);
  }

  // Check for button press
  if (digitalRead(buttonPin) == LOW) {
    radio.write("Button Pressed", sizeof("Button Pressed"));
    Serial.println("Button Pressed");
    delay(500);
  }

  delay(3000); // Adjust delay as necessary
}
