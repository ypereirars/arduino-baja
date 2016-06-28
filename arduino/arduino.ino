/* @Duthor: Yuri Silveira <ypereirars@gmail.com>
 * @CreatedAt: June 25, 2016
 * @Description: A module that will read data from sensors and send it to ESP8266.
 * ESP8266 module must be connected to RX1, TX1 (port 19 and 18 respectively)
 */

//BEGIN MOCK DATA
 //Pin that the thermistor is connected
const int THERMISTOR_PIN        = 0;
 //Pin that the photo sensor is connected
const int PHOTO_PIN             = 1;

//Auxiliar resistor used to voltage division
const int AUX_RESISTOR          = 10000;
//Resistence at 25°C
const int THERMISTOR_RESISTENCE = 200;
//Temperature for normal resistence
const int THERMISTOR_TEMPERATURE= 25;

//Number of times that the value of temperature sensor will be read
const int NUMBER_OF_SAMPLES      = 5;
//B coefficient
const int B_COEFFICIENT          = 3950;

const int MAX_BUFF_LENGTH        = 50;

//Variables used to read and calibrate light level.
int minLightLevel,
    maxLightLevel,
    lightLevel;

/* Formula used to convert Ohms to K
 * (1/T) = (1/T0) + (1/B) * ln ( R/R0)
 * Where:
 *  T: Temperature we want to find
 *  T0: 25°C (298.15K)
 *  R0: Resistence at 25°C (298.15K)
 *  R: Resistence of the sensor we measure
 */
 //END MOCK DATA

void setup() {
  Serial.begin(115200);   //Arduino Serial
  Serial1.begin(115200);  //ESP8266 Serial
  Serial.setTimeout(10);
//BEGIN MOCK DATA
  //Using 3.3V
  analogReference(EXTERNAL);

  //Calibrate light levels
  lightLevel = analogRead(PHOTO_PIN);
  minLightLevel = lightLevel - 30;
  maxLightLevel = lightLevel;
//END MOCK DATA
}

void loop() {
//BEGIN MOCK DATA
  uint8_t i;
  char buff[MAX_BUFF_LENGTH];
  char buffFloat[5];
  float averageTemperatureReading = 0.0,
        temperatureInCelsius;

  //measure light lever NUMBER_OF_SAMPLES times
  for (i = 0; i < NUMBER_OF_SAMPLES; i++) {
    averageTemperatureReading += analogRead(THERMISTOR_PIN);
    delay(50);
  }

  //Take average of the readings
  averageTemperatureReading /= NUMBER_OF_SAMPLES;

  temperatureInCelsius = convertResistenceIntoTemperature(averageTemperatureReading);
  lightLevel  = ajustLightLevel(minLightLevel, maxLightLevel);

  snprintf(buff, MAX_BUFF_LENGTH, "lightLevel:%d", lightLevel);
  writeDataToESPSerial(buff);

  dtostrf(averageTemperatureReading, 4, 2, buffFloat); // snprintf does not work for float point in arduino.
  snprintf(buff, MAX_BUFF_LENGTH, "temperature:%s", buffFloat);
  writeDataToESPSerial(buff);

  delay(1000);
//END MOCK DATA
}

void writeDataToESPSerial(char* msg) {
  Serial1.println(msg);
}

//Send data read from serial [MOCK]
/*
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
*/




int ajustLightLevel(int &minLevel, int &maxLevel) {
  int adjustedLightLevel;

  //Auto-adjust the minimum and maximum limits in real time
  lightLevel = analogRead(PHOTO_PIN);

  if(minLevel > lightLevel){
    minLevel = lightLevel;
  }
  if(maxLevel < lightLevel){
    maxLevel = lightLevel;
  }

  adjustedLightLevel = map(lightLevel, minLevel, maxLevel, 0, 100);

  return adjustedLightLevel;
}
float convertResistenceIntoTemperature(float value) {
  //Convert the value to resistance
  value = 1023 / value - 1;
  value = AUX_RESISTOR / value;

  float steinhart;
  steinhart = value / THERMISTOR_RESISTENCE;            // (R/Ro)
  steinhart = log(steinhart);                           // ln(R/Ro)
  steinhart /= B_COEFFICIENT;                           // 1/B * ln(R/Ro)
  steinhart += 1.0 / (THERMISTOR_TEMPERATURE + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                          // Invert
  steinhart -= 273.15;                                  // convert to C

  return steinhart;
}
