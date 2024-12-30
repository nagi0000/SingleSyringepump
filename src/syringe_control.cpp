#include "main.hpp"

// put function definitions here:
void startPump()
{
    stepper.startMove(dir * MAX_STEPS);
    stepper.enable();
}

void stopPump()
{
    stepper.stop();
    stepper.disable();
}

void setFlowrate(float set_value)
{
    flowrate = set_value;
    calcRPM(flowrate, &rpm);
    stepper.setRPM(rpm);

    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(flowrate);
    lcd.print(" mL/min");
}

void reversePump()
{
    dir = -dir;

    lcd.clear();
    lcd.setCursor(0, 0);
    if (dir > 0)
    {
        lcd.print("Forward");
    }
    else
    {
        lcd.print("Backward");
    }
    lcd.setCursor(0, 1);
    lcd.print(flowrate);
    lcd.print(" mL/min");

    startPump();
}

void switchRun()
{
    if (stepper.getCurrentState() == stepper.STOPPED)
    {
        startPump();
    }
    else
    {
        stopPump();
    }
}

void calcRPM(float flowrate, float *rpm)
{
    *rpm = (flowrate / (PI * sq(RADIUS))) / LEAD;
}