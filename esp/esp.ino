/* @Duthor: Yuri Silveira <ypereirars@gmail.com>
 * @CreatedAt: June 25, 2016
 * @Description: A module that will connect to an arduino to pull data from
 *               sensors and send it to a broker via MQTT protocol.
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h> //It only supports QoS0 or 1

//Wifi ssid and password
const char* SSID = "WiFi";
const char* PASSWORD = "P@ssw0rd!";
const int INPUT_SIZE = 500;

//MQTT server and port
const char* MQTT_SERVER = "192.168.25.9";
uint16_t MQTT_PORT = 1884;

//PubSubClient configuration
WiFiClient ESP_CLIENT;
PubSubClient client(ESP_CLIENT);

void setup() {
 Serial.begin(115200); //Start serial communication
 setupWifi(); //Connect to the wifi

 client.setServer(MQTT_SERVER, MQTT_PORT);  //Set mqtt server
 client.setCallback(callback); //set a callback (optional)
}

void loop() {
  if (!client.connected()) {
    Serial.println("Reconnecting MQTT");
   reconnectMQTT();
  }

  //Reconnect to wifi in case of it looses connection
  reconnectWifi();

  client.loop();

  handleReceivedMessage();
}


//Wifi code
void setupWifi() {
 delay(10);
 // We start by connecting to a WiFi network
 WiFi.begin(SSID, PASSWORD);

 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
 }
}

void reconnectWifi() {
 while (WiFi.status() != WL_CONNECTED) {
   delay(100);
 }
}

//MQTT code
void reconnectMQTT() {
 // Loop until we're reconnected
 while (!client.connected()) {
   // Attempt to connect
   if (client.connect("ESP8266-IPRJ-BAJA")) {
     handleUserConnected();
     break;
   } else {
     // Wait 2 seconds before retrying
     delay(2000);
   }
 }
}

void handleReceivedMessage() {
  // Handle message received from Arduino and publish it

  // Get next command from Serial (add 1 for final 0)
  char input[INPUT_SIZE + 1];
  char topic[INPUT_SIZE + 1];

  byte size = Serial.readBytes(input, INPUT_SIZE);
  // Add the final 0 to end the C string
  input[size] = 0;

  // Read each command pair
  char* command = strtok(input, "&");

  while (command != 0) {
    char* separator = strchr(command, ':');
    if (separator != 0)
    {
      *separator = 0;
      char* sensor = command;
      ++separator;
      char* value = separator;

      snprintf(topic, INPUT_SIZE,"ESP8266-IPRJ-BAJA/arduino/%s", sensor);
      client.publish(topic, value);
    }
    command = strtok(0, "&");
  }

  delay(200);
}

void callback(char* topic, byte* payload, unsigned int length) {
 // Not implemented
}

void handleUserConnected() {
  // Not implemented
}
