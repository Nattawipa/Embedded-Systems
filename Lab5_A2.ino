//6188089 - Nattawipa Saetae 
#define LDR 12

int aValue;

void setup() {
  Serial.begin(9600);
}

void loop() {
  aValue = analogRead(LDR); //12 bits ADC
  Serial.println(aValue);
}
