#include <Arduino.h>
#define RGB_LED 48

float brightness = 0.05; // Adjust brightness here (0.0 to 1.0)

void setup()
{
    pinMode(48, OUTPUT);
}

void loop()
{

    neopixelWrite(RGB_LED, 255 * brightness, 0, 0);

    delay(1000);

    neopixelWrite(RGB_LED, 0 * brightness, 0, 255 * brightness);

    delay(1000);
}
