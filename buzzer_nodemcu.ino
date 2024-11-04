#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN D2
#define CSN_PIN D1

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";
int command = 1;  // Simple command to play sound

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  pinMode(D3, INPUT_PULLUP);  // Button to trigger sound
}

void loop() {
  if (digitalRead(D3) == LOW) {  // Button pressed
    radio.write(&command, sizeof(command));
    delay(500);  // Debounce
  }
}
