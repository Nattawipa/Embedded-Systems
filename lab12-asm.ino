//6188089 - Nattawipa Saetae

#include <WiFi.h>
#include <PubSubClient.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_AHTX0.h>

/* MQTT Instance */
WiFiClient espClient;
PubSubClient client(espClient);

/* NetPie MQTT Server */
const char* mqttServer = "192.168.0.150";
const int mqttPort = 1883;

/* (WiFi) Variables */
char ssid[] = "Teerasak 2.4 G"; // Your WiFi credentials.
char pass[] = "click123"; // Set password to "" for open networks.
bool wifiConnected = true;

Adafruit_AHTX0 aht;
sensors_event_t humidity, temp;

/* Value Buffer */
char buf[100]; //Reserved for 100 bytes

// LED Pin
int ledPin = 4;

void setup() {
  Serial.begin(115200);
  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 or AHT20 found");

  /* (WiFi) Connection Setup */
  WiFi.mode(WIFI_STA);      // set to station mode
  WiFi.begin(ssid, pass);   // connect to an access point
  delay(5000);

  /* loop until ESP32 can sucesfully connect to the WiFi */
  Serial.printf("Connecting to %s ", ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  /* connection is successful */
  Serial.println(" CONNECTED");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  /* MQTT Server */
  client.setServer(mqttServer, mqttPort); //mqtt server and port
  client.setCallback(callback);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data

  /* mqtt-server Transmission */
  sprintf(buf, "%.2f", temp.temperature); //Make it text in the buffer (buf)
  Serial.println(buf);
  client.publish("esp32/temperature", buf);
  sprintf(buf, "%.2f", humidity.relative_humidity);
  Serial.println(buf);
  client.publish("esp32/humidity", buf);
  delay(1000);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++) {
      message = message + (char)payload[i];
  }
  Serial.println(message);
  int val = String(message).toInt();
  if (String(topic) == "@esp32/ledpwn"){
    int level = message.toInt();
    ledcWrite(ledPin, val);
    Serial.println("Brightness Level = " + String(speed));
    client.publish("@shadow/data/update", msg);
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("esp32 Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/led");
      client.subscribe("esp32/ledpwn");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
