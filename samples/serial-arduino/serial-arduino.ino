//Example to try to communicate with ESP8266 via serial

#include <SoftwareSerial.h>

const int ESP8266_RX = 19;
const int ESP8266_TX = 18;

SoftwareSerial serialESP8266(ESP8266_RX, ESP8266_TX);

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
}

void loop() {
  String message = "";

  if (Serial1.available() > 0) {
    while (Serial1.available() > 0) {
      char c = (char)Serial1.read();
      message += c;
      delay(20);
    }
    Serial.println(message);
  }
}
