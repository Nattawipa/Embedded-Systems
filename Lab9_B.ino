//6188089 - Nattawipa Saetae

/* 
https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/
 */
#include <WiFi.h>
#include <PubSubClient.h> 
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

const char* ssid = "Teerasak 2.4 G"; // your SSID
const char* password = "click123"; // your SSID password
const char* mqtt_server = "192.168.64.2"; //IP Address of your mqtt_server

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;
float tempvalue=0;
float humidvalue=0;
char tempString[8];
char humidString[8];

const int ledPin = 25;

void setup_wifi();
void callback(char* topic, byte* message, unsigned int length);
void reconnect();

void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit AHT10/AHT20 MQTT!");

  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 or AHT20 found");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);
  
}

void loop() 
{ 
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) 
  {
    lastMsg = now;
     
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  tempvalue = temp.temperature;
  humidvalue = humidity.relative_humidity;
  dtostrf(tempvalue, 1, 2, tempString);
  dtostrf(humidvalue, 1, 2, humidString);
  
  Serial.print("Temperature: "); Serial.print(tempString); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidvalue); Serial.println("% rH");
  
  client.publish("esp32/temperature",tempString);
  client.publish("esp32/humidity",humidString);
   
  delay(500);
 }
}
void setup_wifi() 
{
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

void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32 Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void callback(char* topic, byte* message, unsigned int length) 
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }
}
