#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h> 
#include "time.h"
#include <ESP_Mail_Client.h> /* Need to install ESP Mail Client by bintray.com/mobizt */

// LoRa pins
#define SCK     5
#define MISO    19
#define MOSI    27
#define SS      18   // LoRa NSS
#define RST     14
#define DIO0    26
#define BAND    915E6

// Wi-Fi credentials
/* Add your wifi credentials here */ 
const char* WIFI_SSID     = "YOUR_SSID";
const char* WIFI_PASSWORD = "YOUR_PASSWORD";

// SMTP server settings
#define SMTP_HOST     "smtp.gmail.com"
#define SMTP_PORT     465 
#define AUTHOR_EMAIL  "mailboxnotifier9@gmail.com"
#define AUTHOR_PASSWORD "dnituoohuvehxifs" // The App Password

// Recipient list
const size_t RECIPIENT_COUNT = 3;
String recipients[RECIPIENT_COUNT] = {
  "teo.field-marsham@student.unisg.ch",
  "ferhat.arslan@student.unisg.ch",
  "thiago.teixeiradossantos@student.unisg.ch"
};

// NTP time settings (St. Gallen is UTC+1 or UTC+2 DST)
const char* NTP_SERVER = "pool.ntp.org";
const long  GMT_OFFSET_SEC    = 3600;   // UTC+1
const int   DST_OFFSET_SEC    = 3600;   // +1h daylight

// Global mail objects 
SMTPSession smtp;
ESP_Mail_Session smtpConfig;

// Connect to the home wifi network
void initWiFi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected!");
}

// Determine the local time 
void initTime() {
  configTime(GMT_OFFSET_SEC, DST_OFFSET_SEC, NTP_SERVER);
  Serial.print("Waiting for NTP time");
  struct tm ti;
  while (!getLocalTime(&ti)) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" OK");
}

// Format the time
String getFormattedTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return String("unknown time");
  }
  char buf[32];
  // example: "2025-05-12 14:23:45"
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buf);
}

// Define email template
void sendEmail(const String &timestamp) {
  // Build message
  SMTP_Message message;
  message.sender.name = "Mailbox Notifier";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "You received mail!";
  // Add each recipient
  for (size_t i = 0; i < RECIPIENT_COUNT; ++i) {
    message.addRecipient("", recipients[i]);
  }
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  message.text.content = "You received some at time " + timestamp;

  // Connect to mail service and send
  Serial.println("Sending e-mail...");
  if (!smtp.connect(&smtpConfig)) {
    Serial.println("  > SMTP connect failed");
    return;
  }
  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.print("  > Error sending: ");
    Serial.println(smtp.errorReason());
  } else {
    Serial.println("  > E-mail sent successfully!");
  }
  smtp.closeSession();
}


// Main loop
void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Connect to the wifi network and get the time
  initWiFi();
  initTime();

  // Configure SMTP session
  smtpConfig.server.host = SMTP_HOST;
  smtpConfig.server.port = SMTP_PORT;
  smtpConfig.login.email = AUTHOR_EMAIL;
  smtpConfig.login.password = AUTHOR_PASSWORD;
  smtpConfig.secure.startTLS = true;  // change if needed
  smtpConfig.priority = esp_mail_smtp_priority_normal;

  // Start the LoRa receiver
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
    String payload;
    while (LoRa.available()) {
      payload += (char)LoRa.read();
    }
    String now = getFormattedTime();
    Serial.printf("Packet received at %s: %s\n", now.c_str(), payload.c_str());

    // Send notification e-mail
    sendEmail(now);

    // 30 second delay so that if the postman puts in letters one at a time, you dont get lots of emails
    delay(30000);
  }
}
