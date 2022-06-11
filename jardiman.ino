/*
  ESP8266 BlinkWithoutDelay by Simon Peter
  Blink the blue LED on the ESP-01 module
  Based on the Arduino Blink without Delay example
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/
#include <Task.h>

#define TRIG 12
#define ECHO 14
#define GET_DISTANCE_PERIOD 2
void get_distance();
Task get_distance_task(
  GET_DISTANCE_PERIOD,
  get_distance,
  true
);


#define ADC_PIN A0
#define ADC_PERIOD 1
void get_analog_value();
Task adc_task(ADC_PERIOD, get_analog_value, true);


#define BLINK_PERIOD 3
bool ledState = false;
void blink_f();
Task blink_task(BLINK_PERIOD, blink_f, true);

#define BLINK2_PERIOD 2
bool led2State = false;
#define LED2 16
void blink2_f();
Task blink2_task(BLINK2_PERIOD, blink2_f, true);

#define BLINK3_PERIOD 1
bool led3State = false;
#define LED3 5
void blink3_f();
Task blink3_task(BLINK3_PERIOD, blink3_f, true);

void setup() {
  Serial.begin(115200);

  Serial.println("Configuring Pins");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  Serial.println("Configuring Ultrasonic Pins");
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);

  Serial.println("Starting Program");
}

void loop() {
  blink_task.run();
  blink2_task.run();
  blink3_task.run();
  get_distance_task.run();
  adc_task.run();
}

//Tasks
void get_analog_value() {
  int sensor_value;
  float volt;
  sensor_value = analogRead(ADC_PIN);
  Serial.print("Analog Value = ");
  //Serial.println(sensor_value);
  volt = sensor_value * (3.3/1024);
  Serial.print(sensor_value);//893 es un buen valor para el cual la bateria esta descargada
  Serial.println("V");
}

void blink_f() {
  ledState = !ledState;
  digitalWrite(LED_BUILTIN, ledState);
}

void blink2_f() {
  led2State = !led2State;
  digitalWrite(LED2, led2State);
}

void blink3_f() {
  led3State = !led3State;
  digitalWrite(LED3, led3State);
}

void get_distance() {
  float distance;
  float duration;

  // Reading distance sequence
  digitalWrite(TRIG, 0);
  delayMicroseconds(4);
  digitalWrite(TRIG, 1);
  delayMicroseconds(10);
  digitalWrite(TRIG, 0);

  duration = pulseIn(ECHO, HIGH);
  duration /= 2.0;
  distance = duration / 29.2;


  Serial.print("distance = ");
  Serial.print(distance);
  Serial.println("cm");
  delay(50);

}
