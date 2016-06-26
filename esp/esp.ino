/* @Duthor: Yuri Silveira <ypereirars@gmail.com>
 * @CreatedAt: June 25, 2016
 * @Description: A module that will connect to an arduino to pull data from
 *               sensors and send it to a broker via MQTT protocol.
 */

 #include <ESP8266WiFi.h>
 #include <PubSubClient.h> //It only supports QoS0 or 1

//Wifi ssid and password
const char* SSID = "...";
const char* PASSWORD = "...";

//MQTT server and port
const char* MQTT_SERVER = "192.168.25.9";
uint16_t MQTT_PORT = 1884;

//PubSubClient configuration
WiFiClient ESP_CLIENT;
PubSubClient client(ESP_CLIENT);

long lastMsg = 0; //Time lapsed since last message arrived
char msg[50];     //Message to publish
long value = 0;
int inc = 1;

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
  handleSendMessage();
}


//Wifi code
void setupWifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnectWifi() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
}

//MQTT code
void reconnectMQTT() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      handleUserConnected();
      break;
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  //Hendle received mqttt messages
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  //handle received message

}

void handleUserConnected() {
  Serial.println("connected");
  // Once connected, publish an announcement...
  client.publish("outTopic", "hello world");
  // ... and resubscribe
  client.subscribe("inTopic");
}

void handleSendMessage() {
  long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;

    if (value == 100 ) {
      inc = -1;
    }
    if (value == 0) {
      inc = 1;
    }

    value += inc;
    snprintf (msg, 75, "%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("2/arduino/temperature/", msg);
  }
}
