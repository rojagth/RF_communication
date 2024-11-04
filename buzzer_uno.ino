#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10
#define speakerOut 4 // Buzzer pin

// Define note frequencies
#define C 2100
#define D 1870
#define E 1670
#define f 1580 // lowercase f
#define G 1400
#define R 0 // Rest note

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";
int receivedCommand = 0;

// MELODY and TIMING
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

int MAX_COUNT = sizeof(melody) / sizeof(melody[0]); // Melody length
long tempo = 10000; // Set overall tempo
int pause = 1000; // Set length of pause between notes
int rest_count = 100; // Length for rest
int tone_ = 0;
long duration = 0;

void setup() {
  pinMode(speakerOut, OUTPUT);
  Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
}

void playTone() {
  long elapsed_time = 0;
  
  if (tone_ > 0) { // If not a rest beat
    while (elapsed_time < duration) {
      digitalWrite(speakerOut, HIGH);
      delayMicroseconds(tone_ / 2);
      digitalWrite(speakerOut, LOW);
      delayMicroseconds(tone_ / 2);
      elapsed_time += tone_;
    }
  } else { // Rest beat
    for (int j = 0; j < rest_count; j++) {
      delayMicroseconds(duration); 
    }
  }                                
}

void loop() {
  if (radio.available()) {
    radio.read(&receivedCommand, sizeof(receivedCommand));
    if (receivedCommand == 1) { // Command to play the melody
      for (int i = 0; i < MAX_COUNT; i++) {
        tone_ = melody[i];
        duration = 50 * tempo; // Set up timing
        playTone();
        delayMicroseconds(pause); // Pause between notes
      }
      noTone(speakerOut); // Ensure the buzzer is off after the song finishes
    }
  }
}
