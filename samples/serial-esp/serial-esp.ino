//Example to try to communicate with arduino via serial

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println("This is only a test");
  delay(1000);
}
