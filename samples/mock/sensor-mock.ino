//Pin that the thermistor is connected
#define THERMISTOR_PIN         A0
//Pin that the photo sensor is connected
#define PHOTO_PIN              A1

//Auxiliar resistor used to voltage division
#define AUX_RESISTOR           10000 
//Resistence at 25°C
#define THERMISTOR_RESISTENCE  200
//Temperature for normal resistence
#define THERMISTOR_TEMPERATURE 25

//Number of times that the value of temperature sensor will be read
#define NUMBER_OF_SAMPLES       5
//B coefficient
#define B_COEFFICIENT           3950  



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

void setup() {
  //Start serial baud rate
  Serial.begin(115200);

  //Using 3.3V
  analogReference(EXTERNAL);

  //Calibrate light levels
  lightLevel = analogRead(PHOTO_PIN);
  minLightLevel = lightLevel - 30;
  maxLightLevel = lightLevel;
}

void loop() {
  uint8_t i;
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

  Serial.print("Temperatura: [*C]"); Serial.println(temperatureInCelsius);
  Serial.print("Luminosidade: [%]"); Serial.println(lightLevel);
  
  delay(500);
}

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
Serial.println(value);
  float steinhart;
  steinhart = value / THERMISTOR_RESISTENCE;            // (R/Ro)
  steinhart = log(steinhart);                           // ln(R/Ro)
  steinhart /= B_COEFFICIENT;                           // 1/B * ln(R/Ro)
  steinhart += 1.0 / (THERMISTOR_TEMPERATURE + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                          // Invert
  steinhart -= 273.15;                                  // convert to C

  return steinhart;
}
 
