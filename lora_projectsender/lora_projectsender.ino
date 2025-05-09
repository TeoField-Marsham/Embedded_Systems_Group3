#include <SPI.h>
#include <LoRa.h>

// Pinos LoRa para LilyGO T3
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

#define BAND 915E6  // Frequência LoRa (ajuste conforme necessário)

int counter = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Inicializa SPI e LoRa
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Erro ao iniciar LoRa");
    while (1);
  }

  Serial.println("LoRa inicializado com sucesso!");
}

void loop() {
  Serial.print("Enviando pacote: ");
  Serial.println(counter);

  LoRa.beginPacket();
  LoRa.print("Pacote ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;
  delay(5000);  // Intervalo entre pacotes
}
