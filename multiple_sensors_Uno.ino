#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
LiquidCrystal_I2C lcd(0x27, 20, 4);
const int ledPin = 7;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  
  pinMode(ledPin, OUTPUT);
  radio.begin();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[128] = "";
    radio.read(&text, sizeof(text));
    
    Serial.print("Received: ");
    Serial.println(text);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(text);
    
    if (strcmp(text, "Button Pressed") == 0) {
      digitalWrite(ledPin, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("LED ON   ");
    } else {
      digitalWrite(ledPin, LOW);
      lcd.setCursor(0, 1);
      lcd.print("Waiting for data   ");
    }
  }
}
