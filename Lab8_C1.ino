//6188089 Nattawipa Saetae

//Complete this file according to the FSM

#define STATE1 1
#define STATE2 2
#define STATE3 3
#define STATE4 4
#define STATE_END 100

int sensor1 = 34; //GPIO 34
int sensor2 = 35; //GPIO 35
int led1 = 12; //GPIO 12
int led2 = 14; //GPIO 14

unsigned char state = 4;

void setup() {
  pinMode (sensor1, INPUT);
  pinMode (sensor2, INPUT);
  pinMode (led1, OUTPUT);
  pinMode (led2, OUTPUT);
}

void loop() 
{
  switch(state) 
  {
    //case state 1
    case STATE1: 
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      if((digitalRead(sensor1)==LOW) && (digitalRead(sensor2)==HIGH))
        state = STATE2;
      else if((digitalRead(sensor1)==HIGH) && (digitalRead(sensor2)==LOW))
        state = STATE3;
      else if((digitalRead(sensor1)==HIGH) && (digitalRead(sensor2)==HIGH))
        state = STATE4;
      break;
    //.....to be continued
    //case state 2
    case STATE2:
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      if((digitalRead(sensor1)==HIGH) && (digitalRead(sensor2)==LOW))
        state = STATE3;
      else if((digitalRead(sensor1)==HIGH) && (digitalRead(sensor2)==HIGH))
        state = STATE4;
      else if((digitalRead(sensor1)==LOW) && (digitalRead(sensor2)==LOW))
        state = STATE1;
      break;
    //case state 3
    case STATE3: 
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
      if((digitalRead(sensor1)==HIGH) && (digitalRead(sensor2)==HIGH))
        state = STATE4;
      else if((digitalRead(sensor1)==LOW) && (digitalRead(sensor2)==LOW))
        state = STATE1;
      else if((digitalRead(sensor1)==LOW) && (digitalRead(sensor2)==HIGH))
        state = STATE2;
      break;
    //case state 4
    case STATE4: //case state 1
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      if((digitalRead(sensor1)==LOW) && (digitalRead(sensor2)==LOW))
        state = STATE1;
      else if((digitalRead(sensor1)==LOW) && (digitalRead(sensor2)==HIGH))
        state = STATE2;
      else if((digitalRead(sensor1)==HIGH) && (digitalRead(sensor2)==LOW))
        state = STATE3;
      break;
  }
}
