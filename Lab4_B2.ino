//6188089 - Nattawipa Saetae
#define SW 34 //Input from a switch on GPIO34 //6188089 Nattawipa
#define LED 12 //Output to LED on GPIO12

int state;

void setup() {
  pinMode(SW, INPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  state = digitalRead(SW);
  if(state == HIGH){
    digitalWrite(LED, HIGH);
  }
  else{
    digitalWrite(LED, LOW);
  }
  Serial.println(state);
}
