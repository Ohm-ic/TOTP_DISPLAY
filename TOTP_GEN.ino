#include <Base32-Decode.h>   //https://github.com/dirkx/Arduino-Base32-Decode/tree/main/examples/Base32-decode
#include <Wire.h>  
#include "SH1106Wire.h"      //https://github.com/ThingPulse/esp8266-oled-ssd1306/tree/master (Also Availale in Arduino IDE Search for esp8266-oled-ssd1306)
#include <qrcodeoled.h>      // https://github.com/yoprogramo/QRcodeOled +  https://github.com/yoprogramo/QRcodeDisplay (Install Both the Libraries, Also Available on Arduino Library Search for QRcodeOLED and QRcodeDisplay)
#include <TOTP.h>            // https://github.com/lucadentella/TOTP-Arduino
#include <WiFi.h>
#include <NetworkClient.h> 
#include <time.h>


// Wi-Fi Config
const char* ssid = "ESPP";
const char* password = "Nasa@2023";



// Secret Key
String secretKey = "JBSWY3DPEHPK3PXP";  // Base32-encoded secret key
String decodedKey;
uint8_t hmacKey[20];
TOTP totp(hmacKey, sizeof(hmacKey), 30); // TOTP with time interval of 30 seconds

// OLED Display
SH1106Wire display(0x3c, SDA, SCL);  // OLED display
QRcodeOled qrcode(&display);  // QR code for OLED

void setup() {
  Wire.begin();
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  Serial.println(WiFi.localIP());

  // Configure SNTP
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  // Wait for time to sync
  Serial.println("Waiting for time to sync...");
  while (time(nullptr) < 1000000000) {  // Wait until epoch time is valid (after 1 Jan 1970)
    delay(100);
  }
  Serial.println("Time synced");

  // Decode the secret key
  int result = base32decodeToString(secretKey, decodedKey);
  if (result < 0) {
    Serial.println("Failed to Decode the Key");
    return;
  }

  // Convert the decoded key string to a byte array
  for (size_t i = 0; i < decodedKey.length() && i < sizeof(hmacKey); i++) {
    hmacKey[i] = static_cast<uint8_t>(decodedKey[i]);
  }

  totp = TOTP(hmacKey, sizeof(hmacKey), 30); // Initialize TOTP with the decoded key
  qrcode.init();  // Initialize QR code display
}

void loop() {
   CodeGen();
}

void CodeGen()
{
    // Get the current epoch time
  time_t now = time(nullptr);
  int currentSecond = now % 60;  // Extract current second (mod 60 for seconds of the minute)

  // Wait until the seconds are either 0 or 30 (time intervals)
  while (currentSecond != 0 && currentSecond != 30) {
    //delay(100);  // Add a small delay to avoid excessive CPU usage
    now = time(nullptr);  // Update the current time
    currentSecond = now % 60;  // Extract new second
  }

  // Generate TOTP (Time-based One-Time Password)
  String generatedTOTP = totp.getCode(now);  // Pass the epoch time directly
  Serial.print("Generated Timestamp (Epoch): ");
  Serial.println(now);
  Serial.println("Generated TOTP: " + generatedTOTP);

  // Generate QR code and update OLED screen
  qrcode.create(generatedTOTP.c_str());
  qrcode.screenupdate();
  delay(30000);// Adding delay of 30 Sec 
}
