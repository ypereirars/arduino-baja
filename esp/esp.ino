/* @Duthor: Yuri Silveira <ypereirars@gmail.com>
 * @CreatedAt: June 25, 2016
 * @Description: A module that will connect to an arduino to pull data from
 *               sensors and send it to a broker via MQTT protocol.
 */

 #include <ESP8266WiFi.h>
 #include <PubSubClient.h> //It only supports QoS0 or 1

//Wifi ssid and password
const char* ssid = "...";
const char* password = "...";

//MQTT server and port
const char mqtt_server[40] = "192.168.25.9";
uint16_t mqtt_port = 1884;

//PubSubClient configuration
WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0; //Time lapsed since last message arrived
char msg[50];     //Message to publish
long value = 0;


void setup() {
  Serial.begin(115200); //Start serial communication
  setup_wifi(); //Connect to the wifi

  client.setServer(mqtt_server, mqtt_port);  //Set mqtt server
  client.setCallback(callback); //set a callback (optional)
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  handleSendMessage();
}

void reconnect() {
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

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

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
    value++;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
}
