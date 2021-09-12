//6188089 - Nattawipa Saetae
#define RED 25
#define BLUE 26

#define PWM_CH_1 0
#define PWM_CH_2 1

#define PWM_FREQ 15000
#define PWM_RES 8 // Resolution in bits
#define MAX 255

int redLevel, blueLevel;
bool redUp, blueUp;

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);

  ledcSetup(PWM_CH_1, PWM_FREQ, PWM_RES);
  ledcSetup(PWM_CH_2, PWM_FREQ, PWM_RES);

  ledcAttachPin(RED, PWM_CH_1);
  ledcAttachPin(BLUE, PWM_CH_2);

  redLevel = MAX;
  blueLevel = 0;
  redUp = false;
  blueUp = true;
}

void loop() {
  if(redUp){
    ledcWrite(PWM_CH_1, redLevel++);
    if(redLevel == MAX){
      redUp = false;
    }
  }
  else{
    ledcWrite(PWM_CH_1, redLevel--);
    if(redLevel == 0){
      redUp = true;
    }
  }

  if(blueUp){
    ledcWrite(PWM_CH_2, blueLevel++);
    if(blueLevel == MAX){
      blueUp = false;
    }
  }
  else{
    ledcWrite(PWM_CH_2, blueLevel--);
    if(blueLevel == 0){
      blueUp = true;
    }
  }
  delay(50);
}
