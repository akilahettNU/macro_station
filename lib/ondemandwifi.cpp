#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

// select which pin will trigger the configuration portal when set to LOW
#define TRIGGER_PIN 7
#define RGB_LED 48

int timeout = 120; // seconds to run for

void setup()
{
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("\n Starting");
    pinMode(TRIGGER_PIN, INPUT_PULLUP);
}

void loop()
{
    // is configuration portal requested?
    if (digitalRead(TRIGGER_PIN) == LOW)
    {
        WiFiManager wm;
        neopixelWrite(RGB_LED, 0, 0, 255 * 0.05);

        // reset settings - for testing
        wm.resetSettings();

        // set configportal timeout
        wm.setConfigPortalTimeout(timeout);

        if (!wm.startConfigPortal("OnDemandAP"))
        {
            Serial.println("failed to connect and hit timeout");
            delay(3000);
            // reset and try again, or maybe put it to deep sleep
            ESP.restart();
            delay(5000);
        }

        // if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
    }

    // put your main code here, to run repeatedly:
}