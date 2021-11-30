//6188089 - Nattawipa Saetae

#include <WiFi.h>
#include <PubSubClient.h>

#include <Adafruit_AHTX0.h>

#include "config.h"
#include <ESP32Servo.h>

#define SERVO_PIN 23

Servo sv;

#define LED 27
#define FREQ 5000
#define LED_CH0 0
#define LED_RES 8

/* MQTT Instance */
WiFiClient espClient;
PubSubClient client(espClient);

bool wifiConnected = true;

Adafruit_AHTX0 aht;
sensors_event_t humidity, temp;

/* Value Buffer */
char buf[200]; //Reserved for 200 bytes
long now, lastMsg;

void setup() {
  Serial.begin(115200);
  sv.setPeriodHertz(50);
  ledcSetup(LED_CH0, FREQ, LED_RES);
  ledcAttachPin(LED, LED_CH0);
  
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

  /* MQTT NetPie Server */
  client.setServer(mqttServer, mqttPort); //NetPie server and port
  client.setCallback(callback);
}

void loop() {
  now = millis(); //Milliseconds now (timestamp)
  if(now - lastMsg > 5000){ //Publish new messages to the broker again when 5s passes; otherwise, let it handle subscribed messages with little blocking
    lastMsg = now;
    if(!client.connected())
      netpieReconnect();
    client.loop();
    aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  
    /* NetPie Transmission */
    sprintf(buf, "{\"data\":{\"temperature\":%.2f, \"Humidity\":%.2f}}", temp.temperature, humidity.relative_humidity);
    Serial.println(buf);
    client.publish("@shadow/data/update", buf);
    delay(1);
  }
}

void netpieReconnect(){
  while(!client.connected()){
    Serial.println("Connecting to NetPie...");
    if (client.connect(mqttClient, mqttUser, mqttPassword )) {
      Serial.println("connected");
      client.subscribe("@msg/Servo");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length){
  sv.attach(SERVO_PIN);
  char msg[length+1];
  memcpy(msg, payload, length);
  msg[length] = '\0';
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.printf(": %s]\n", msg);
  int val = String(msg).toInt();
  sv.write(val);

  sprintf(buf, "{\"data\":{\"Servo\":%d}}", val);
  Serial.println(buf);
  client.publish("@shadow/data/update", buf); //Feedback and record the updated value to NetPie
}#include <WiFi.h>
#include <PubSubClient.h>

#include <Adafruit_AHTX0.h>

#include "config.h"
#include <ESP32Servo.h>

#define SERVO_PIN 23

Servo sv;

#define LED 4
#define FREQ 5000
#define LED_CH0 0
#define LED_RES 8

/* MQTT Instance */
WiFiClient espClient;
PubSubClient client(espClient);

bool wifiConnected = true;

Adafruit_AHTX0 aht;
sensors_event_t humidity, temp;

/* Value Buffer */
char buf[200]; //Reserved for 200 bytes
long now, lastMsg;

void setup() {
  Serial.begin(115200);
  sv.setPeriodHertz(50);
  ledcSetup(LED_CH0, FREQ, LED_RES);
  ledcAttachPin(LED, LED_CH0);
  
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

  /* MQTT NetPie Server */
  client.setServer(mqttServer, mqttPort); //NetPie server and port
  client.setCallback(callback);
}

void loop() {
  now = millis(); //Milliseconds now (timestamp)
  if(now - lastMsg > 5000){ //Publish new messages to the broker again when 5s passes; otherwise, let it handle subscribed messages with little blocking
    lastMsg = now;
    if(!client.connected())
      netpieReconnect();
    client.loop();
    aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  
    /* NetPie Transmission */
    sprintf(buf, "{\"data\":{\"temperature\":%.2f, \"Humidity\":%.2f}}", temp.temperature, humidity.relative_humidity);
    Serial.println(buf);
    client.publish("@shadow/data/update", buf);
    delay(1);
  }
}

void netpieReconnect(){
  while(!client.connected()){
    Serial.println("Connecting to NetPie...");
    if (client.connect(mqttClient, mqttUser, mqttPassword )) {
      Serial.println("connected");
      client.subscribe("@msg/Servo");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length){
  sv.attach(SERVO_PIN);
  char msg[length+1];
  memcpy(msg, payload, length);
  msg[length] = '\0';
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.printf(": %s]\n", msg);
  int val = String(msg).toInt();
  sv.write(val);

  sprintf(buf, "{\"data\":{\"Servo\":%d}}", val);
  Serial.println(buf);
  client.publish("@shadow/data/update", buf); //Feedback and record the updated value to NetPie
}
