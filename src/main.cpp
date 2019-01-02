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

void ejectTile() {
  positions[0] = 150;
  positions[1] = 370;
  positions[2] = 470;

  stepper1.enableOutputs();
  stepper2.enableOutputs();
  stepper3.enableOutputs();

  steppers.moveTo(positions);

  stepper1.setSpeed(35);
  stepper2.setSpeed(35);
  stepper3.setSpeed(35);

  steppers.runSpeedToPosition(); // Blocks until all are in position

  stepper1.disableOutputs();
  stepper2.disableOutputs();
  stepper3.disableOutputs();

  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);
  stepper3.setCurrentPosition(0);
}

void setup() {
  SabertoothTXPinSerial.begin(9600);
  stepper1.setMaxSpeed(35);
  stepper1.setAcceleration(10);
  stepper1.setEnablePin(25);
  stepper1.setMinPulseWidth(60);

  stepper2.setMaxSpeed(35);
  stepper2.setAcceleration(10);
  stepper2.setEnablePin(29);
  stepper2.setMinPulseWidth(60);

  stepper3.setMaxSpeed(35);
  stepper3.setAcceleration(10);
  stepper3.setEnablePin(33);
  stepper3.setMinPulseWidth(60);

  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  steppers.addStepper(stepper3);

  ST.motor(1, POWER);
  ST.motor(2, POWER);
  ejectTile();
  delay(1000);
  ST.motor(1, 0);
  ST.motor(2, 0);
}

void loop() {
  //code executed in setup()
}
