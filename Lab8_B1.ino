//6188089 Nattawipa Saetae

#define LED 32 
#define SW 36

bool ledState;
bool swState;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(SW, INPUT);

  ledState = true;

  digitalWrite(LED, ledState);
}

void loop() {
  swState = digitalRead(SW);
  if(!swState){ //When pressed
    ledState = !ledState;
    digitalWrite(LED, ledState);
  }
  delay(1000);
}
