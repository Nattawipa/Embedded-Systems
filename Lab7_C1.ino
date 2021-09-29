//6188089 - Nattawipa Saetae

#define TRIG_PIN 4 //Trigger pin
#define ECHO_PIN 5 //Echo pin

long duration;
long distance;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration/2.0) / 29.1; //Convert the echo duration to distance in centimeters

  Serial.println("Distance: " + String(distance) + " cm");
}
