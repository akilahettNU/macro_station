#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

USBHIDKeyboard Keyboard;
#define WIFI_BUTTON 7
#define BUTTON_1 6

void setup()
{
    Serial.begin(115200);
    USB.begin();
    Keyboard.begin();
    delay(3000); // Wait for USB to initialize
    pinMode(WIFI_BUTTON, INPUT_PULLUP);
    pinMode(BUTTON_1, INPUT_PULLUP);
}

void loop()
{
    // Press Win + D
    if (digitalRead(WIFI_BUTTON) == 0)
    {
        Keyboard.press(KEY_LEFT_GUI); // Windows key
        delay(50);
        Keyboard.press('d'); // D key
        delay(100);
        Keyboard.releaseAll();

        Serial.println("Pressed Win + D");
    }
    if (digitalRead(BUTTON_1) == 0)
    {
        Keyboard.press(KEY_LEFT_GUI); // Windows key
        delay(50);
        Keyboard.press('p'); // D key
        delay(100);
        Keyboard.releaseAll();

        Serial.println("Pressed Win + P");
    }

    // delay(3000); // Wait before next press
}
