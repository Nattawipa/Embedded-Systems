//6188089 - Nattawipa Saetae
// C++ code

int pin3 = 0; //button switch

int dip_state = 0; //dip switch

void setup()
{
  pinMode(34, INPUT);
  pinMode(35, INPUT);
  Serial.begin(9600);

  pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);
}

void loop()
{
  dip_state = digitalRead(34); //read dip switch
  pin3 = digitalRead(35);  //read button switch
  Serial.print(pin3);
  Serial.print("---");
  Serial.println(dip_state);
  if (dip_state == 1) {
    if (pin3 == 1) {  
      digitalWrite(14, HIGH);
      digitalWrite(12, LOW);
    } else {
      digitalWrite(14, LOW);
      digitalWrite(12, HIGH);
    }
  } else {
    digitalWrite(14, LOW);
    digitalWrite(12, LOW);
  }
  delay(10); // Delay a little bit to improve simulation performance
}
