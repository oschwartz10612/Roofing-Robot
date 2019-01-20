#include "Arduino.h"
#include <AccelStepper.h>
#include <MultiStepper.h>

#include <SabertoothSimplified.h>

SabertoothSimplified ST; // We'll name the Sabertooth object ST.
                         // For how to configure the Sabertooth, see the DIP Switch Wizard for
                         //   http://www.dimensionengineering.com/datasheets/SabertoothDIPWizard/start.htm
                         // Be sure to select Simplified Serial Mode for use with this library.
                         // This sample uses a baud rate of 9600.
                         //
                         // Connections to make:
                         //   Arduino TX->1  ->  Sabertooth S1
                         //   Arduino GND    ->  Sabertooth 0V
                         //   Arduino VIN    ->  Sabertooth 5V (OPTIONAL, if you want the Sabertooth to power the Arduino)
                         //
                         // If you want to use a pin other than TX->1, see the SoftwareSerial example.

AccelStepper stepper1(AccelStepper::DRIVER, 22, 23);
AccelStepper stepper2(AccelStepper::DRIVER, 26, 27);
AccelStepper stepper3(AccelStepper::DRIVER, 30, 31);

MultiStepper steppers;

long positions[3];
int timeout1 = 1000;
int timeout2 = -5000;
int power = -18;
boolean toggle = false;

#define SPEED 35
#define PULSEWIDTH 60
#define ACCELERATION 10
#define MTR1RELAY 44
#define MTR2RELAY 45
#define BUTTON 46
#define BUTTONGND 47
#define SENSOR 0

void enableBreak() {
  digitalWrite(MTR1RELAY, HIGH);
  digitalWrite(MTR2RELAY, HIGH);
}

void disableBreak() {
  digitalWrite(MTR1RELAY, LOW);
  digitalWrite(MTR2RELAY, LOW);
}

void ejectTile() {
  //Serial.begin(9600); //for debugging

  stepper1.enableOutputs();
  stepper2.enableOutputs();
  stepper3.enableOutputs();


  stepper1.setSpeed(SPEED);
  stepper2.setSpeed(SPEED);
  stepper3.setSpeed(SPEED);

  while (analogRead(SENSOR) > 50) {
    stepper1.runSpeed();
    stepper2.runSpeed();
    stepper3.runSpeed();
    if (digitalRead(BUTTON) == LOW) {
      ST.motor(1, 0);
      ST.motor(2, 0);
      enableBreak();
      toggle = false;
      break;
    }
    delay(1);
  }

  for (size_t i = 0; i < 1000; i++) {
    stepper1.runSpeed();
    stepper2.runSpeed();
    stepper3.runSpeed();
    delay(1);
  }

  disableBreak();
  ST.motor(1, power);
  ST.motor(2, power);

  timeout1 = 4000;
  timeout2 = 8000;
  while (analogRead(SENSOR) < 300) {
    if (timeout1 != 0) {
      stepper1.runSpeed();
      timeout1--;
    }
    if (timeout2 != 0) {
      stepper2.runSpeed();
      timeout2--;
    }
    stepper3.runSpeed();
    if (digitalRead(BUTTON) == LOW) {
      ST.motor(1, 0);
      ST.motor(2, 0);
      enableBreak();
      toggle = false;
      break;
    }
    delay(1);
  }

  delay(1000);

  ST.motor(1, 0);
  ST.motor(2, 0);
  enableBreak();

  stepper1.disableOutputs();
  stepper2.disableOutputs();
  stepper3.disableOutputs();
}

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(BUTTONGND, OUTPUT);
  pinMode(MTR1RELAY, OUTPUT);
  pinMode(MTR2RELAY, OUTPUT);

  digitalWrite(BUTTONGND, LOW);

  enableBreak();

  SabertoothTXPinSerial.begin(9600);

  stepper1.setMaxSpeed(SPEED);
  stepper1.setAcceleration(ACCELERATION);
  stepper1.setEnablePin(25);
  stepper1.setMinPulseWidth(PULSEWIDTH);

  stepper2.setMaxSpeed(SPEED);
  stepper2.setAcceleration(ACCELERATION);
  stepper2.setEnablePin(29);
  stepper2.setMinPulseWidth(PULSEWIDTH);

  stepper3.setMaxSpeed(SPEED);
  stepper3.setAcceleration(ACCELERATION);
  stepper3.setEnablePin(33);
  stepper3.setMinPulseWidth(PULSEWIDTH);

  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  steppers.addStepper(stepper3);
}

void loop() {
if (digitalRead(BUTTON) == LOW) {
    delay(2000);
    if (digitalRead(BUTTON) == LOW) {
      while (digitalRead(BUTTON) == LOW) {
        disableBreak();
        ST.motor(1, 70);
        ST.motor(2, 70);
      }
      ST.motor(1, 0);
      ST.motor(2, 0);
      enableBreak();
    }else {
      if (toggle == true) {
        toggle = false;
      } else {
        toggle = true;
      }
    }
  }
  if (toggle == true) {
    ejectTile();
  }

  delay(10);
}
