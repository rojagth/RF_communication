#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(D2, D1);  // CE, CSN pins on NodeMCU
const byte address[6] = "00001";  // Unique address for communication

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);   // Start with low power
  radio.setDataRate(RF24_1MBPS);   // Set data rate to 1Mbps for a balanced test
  radio.stopListening();           // Set as transmitter
}

void loop() {
  const char text[] = "Test Message";
  bool success = radio.write(&text, sizeof(text));
  if (success) {
    Serial.println("Message sent successfully");
  } else {
    Serial.println("Message failed to send");
  }
  delay(1000);  // Send message every second
}
