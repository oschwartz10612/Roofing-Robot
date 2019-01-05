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

#define POWER 50
#define SPEED 35
#define PULSEWIDTH 60
#define ACCELERATION 10
#define MTR1RELAY 35
#define MTR2RELAY 36
#define RELAYGND 34
#define RELAY5V 37
#define BUTTON 38
#define BUTTONGND 39

void ejectTile() {
  positions[0] = 150;
  positions[1] = 370;
  positions[2] = 470;

  stepper1.enableOutputs();
  stepper2.enableOutputs();
  stepper3.enableOutputs();

  steppers.moveTo(positions);

  stepper1.setSpeed(SPEED);
  stepper2.setSpeed(SPEED);
  stepper3.setSpeed(SPEED);

  steppers.runSpeedToPosition(); // Blocks until all are in position

  stepper1.disableOutputs();
  stepper2.disableOutputs();
  stepper3.disableOutputs();

  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);
  stepper3.setCurrentPosition(0);
}

void enableDriveMotors() {
  digitalWrite(MTR1RELAY, HIGH);
  digitalWrite(MTR2RELAY, HIGH);
}

void disableDriveMotors() {
  digitalWrite(MTR1RELAY, LOW);
  digitalWrite(MTR2RELAY, LOW);
}

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(RELAY5V, OUTPUT);
  pinMode(RELAYGND, OUTPUT);
  pinMode(BUTTON5V, OUTPUT);
  pinMode(MTR1RELAY, OUTPUT);
  pinMode(MTR2RELAY, OUTPUT);

  digitalWrite(RELAY5V, HIGH);
  digitalWrite(RELAYGND, LOW);
  digitalWrite(BUTTONGND, LOW);

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
    ST.motor(1, POWER);
    ST.motor(2, POWER);
    ejectTile();
    delay(1000);
    ST.motor(1, 0);
    ST.motor(2, 0);
  }
}
