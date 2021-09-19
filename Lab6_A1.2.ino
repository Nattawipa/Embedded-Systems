//6188089 - Nattawipa Saetae

#define POT 33
#define RED 25
#define GREEN 26
#define BLUE 27

#define PWM_CH_1 0
//#define PWM_CH_2 1
#define PWM_CH_2 10000
//#define PWM_CH_3 2
#define PWM_CH_3 8000

#define PWM_FREQ 15000
#define PWM_RES 16 // Change resolution in bits from 12 to 16

int aValue;

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  ledcSetup(PWM_CH_1, PWM_FREQ, PWM_RES);
  ledcSetup(PWM_CH_2, PWM_FREQ, PWM_RES);
  ledcSetup(PWM_CH_3, PWM_FREQ, PWM_RES);

  ledcAttachPin(RED, PWM_CH_1);
  ledcAttachPin(GREEN, PWM_CH_2);
  ledcAttachPin(BLUE, PWM_CH_3);
}

void loop() {
  aValue = analogRead(POT); //12 bits ADC

  ledcWrite(PWM_CH_1, aValue);
  delay(250);
  ledcWrite(PWM_CH_2, aValue);
  delay(250);
  ledcWrite(PWM_CH_3, aValue);
  delay(250);
}
