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

//MQTT server and port
const char* MQTT_SERVER = "192.168.25.9";
uint16_t MQTT_PORT = 1884;

char msg[50];
String message = "";
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
   reconnectMQTT();
  }

  //Reconnect to wifi in case of it looses connection
  reconnectWifi();

  client.loop();

  // Handle message received from Arduino and publish it
  if(Serial.available()) {
    while (Serial.available() > 0) {
      char c = (char)Serial.read();
      message += c;
      delay(20);
    }

    message.toCharArray(msg, message.length() + 1);
    client.publish("ESP8266-IPRJ-BAJA/arduino", msg);
  }
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

void callback(char* topic, byte* payload, unsigned int length) {
 // Not implemented
}

void handleUserConnected() {
  // Not implemented
}
