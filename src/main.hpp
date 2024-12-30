#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "A4988.h"
#include "LiquidCrystal_I2C.h"

// Syringe settings
#define SYRINGE_VOLUME 10        // [mL]
#define RADIUS 14.567 / 10 * 0.5 // [cm]
#define SYRINGE_LENGTH (10 / (PI * sq(RADIUS)))

// Lead screw settings
#define LEAD 0.8

// Motor settings
#define MOTOR_STEPS 200 // Motor steps per revolution.
#define MICROSTEPS 16
#define MAX_ROTATION ((int)SYRINGE_LENGTH / LEAD)
#define MAX_STEPS (MOTOR_STEPS * MICROSTEPS * MAX_ROTATION)
#define MOTOR_ACCEL 200
#define MOTOR_DECEL 200

// Pin settings
#define DIR 8
#define STEP 9
#define SLEEP 13
#define START_PIN 2
#define STOP_PIN 3
#define REVERSE_PIN 4
#define MODE_PIN 5
#define ENDSTOP_PIN 6

// Serial port settings
#define BAUD_RATE 115200

// Variables
extern A4988 stepper;
extern LiquidCrystal_I2C lcd;
extern float flowrate;
extern float rpm; // [/min]
extern int dir;
extern int pos;


// prototype function declarations:
void startPump();
void stopPump();
void reversePump();
void calcRPM(float floarate, float *rpm);
void setFlowrate(float flowrate);
void switchRun();

void serialCommand();

#endif // MAIN_H
