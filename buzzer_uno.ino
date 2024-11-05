#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 7
#define CSN_PIN 8
#define speakerOut 10
#define buttonPin 2

#define C 2100
#define D 1870
#define E 1670
#define f 1580
#define G 1400
#define R 0 // Rest note

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";
int buttonState = 0;

int melody[] = {
  E, E, E, R,
  E, E, E, R,
  E, G, C, D, E, R,
  f, f, f, f, f, E, E, E, E, D, D, E, D, R, G, R,
  E, E, E, R,
  E, E, E, R,
  E, G, C, D, E, R,
  f, f, f, f, f, E, E, E, G, G, f, D, C, R
};

int MAX_COUNT = sizeof(melody) / sizeof(melody[0]);
long tempo = 10000;
int pause = 1000;
int rest_count = 100;
int tone_ = 0;
long duration = 0;
int receivedCommand = 0;

void setup() {
  pinMode(speakerOut, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
  Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
}

void playTone() {
  long elapsed_time = 0;
  
  if (tone_ > 0) {
    while (elapsed_time < duration) {
      digitalWrite(speakerOut, HIGH);
      delayMicroseconds(tone_ / 2);
      digitalWrite(speakerOut, LOW);
      delayMicroseconds(tone_ / 2);
      elapsed_time += tone_;
    }
  } else {
    for (int j = 0; j < rest_count; j++) {
      delayMicroseconds(duration); 
    }
  }                                
}

void loop() {
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == LOW) {
    radio.stopListening();
    int command = 1;
    radio.write(&command, sizeof(command));
    delay(100);
    radio.startListening();
  }

  if (radio.available()) {
    radio.read(&receivedCommand, sizeof(receivedCommand));
    if (receivedCommand == 2) {
      for (int i = 0; i < MAX_COUNT; i++) {
        tone_ = melody[i];
        duration = 50 * tempo;
        playTone();
        delayMicroseconds(pause);
      }
      noTone(speakerOut);
    }
  }
}
