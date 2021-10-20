//6188089 - Nattawipa Saetae

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <FreeRTOS.h>

#include <Adafruit_AHTX0.h>
 
const char *SSID = "Teerasak 2.4 G"; //ESP32 not support 5G wifi
const char *PWD = "click123";

#define WHITE 25
#define RED 26
#define BLUE 27
 
// Web server running on port 80
WebServer server(80);
 
// Sensor
Adafruit_AHTX0 aht;

// JSON data buffer
StaticJsonDocument<250> jsonDocument;
char buffer[250];
 
// env variable
sensors_event_t humidity, temp;
 
void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(SSID);
  
  WiFi.begin(SSID, PWD);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    // we can even make the ESP32 to sleep
  }
 
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
}
 
void create_json(char *tag, float value, char *unit) { 
  jsonDocument.clear(); 
  jsonDocument["type"] = tag;
  jsonDocument["value"] = value;
  jsonDocument["unit"] = unit;
  serializeJson(jsonDocument, buffer);
  Serial.println("Buffer:");
  Serial.println(buffer);  
}
 
void add_json_object(char *tag, float value, char *unit) {
  JsonObject obj = jsonDocument.createNestedObject();
  obj["type"] = tag;
  obj["value"] = value;
  obj["unit"] = unit; 
}

void read_sensor_data(void * parameter) {
   for (;;) {
     aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
     Serial.println("Read sensor data");
 
     // delay the task
     vTaskDelay(60000 / portTICK_PERIOD_MS);
   }
}
 
void getTemperature() {
  Serial.println("Get temperature");
  create_json("temperature", temp.temperature, "°C");
  server.send(200, "application/json", buffer);
}
 
void getHumidity() {
  Serial.println("Get humidity");
  create_json("humidity", humidity.relative_humidity, "%rH");
  server.send(200, "application/json", buffer);
}
 
void getEnv() {
  Serial.println("Get env");
  jsonDocument.clear();
  add_json_object("temperature", temp.temperature, "°C");
  add_json_object("humidity", humidity.relative_humidity, "%rH");
  serializeJson(jsonDocument, buffer);
  server.send(200, "application/json", buffer);
}

void handlePost() {
  if (server.hasArg("plain") == false) {
    //handle error here
  }

  String body = server.arg("plain");
  Serial.println(body);
  deserializeJson(jsonDocument, body);
  
  // Get RGB components
  int red = jsonDocument["red"];
  int white = jsonDocument["white"];
  int blue = jsonDocument["blue"];

  Serial.print("Red: ");
  Serial.print(red);
  
  digitalWrite(RED, red);
  digitalWrite(WHITE, white);
  digitalWrite(BLUE, blue);
  delay(30);

  // Respond to the client
  jsonDocument.clear();
  add_json_object("red", red, "");
  add_json_object("white", white, "");
  add_json_object("blue", blue, "");
  serializeJson(jsonDocument, buffer);
  server.send(200, "application/json", buffer);
}
 
 
// setup API resources
void setup_routing() {
  server.on("/temperature", getTemperature);
  server.on("/humidity", getHumidity);
  server.on("/env", getEnv);
  server.on("/led", HTTP_POST, handlePost);
 
  // start server
  server.begin();
}


void setup_task() {
  xTaskCreate(
    read_sensor_data,    
    "Read sensor data",   // Name of the task (for debugging)
    1000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL             // Task handle
  );
}
 
void setup() {
  Serial.begin(9600);
  
   // Sensor setup
  if (!aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
  connectToWiFi();
  setup_task();
  setup_routing();  

  pinMode(RED, OUTPUT);
  pinMode(WHITE, OUTPUT);
  pinMode(BLUE, OUTPUT);
}
 
void loop() {
  server.handleClient();
  
}
