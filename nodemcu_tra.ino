#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>

#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

RF24 radio(D2, D1); // CE, CSN
const byte address[6] = "00001";
const int buttonPin = D3;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.stopListening();
  
  dht.begin();
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  char data[32];
  snprintf(data, sizeof(data), "T:%.1f H:%.1f", t, h);
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

  delay(1000);
}
