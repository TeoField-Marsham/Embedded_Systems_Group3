#include <SPI.h>
#include <LoRa.h>

// Pinos SPI/LoRa para a LilyGO T3 V1.6.1
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18     // LoRa NSS
#define RST 14
#define DIO0 26

#define BAND 915E6  // Frequência LoRa: use 915E6 (Américas), 866E6 (Europa), ou 433E6 (Ásia)

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Inicializa a interface SPI
  SPI.begin(SCK, MISO, MOSI, SS);

  // Configura os pinos do LoRa
  LoRa.setPins(SS, RST, DIO0);

  Serial.println("Inicializando LoRa Receiver...");
  if (!LoRa.begin(BAND)) {
    Serial.println("Falha ao iniciar o módulo LoRa!");
    while (1);
  }

  Serial.println("LoRa iniciado com sucesso! Esperando pacotes...");
}

void loop() {
  // Verifica se chegou algum pacote
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Pacote recebido: ");

    // Lê o conteúdo do pacote
    while (LoRa.available()) {
      char c = LoRa.read();
      Serial.print(c);
    }

    Serial.println();
  }
}
