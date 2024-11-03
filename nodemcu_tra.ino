#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>

#define DHTPIN D4      // Pin where the DHT11 is connected
#define DHTTYPE DHT11  // Define the DHT type
DHT dht(DHTPIN, DHTTYPE); // Create DHT instance

RF24 radio(D2, D1); // CE, CSN
const byte address[6] = "00001";
const int buttonPin = D3; // Button pin

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.stopListening();
  
  dht.begin(); // Initialize the DHT sensor
  pinMode(buttonPin, INPUT_PULLUP); // Set button pin as input with pullup
}

void loop() {
  // Read temperature and humidity
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius
  
  // Send temperature and humidity data
  char data[32];
  snprintf(data, sizeof(data), "T:%.1f H:%.1f", t, h);
  radio.write(data, sizeof(data));
  Serial.print("Sending: ");
  Serial.println(data);
  
  // Check for incoming data from Serial Monitor
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n'); // Read until newline
    radio.write(message.c_str(), message.length() + 1); // Send custom message
    Serial.print("Sending custom message: ");
    Serial.println(message);
  }

  // Check button press
  if (digitalRead(buttonPin) == LOW) { // Button pressed
    radio.write("Button Pressed", sizeof("Button Pressed"));
    Serial.println("Button Pressed");
    delay(500); // Debounce delay
  }

  delay(1000); // Send data every 5 seconds
}
