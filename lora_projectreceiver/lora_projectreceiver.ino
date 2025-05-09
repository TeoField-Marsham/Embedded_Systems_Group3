#include <SPI.h>
#include <LoRa.h>

// Pins SPI/LoRa para a LilyGO T3 V1.6.1
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18     // LoRa NSS
#define RST 14
#define DIO0 26

#define BAND 915E6  // Frequencies LoRa: use 915E6 (Americas), 866E6 (Europa), ou 433E6 (Asia)

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // SPI interface
  SPI.begin(SCK, MISO, MOSI, SS);

  // LoRa pins
  LoRa.setPins(SS, RST, DIO0);

  Serial.println("Initializing LoRa Receiver...");
  if (!LoRa.begin(BAND)) {
    Serial.println("Fail to start LoRa!");
    while (1);
  }

  Serial.println("LoRa started! Waiting for data...");
}

void loop() {
  // Verify if there is a packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Packet received: ");

    // Read packet
    while (LoRa.available()) {
      char c = LoRa.read();
      Serial.print(c);
    }

    Serial.println();
  }
}
