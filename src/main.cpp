#include <main.hpp>
// #define DEBUG

int dir;
int pos;
float rpm;
float flowrate;
bool stop_flag;
A4988 stepper(MOTOR_STEPS, DIR, STEP, SLEEP);
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

typedef enum
{
    Operaten,
    Settings,
    Others
} MODE;

MODE mode = Operaten;

void setup()
{
    Serial.begin(BAUD_RATE);

    pinMode(START_STOP_PIN, INPUT_PULLUP);
    pinMode(REVERSE_PIN, INPUT_PULLUP);
    pinMode(UP_PIN, INPUT_PULLUP);
    pinMode(DOWN_PIN, INPUT_PULLUP);
    pinMode(ENDSTOP_PIN, INPUT_PULLUP);

    dir = 1;
    flowrate = 10;
    stop_flag = false;
    calcRPM(flowrate, &rpm);
    stepper.begin(rpm, MICROSTEPS);

    lcd.init(); // initialize the lcd
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Forward");
    lcd.setCursor(0, 1);
    lcd.print(flowrate);
    lcd.print(" mL/min");

    // 原点位置を取得（エンドストップまで動かす）
    // findHomePosition()
}

void loop()
{
    static unsigned long previous_step = 0;
    unsigned long current_step = stepper.getStepsCompleted();

    if (current_step != previous_step)
    {
        previous_step = current_step;
        pos = (dir == 1) ? pos + 1 : pos - 1;
    }

    unsigned long next_action_interval = stepper.nextAction();

    if (next_action_interval > 100)
    {
        // lcd.clear();
        // lcd.print(next_action_interval);// 処理
    }

// button control
#define DEBOUNCE_DELAY 200
    static unsigned long last_button_press = 0;

    if ((millis() - last_button_press) > DEBOUNCE_DELAY)
    {
        if (digitalRead(START_STOP_PIN) == LOW)
        {
            if (stepper.getCurrentState() == stepper.STOPPED)
            {
                startPump();
            }
            else
            {
                stopPump();
            }
            last_button_press = millis();
        }

        if (digitalRead(REVERSE_PIN) == LOW)
        {
            reversePump();
            last_button_press = millis();
        }

        if (digitalRead(UP_PIN) == LOW)
        {
            unsigned long lastPressedTime = millis();
            while (digitalRead(UP_PIN) == LOW)
            {
                unsigned long currentTime = millis();

                if ((currentTime - lastPressedTime) > 2000)
                {
                    flowrate = flowrate + 10;
                }
                else if ((currentTime - lastPressedTime) > 1000)
                {
                    flowrate = flowrate + 1;
                }
                else
                {
                    flowrate = flowrate + 0.1;
                }
                setFlowrate(flowrate);
                delay(100);
            }
        }

        if (digitalRead(DOWN_PIN) == LOW)
        {
             last_button_press= millis();
            unsigned long lastPressedTime = millis();

            while (digitalRead(DOWN_PIN) == LOW)
            {
                unsigned long currentTime = millis();

                if ((currentTime - lastPressedTime) > 2000)
                {
                    flowrate = flowrate - 10;
                }
                else if ((currentTime - lastPressedTime) > 1000)
                {
                    flowrate = flowrate - 1;
                }
                else
                {
                    flowrate = flowrate - 0.1;
                }
                setFlowrate(flowrate);
                delay(100);
            }

        }

        if (digitalRead(ENDSTOP_PIN) == LOW)
        {
            stopPump();
        }
    }

    // Serial communication control
    if (Serial.available() > 0)
    {
        serialCommand();
    }

#ifdef DEBUG
    debugOut();
#endif
}
