//6188089 - Nattawipa Saetae
#define SW 14 //Input from a switch on GPIO14 //6188089 Nattawipa
#define LED 12 //Output to LED on GPIO12

int state;

void setup() {
  pinMode(SW, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  state = digitalRead(SW);
  if(state == LOW){
    digitalWrite(LED, HIGH);
  }
  else{
    digitalWrite(LED, LOW);
  }
  Serial.println(1-state);
}
