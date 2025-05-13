#include <SPI.h>
#include <LoRa.h>

// SPI/LoRa pins for LilyGO T3 V1.6.1
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

#define RESET_PIN 22
#define MOTION_PIN 19

#define BAND 915E6  // LoRa: use 915E6 (Americas), 866E6 (Europe), ou 433E6 (Asia)

bool resetPressed = false;
bool motionDetected = false;

int lastResetTime = 0;
int lastMotionTime = 0;

void IRAM_ATTR handleReset() {
  unsigned long now = millis();
  if (now - lastResetTime > 3000) {
    resetPressed = true;
    lastResetTime = now;
  }
}

void IRAM_ATTR handleMotion() {
  unsigned long now = millis();
  if (now - lastMotionTime > 5000) {
    motionDetected = true;
    lastMotionTime = now;
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Configure LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Fail to start LoRa!");
    while (1);
  }

  Serial.println("LoRa successfully initiated!");

  attachInterrupt(digitalPinToInterrupt(RESET_PIN), handleReset, FALLING);
  attachInterrupt(digitalPinToInterrupt(MOTION_PIN), handleMotion, RISING);
}

void loop() {

  if (resetPressed){
    Serial.println("Sending: reset");
    LoRa.beginPacket();
    LoRa.print("reset");
    LoRa.endPacket();

    resetPressed = false;

    delay(5000);  // Interval between resets
  } else if (motionDetected){
    delay(5000); //waits 5 seconds to see if the reset button won't be pressed
    if (!resetPressed){
      Serial.println("Sending: newmail");
      LoRa.beginPacket();
      LoRa.print("newmail");
      LoRa.endPacket();

      motionDetected = false;

      delay(5000);  // Interval
    }
  }

  delay(10);
}