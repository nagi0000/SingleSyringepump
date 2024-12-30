#include "main.hpp"

    int splitString(String input, char delimiter, int arraySize, String outputArray[]);

    
void serialCommand()
{
    String recievedData = Serial.readStringUntil('\n'); // 改行まで読込

    char delimiter = ' ';
    int maxWords = 5;
    String words[maxWords];
    int wordCount = splitString(recievedData, delimiter, maxWords, words);

    if (wordCount > 0)
    {
        if (words[0] == "Start")
        {
            startPump();
        }
        else if (words[0] == "Stop")
        {
            stopPump();
        }
        else if (words[0] == "Set")
        {
            setFlowrate(words[1].toFloat());
        }
        else if (words[0] == "Reverse")
        {
            reversePump();
        }
        else
        {
            Serial.println("unkonwn command");
        }
    }
}

void debugOutput()
{
    const unsigned long interval = 500; // シリアル通信の間隔 (ミリ秒)
    static unsigned long previousMillis = 0;   // 前回のシリアル通信の時間を記録
    unsigned long currentMillis = millis();

    // 一定間隔が経過した場合にシリアル通信を行う
    if ((currentMillis - previousMillis) >= interval)
    {
        previousMillis = currentMillis;
        // Serial.print("motorX_state:");
        // Serial.println(stepperX.getCurrentState());
        // Serial.print("motorY_state:");
        // Serial.println(stepperY.getCurrentState());
        Serial.print(">pos:");
        Serial.println(pos);
        // Serial.print(">step_remaining:");
        // Serial.println(steps_remaining);
        Serial.print(">rpm:");
        Serial.println(rpm);
    }
}


// Split a string by a delimiter
int splitString(String input, char delimiter, int arraySize, String outputArray[])
{
    int startIndex = 0;
    int endIndex = 0;
    int wordCount = 0;

    while ((endIndex = input.indexOf(delimiter, startIndex)) != -1)
    {
        if (wordCount >= arraySize)
        {
            break;
        }
        outputArray[wordCount] = input.substring(startIndex, endIndex);
        wordCount++;
        startIndex = endIndex + 1;
    }

    // Add the last segment (after the last delimiter) only if there's room
    if (wordCount < arraySize)
    {
        outputArray[wordCount] = input.substring(startIndex);
        wordCount++;
    }

    return wordCount;
}