#pragma once

// Wi-Fi credentials
/* Add your wifi credentials here */ 
const char* WIFI_SSID     = "YOUR_SSID";
const char* WIFI_PASSWORD = "YOUR_PASSWORD";

// SMTP server settings
#define SMTP_HOST     "smtp.gmail.com"
#define SMTP_PORT     465 
#define AUTHOR_EMAIL  "mailboxnotifier9@gmail.com"
#define AUTHOR_PASSWORD "Password" // Enter the correct App Password here

// Recipient list 
const size_t RECIPIENT_COUNT = 3;
// adjust the email addresses
String recipients[RECIPIENT_COUNT] = {
  "testperson1@student.unisg.ch",
  "testperson2@student.unisg.ch",
  "testperson3@student.unisg.ch"
};
