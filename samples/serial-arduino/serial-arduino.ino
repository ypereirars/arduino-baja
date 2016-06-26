//Example to try to communicate with ESP8266 via serial

#include <SoftwareSerial.h>

const int ESP8266_RX = 15;
const int ESP8266_TX = 14;

SoftwareSerial serialESP8266(ESP8266_RX, ESP8266_TX);

void setup() {
  Serial.begin(115200);
  serialESP8266.begin(115200);
}

void loop() {
  String message = "";

  if (serialESP8266.available() > 0) {
    while (serialESP8266.available() > 0) {
      char c = (char)serialESP8266.read();
      message += c;
      delay(20);
    }
    Serial.println(message);
  }
}
