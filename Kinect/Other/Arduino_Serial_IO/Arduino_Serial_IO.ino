/*
Code to control the Kinect turret rotation motor and provide feedback about
the sensor's yaw angle. The pinmap is:

2 - Beam sensor / index pulse
3 - Encoder input (A or B phase)
4 - Motor Driver PWM output
5 - Motor speed LED
6 - Enable switch
13- Enabled indicator
A0- Speed control potentiometer

Written 9-20-2015, Brian Duemmer
*/

#include <Servo.h>

int servo_speed = 90;
volatile unsigned long usec_time = micros();
volatile unsigned long turret_pos = 0;
volatile byte at_zero = 0;

Servo motor;


void setup() {

  // Initialize interrupts, motors, and serial connection
  Serial.begin(9600);

  motor.attach(4);
  
  attachInterrupt(0, zero_hit, RISING);
  attachInterrupt(1, enc_count, RISING);

  pinMode(6, INPUT_PULLUP);
  pinMode(13, OUTPUT);
}

void loop() {
  
//Control output motor, make sure switch is pressed
  
  if(digitalRead(6)==LOW) {
    servo_speed = map(analogRead(A0), 0, 1023, 90 , 180);
  }

  else {
    servo_speed = 90;
  }

  analogWrite(5, map(servo_speed, 90, 180, 0, 255));
  digitalWrite(13, !digitalRead(6));
     
  // format serial output packet
  Serial.print(String(usec_time));
  Serial.print('_');
  Serial.print(String(turret_pos));
  Serial.print('_');
  Serial.print(String(servo_speed));
  Serial.print('_');
  Serial.println(String(at_zero));
  delay(100);
  
}

// Interrupt service routines
void zero_hit() {

  turret_pos = 0;
  usec_time = micros();
  at_zero = 1;
}

void enc_count() {

  turret_pos++ ;
  usec_time = micros();
  at_zero = 0;
}
