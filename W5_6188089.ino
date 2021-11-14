//6188089 - Nattawipa Saetae

#include <WiFi.h>
#include <PubSubClient.h>


const char* ssid = "Teerasak 2.4 G";
const char* password = "click123";
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_client = "46ac06a6-bce6-48af-8605-4238e3e1eeb3";
const char* mqtt_username = "iosgvE1fgTJNRsdBVNHLc3Xbsyc5QcjX";
const char* mqtt_password = "S4TkMhhjdJ6PuJePhQJd6F)nwTVoXfar";

WiFiClient espClient;
PubSubClient client(espClient);
char msg[150];


void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection…");
    if (client.connect(mqtt_client, mqtt_username, mqtt_password)) {
      Serial.println("NETPIE2020 connected");
      client.subscribe("@msg/control/led");
      client.subscribe("@msg/control/speed");
      client.subscribe("@msg/test3");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

// ----- CALLBACK -----

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;

  for (int i = 0; i < length; i++) 
  {
     message = message + (char)payload[i];
  }
  Serial.println(message);

  if(String(topic) == "@msg/control/led") 
  {
    if (message == "on")
    {
      Serial.println("LED ON");
      String data = "{\"data\" : { \"led\" : \"on\" }}";
      Serial.println(data);
      data.toCharArray(msg, (data.length() + 1));
      client.publish("@shadow/data/update", msg);
    }
    else if (message == "off") 
    {
      Serial.println("LED OFF");
      String data = "{\"data\" : { \"led\" : \"off\" }}";
      Serial.println(data);
      data.toCharArray(msg, (data.length() + 1));
      client.publish("@shadow/data/update", msg);
    }
  }
  if (String(topic) == "@msg/control/speed")
  {
    int speed = message.toInt();
    Serial.println("Speed Motor = " + String(speed));
    String data = "{\"data\" : { \"speed_motor\" :" + String(speed) + "}}" ;
    Serial.println(data);
    data.toCharArray(msg, (data.length() + 1));
    client.publish("@shadow/data/update", msg);
  }
}

// ----- SETUP -----

void setup() 
{
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() 
{
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();

  int temperature = 25;
  int humidity = 50;

  String data = "{\"data\": {\"temperature\": "+ String(temperature) +",\"humidity\": "+ String(humidity) +"}}";
  Serial.println(data);
  data.toCharArray(msg,(data.length()+1));
  client.publish("@shadow/data/update",msg);
  delay(1000);
}
