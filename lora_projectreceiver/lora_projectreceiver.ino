#include <SPI.h>
#include <LoRa.h>

// SPI/LoRa pins for LilyGO T3 V1.6.1
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18     // LoRa NSS
#define RST 14
#define DIO0 26

#define BAND 866E6  // LoRa: use 915E6 (Americas), 866E6 (Europe), ou 433E6 (Asia)

int mailCount = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Configure LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);

  Serial.println("Starting LoRa Receiver...");
  if (!LoRa.begin(BAND)) {
    Serial.println("Fail to start LoRa!");
    while (1);
  }

  Serial.println("LoRa successfully initiated! Waiting for packages...");
}

void loop() {
  // Look out for packages
  int packetSize = LoRa.parsePacket();
  if (packetSize > 0) {
    String message = "";

    while (LoRa.available()) {
      message += (char)LoRa.read();
    }

    message.trim(); // Remove any trailing newlines or spaces
  
    Serial.print("Mail: ");
    if (message=="reset"){mailCount = 0;} else if (message=="newmail"){
      mailCount++;
    }
      Serial.println(mailCount);
    }
}
