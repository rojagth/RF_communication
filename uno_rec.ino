#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
LiquidCrystal_I2C lcd(0x27, 20, 4); // Adjust the address if necessary
const int ledPin = 7; // Pin for LED

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  
  pinMode(ledPin, OUTPUT); // Set LED pin as output
  radio.begin();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    
    // Display on Serial Monitor
    Serial.print("Received: ");
    Serial.println(text);
    
    // Display message on LCD
    lcd.clear();
    lcd.setCursor(0, 0); // First row for custom message or DHT data
    lcd.print(text);
    
    // Control LED based on received message
    if (strcmp(text, "Button Pressed") == 0) {
      digitalWrite(ledPin, HIGH); // Turn on LED
      lcd.setCursor(0, 1);
      lcd.print("LED ON   "); // Indicate LED status on LCD
    } else {
      digitalWrite(ledPin, LOW); // Turn off LED
      lcd.setCursor(0, 1);
      lcd.print("Waiting for data   "); // Indicate waiting status
    }
  }
}
