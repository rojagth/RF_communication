#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define CE_PIN D2
#define CSN_PIN D1
#define servoPin D3

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

Servo myServo;
int receivedCommand = 0;

void setup() {
  Serial.begin(9600);
  
  myServo.attach(servoPin); // Attach servo to designated pin
  myServo.write(0); // Initial position
  
  radio.begin();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    radio.read(&receivedCommand, sizeof(receivedCommand));
    if (receivedCommand == 1) { // Command from Arduino to move the servo
      myServo.write(90); // Move servo to 90 degrees
      delay(1000); // Hold position for 1 second
      myServo.write(0); // Return to original position

      // Send a command back to Arduino to play melody
      radio.stopListening();
      int melodyCommand = 2; // Signal to play melody on Arduino
      radio.write(&melodyCommand, sizeof(melodyCommand));
      delay(100);
      radio.startListening();
    }
  }
}
