//6188089 - Nattawipa Saetae
#define LDR 12

#define RED 25
#define YELLOW 26

#define PWM_CH_1 0
#define PWM_CH_2 1

#define PWM_FREQ 15000
#define PWM_RES 8 // Resolution in bits
#define MAX 255

void setup() 
{
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);

  ledcSetup(PWM_CH_1, PWM_FREQ, PWM_RES);
  ledcSetup(PWM_CH_2, PWM_FREQ, PWM_RES);

  ledcAttachPin(RED, PWM_CH_1);
  ledcAttachPin(YELLOW, PWM_CH_2);

  Serial.begin(9600);
}

void loop() 
{
  float LED = analogRead(LDR);
  float value = (LED/4096) * MAX;
  
  ledcWrite(PWM_CH_1, value);
  ledcWrite(PWM_CH_2, value);
  
  delay(50);
}
