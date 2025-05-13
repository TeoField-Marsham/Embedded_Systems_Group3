#include <SPI.h> //SPI library for communication with LoRa module
#include <LoRa.h> //LoRa library
#include <WiFi.h> //WiFi library for ESP32
#include <WebServer.h> //Web server library for ESP32
#include <time.h> //Time library for ESP32
#include <vector> //Vector library for storing timestamps
#include "loging_data.h" //WiFi credentials and SMTP settings

using namespace std;

// Pins SPI/LoRa para a LilyGO T3 V1.6.1
#define SCK 5 //SPI clock pin
#define MISO 19 //SPI MISO pin
#define MOSI 27 //SPI MOSI pin
#define SS 18 //LoRa chip select pin
#define RST 14 //LoRa reset pin
#define DIO0 26 //LoRa IRQ pin for packet reception

//frequency for LoRa communication for European region
#define BAND 868E6 
 // Frequencies LoRa: use 915E6 (Americas), 866E6 (Europa), ou 433E6 (Asia)

 WebServer server(80);

vector<pair<time_t, String>> loraMessages;  // Vector of timestamp + message pairs
const int logPeriodHours = 1;  // Log period in hours
void setup() {
  Serial.begin(115200);
  while (!Serial);

 // Connecting to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

// Set up NTP time synchronization
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("Waiting for NTP sync...");
  delay(2000);  // delay to allow time sync

  //Web server setup
  server.on("/", HTTP_GET, []() {
    time_t nowTime = time(nullptr);
    time_t threshold = nowTime - (logPeriodHours * 3600);

    String response = "LoRa messages received in the last ";
    response += String(logPeriodHours);
    response += " hours:\n";

    int count = 0;
    for (const auto& entry : loraMessages) {
      if (entry.first >= threshold) {
        count++;
        char timeStr[26];
        ctime_r(&entry.first, timeStr);  // Convert timestamp to string
        response += "[" + String(timeStr) + "] ";
        response += entry.second + "\n";
      }
    }

    response = "Count: " + String(count) + "\n" + response;
    server.send(200, "text/plain", response);
  });
  server.begin();
  Serial.println("Web server started.");

  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);

  Serial.println("Initializing LoRa Receiver...");
  if (!LoRa.begin(BAND)) {
    Serial.println("LoRa init failed. Check connections.");
    while (true);
  }
  Serial.println("LoRa started and ready.");
}

void loop() {
  server.handleClient(); 

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String received = "";
    while (LoRa.available()) {
      received += (char)LoRa.read();
    }

    time_t now = time(nullptr);
    loraMessages.push_back({now, received});  //storing timestamp and message

    Serial.print("LoRa message received: ");
    Serial.println(received);
  }
}
