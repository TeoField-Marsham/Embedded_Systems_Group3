#include <SPI.h>
#include <LoRa.h> /* Install "LoRa" by Sandeep Mistry */
#include <WiFi.h> 
#include "time.h"
#include <ESP_Mail_Client.h> /* Install "ESP Mail Client" by mobizt */
#include <WebServer.h>              
WebServer server(80); // HTTP server on port 80

String eventLog;    

// SPI/LoRa pins for LilyGO T3 V1.6.1
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

#define BAND 915E6

// Wi-Fi credentials
/* Add your own wifi credentials */ 
const char* WIFI_SSID     = "WIFI_SSID";
const char* WIFI_PASSWORD = "WIFI_PASSWORD";

// SMTP server settings
#define SMTP_HOST     "smtp.gmail.com"
#define SMTP_PORT     465 
#define AUTHOR_EMAIL  "mailboxnotifier9@gmail.com"
#define AUTHOR_PASSWORD "dnituoohuvehxifs" // The App Password

// Recipient list
/* Add all users who should receive notifications when a letter arrives */ 
const size_t RECIPIENT_COUNT = 2;
String recipients[RECIPIENT_COUNT] = {
  "youremail@youremail.com",
  "anotheremail@youremail.com"
};

// NTP time settings (currently set for St. Gallen(Central European Summer Time))
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
  Serial.print("ESP IP address: ");
  Serial.println(WiFi.localIP());
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

int mailCount = 0;

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
  message.text.content = "You received some at time " + timestamp +". You have now at least " + String(mailCount) + " piece(s) of mail.";

  // Connect to mail service and send
  Serial.println("Sending e-mail...");
  if (!smtp.connect(&smtpConfig)) {
    Serial.println("SMTP connect failed: " + smtp.errorReason());
    return;
  }
  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("Send failed: " + smtp.errorReason());
  } else {
    Serial.println("E-mail sent successfully!");
  }
  smtp.closeSession();
}

// Define email template
void sendResetEmail(const String &timestamp) {
  // Build message
  SMTP_Message message;
  message.sender.name = "Mailbox Notifier";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "Mail collected!";
  // Add each recipient
  for (size_t i = 0; i < RECIPIENT_COUNT; ++i) {
    message.addRecipient("", recipients[i]);
  }
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  message.text.content = "Your mailbox has been collected at " + timestamp;

  // Connect to mail service and send
  Serial.println("Sending e-mail...");
  if (!smtp.connect(&smtpConfig)) {
    Serial.println("SMTP connect failed: " + smtp.errorReason());
    return;
  }
  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("Send failed: " + smtp.errorReason());
  } else {
    Serial.println("E-mail sent successfully!");
  }
  smtp.closeSession();
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Connect to the wifi network and get the time
  initWiFi();
  initTime();

  // Configure SMTP session
  smtpConfig.server.host_name = SMTP_HOST;
  smtpConfig.server.port = SMTP_PORT;
  smtpConfig.login.email = AUTHOR_EMAIL;
  smtpConfig.login.password = AUTHOR_PASSWORD;
  smtpConfig.login.user_domain   = "";
  // smtpConfig.secure.startTLS = true;  // change if needed
  // smtpConfig.priority = esp_mail_smtp_priority_normal;

  MailClient.networkReconnect(true);

  // Start HTTP server
  server.on("/", [&]() {
    String html =
      "<!DOCTYPE html><html><head><meta charset='utf-8'>"
      "<title>Mailbox Event Log</title></head><body>"
      "<h1>Mailbox Event Log</h1>"
      "<div id='log'>" + eventLog + "</div>"
      "<script>"
      "async function refreshLog() {"
      "  const res = await fetch('/');"
      "  const text = await res.text();"
      "  const match = text.match(/<div id='log'>([\\s\\S]*?)<\\/div>/);"
      "  if (match) document.getElementById('log').innerHTML = match[1];"
      "}"
      "setInterval(refreshLog, 5000);"
      "</script>"
      "</body></html>";
    server.send(200, "text/html", html);
  });
  server.begin();

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
  server.handleClient();

  // Look out for packages
  int packetSize = LoRa.parsePacket();
  if (packetSize > 0) {
    String message = "";

    while (LoRa.available()) {
      message += (char)LoRa.read();
    }

    message.trim(); // Remove any trailing newlines or spaces
    String now = getFormattedTime();

    Serial.print("Mail: ");
    if (message=="reset"){
      mailCount = 0;
      sendResetEmail(now);
      eventLog += now + " – Mail collected<br>";
      } else if (message=="newmail"){
      mailCount++;
      sendEmail(now);
      eventLog += now + " – New mail detected. Current mail count: " + String(mailCount) + "<br>";
    }
    Serial.println(mailCount);
    delay(30000);
  }
}
