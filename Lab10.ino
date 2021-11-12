//6188089 - Nattawipa Saetae

#define _DEBUG

/* Session 1 AHT */
#include <Adafruit_AHTX0.h>
Adafruit_AHTX0 aht;
sensors_event_t humidity, temp;

/* Session 2 (WiFi)*/
#include <WiFi.h>
bool wifiConnected = true; // Flag determining if this device can connect to WiFi

/* Session 3 NETPIE2020*/
#include <PubSubClient.h> 

const char* ssid = "YingsakAIS1_2.4G"; // Your_SSID
const char* password = "Y0898111144Y"; // Your_SSID_Password
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "1bbf7d26-5572-4b30-908e-0038317f79b7"; // Client ID 
const char* mqtt_username = "KEgPPxPDHjd2NmWicrMXeck8GYGuiwHk"; // Token 
const char* mqtt_password = "9C2-8LdRdwmD#$6w2xm8(IfJApYqh6AN"; // Secret 

WiFiClient espClient;
PubSubClient client (espClient);

char msg[150];
long int lastMsg = 0; 
long int nowMsg = 0; 
int value = 0;
String output;

#define LED1 2 // use on-board LED

// Session NETPIE 2020

void reconnect() {
  while (!client.connected()) {
    
    Serial.print("Attempting MQTT connection…");
    
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("@msg/led"); //-- add subscribe
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
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
  if(String(topic) == "@msg/led") {
    if (message == "on"){
      digitalWrite(LED1,1); //1--on
      client.publish("@shadow/data/update", "{\"data\" : {\"led\" : \"on\"}}");
      Serial.println("LED ON");
    }
    else if (message == "off") {
      digitalWrite(LED1,0); //0-off
      client.publish("@shadow/data/update", "{\"data\" : {\"led\" : \"off\"}}");
      Serial.println("LED OFF");
    }
  }
}

void setup() {
  /* For debugging */
#ifdef _DEBUG
  Serial.begin(115200); // Initiate Serial for Serial Monitor
#endif
  pinMode(LED1, OUTPUT); //--led


  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 or AHT20 found");
  
  /* Session (WiFi) Connect WiFi */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  uint32_t t = millis();
  while(WiFi.status() != WL_CONNECTED){ // Wait until connected if not connected
    delay(100);
    Serial.print(".");
    if(millis() - t > 1000){
      if(WiFi.status() != WL_CONNECTED)
        wifiConnected = false; // If the time elapses pass 1s, set the WiFi status to not connected
      break;
    }
  }
  Serial.println("");
   /* Session (NETPIE) Event Handlers and Connection */
  
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback); //uncomment
    
}

void loop() {
  
    Serial.println(WiFi.localIP());
    if (!client.connected()) 
    { reconnect();
      Serial.println("reconnect");
    }
    client.loop(); 
    nowMsg = millis();
    
    aht.getEvent(&humidity, &temp);
    
    if (nowMsg - lastMsg > 1000) {
      lastMsg = nowMsg;
      ++value;
      
      output = "{\"data\": {\"temp\": "+ String(temp.temperature) +",\"humidity\": "+ String(humidity.relative_humidity) +"}}";  //--- change code

      Serial.println(output);
      output.toCharArray(msg, (output.length() + 1));
      client.publish("@shadow/data/update", msg);
      client.publish("@msg/test", "Hello NETPIE2020");
      Serial.println("Hello NETPIE2020");
    }
    delay(3000);
    
}
