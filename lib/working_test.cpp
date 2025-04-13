#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

USBHIDKeyboard Keyboard;

// Pin definitions
#define WIFI_BUTTON 7 // Button to trigger WiFi config portal
#define BUTTON_1 6    // Button for Win+P shortcut

// WiFi config portal settings
int timeout = 120; // Seconds to run config portal

void setup()
{
    // Initialize serial
    Serial.begin(115200);
    Serial.println("\n Starting");

    // Initialize USB HID
    USB.begin();
    Keyboard.begin();

    // Set WiFi mode
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

    // Configure pins
    pinMode(WIFI_BUTTON, INPUT_PULLUP);
    pinMode(BUTTON_1, INPUT_PULLUP);

    delay(1000); // Wait for USB to initialize
}

void loop()
{
    // Check if WiFi configuration portal is requested
    if (digitalRead(WIFI_BUTTON) == LOW)
    {
        Serial.println("WiFi button pressed - starting config portal");
        neopixelWrite(RGB_LED, 0 * brightness, 0, 255 * brightness);
        WiFiManager wm;

        // Set config portal timeout
        wm.setConfigPortalTimeout(timeout);

        // Start config portal
        if (!wm.startConfigPortal("KeyboardAP"))
        {
            Serial.println("Failed to connect and hit timeout");
            delay(3000);
            // Reset and try again
            ESP.restart();
            delay(5000);
        }

        // If we get here, we've connected to WiFi
        Serial.println("Connected to WiFi successfully!");

        // Debounce
        while (digitalRead(WIFI_BUTTON) == LOW)
        {
            delay(10);
        }
        delay(300);
    }

    // Check if Button 1 is pressed
    if (digitalRead(BUTTON_1) == LOW)
    {
        Keyboard.press(KEY_LEFT_GUI); // Windows key
        delay(50);
        Keyboard.press('p'); // P key
        delay(100);
        Keyboard.releaseAll();

        Serial.println("Pressed Win + P");

        // Debounce
        while (digitalRead(BUTTON_1) == LOW)
        {
            delay(10);
        }
        delay(300);
    }
}