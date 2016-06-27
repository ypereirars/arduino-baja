/* @Duthor: Yuri Silveira <ypereirars@gmail.com>
 * @CreatedAt: June 25, 2016
 * @Description: A module that will read data from sensors and send it to ESP8266.
 * ESP8266 module must be connected to RX1, TX1 (port 19 and 18 respectively)
 */



void setup() {
  Serial.begin(115200);   //Arduino Serial
  Serial1.begin(115200);  //ESP8266 Serial
}

void loop() {
  // Read sensors and than send values to ESP
  readDataFromArduinoSerial();
}

void writeDataToESPSerial(String msg) {
  Serial1.print(msg);
}

//Send data read from serial [MOCK]
void readDataFromArduinoSerial() {
  String message = "";
  
  if (Serial.available() > 0) { // Reads serial if there's data
    while (Serial.available() > 0) { // Keep reading until there's no data
      char c = (char)Serial.read(); // Read char by char
      message += c;
      delay(20);
    }
    writeDataToESPSerial(message); //Write data to ESP
  }
}
