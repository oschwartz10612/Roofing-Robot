#include "Arduino.h"
#include <AccelStepper.h>
#include <MultiStepper.h>

AccelStepper stepper1(AccelStepper::DRIVER, 22, 23);
AccelStepper stepper2(AccelStepper::DRIVER, 26, 27);
AccelStepper stepper3(AccelStepper::DRIVER, 30, 31);

MultiStepper steppers;

long positions[3];

void setup() {
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
}

void loop() {

}
