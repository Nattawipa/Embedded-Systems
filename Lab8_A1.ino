//6188089 Nattawipa Saetae

#include <ESP32Servo.h>

#define SERVO_PIN 23

Servo sv;

int minUs = 1000;
int maxUs = 2000;

int pos = 0;      // position in degrees
//ESP32PWM pwm;

void setup() {
  Serial.begin(9600);
  sv.setPeriodHertz(50);      // Standard 50hz servo
}

void loop() {
  sv.attach(SERVO_PIN);

  for (pos = 0; pos <= 180; pos += 1) { // sweep from 0 degrees to 180 degrees
    // in steps of 1 degree
    sv.write(pos);
    delay(10);             // waits 20ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // sweep from 0 degrees to 180 degrees
    // in steps of 1 degree
    sv.write(pos);
    delay(10);             // waits 20ms for the servo to reach the position
  }

  sv.detach();
  delay(500);
}
