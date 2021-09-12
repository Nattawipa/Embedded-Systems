//6188089 - Nattawipa Saetae
//VDO URL: https://drive.google.com/file/d/1i4Kiv9YjdCbEJAfTBKeq2YYLpHFfJLrX/view?usp=sharing
// the switch leg is too short

int dip_state = 0; //dip switch
int LED1 = 32;
int LED2 = 33; 
int LED3 = 25;
int LED4 = 26; 
int current = 0;

void setup()
{
  pinMode(34, INPUT);
  Serial.begin(9600);

  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
}

void loop()
{
  dip_state = digitalRead(34); //read dip switch
  Serial.println(dip_state);
  if (dip_state == 1) //switch on
  {
    if(current == 0 || current == LED4)
    {
      digitalWrite(LED1, HIGH); //led1 on
      digitalWrite(LED2, LOW); 
      digitalWrite(LED3, LOW); 
      digitalWrite(LED4, LOW);
      current = LED1;  
     }
    else if(current == LED1)
    {
      digitalWrite(32, LOW); 
      digitalWrite(33, HIGH); //led2 on
      digitalWrite(LED3, LOW); 
      digitalWrite(LED4, LOW);  
      current = LED2;  
     }
     else if(current == LED2)
    {
      digitalWrite(LED1, LOW); 
      digitalWrite(LED2, LOW); 
      digitalWrite(LED3, HIGH); //led3 on
      digitalWrite(LED4, LOW);  
      current = LED3; 
     }
     else if(current == LED3)
    {
      digitalWrite(LED1, LOW); 
      digitalWrite(LED2, LOW); 
      digitalWrite(LED3, LOW); 
      digitalWrite(LED4, HIGH); //led4 on
      current = LED4; 
     }
     delay(500); //0.5 seconds
  } 
  else //switch off
  {
    digitalWrite(LED1, HIGH); //led1 on
    digitalWrite(LED2, HIGH); //led2 on
    digitalWrite(LED3, HIGH); //led3 on
    digitalWrite(LED4, HIGH); //led4 on
    current = 0;
  }
}
