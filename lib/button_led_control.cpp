#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "html_content.h"

// Initialize keyboard
USBHIDKeyboard Keyboard;

// Define GPIO pins
#define WIFI_BUTTON 7
#define BUTTON_1 5

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliary variables to store the current output state
String output12State = "off";
String output14State = "off";

// Assign output variables to GPIO pins
const int output12 = 4;
const int output14 = 14;

// Current key combination - default to 'd' (Win+D)
char currentKeyCombo = 'd';

// Time variables for web server
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

// WiFiManager portal timeout in seconds
int wifiPortalTimeout = 120;

// Debounce variables for buttons
unsigned long lastDebounceTime = 0;
const long debounceDelay = 300;

// Function to process template with placeholders
String processTemplate(const char *templ)
{
    String processed = String(templ);

    // GPIO placeholders
    processed.replace("%OUTPUT12_STATE%", output12State);
    processed.replace("%OUTPUT14_STATE%", output14State);
    processed.replace("%BUTTON12_CLASS%", output12State == "off" ? "button" : "button button2");
    processed.replace("%BUTTON14_CLASS%", output14State == "off" ? "button" : "button button2");
    processed.replace("%BUTTON12_TEXT%", output12State == "off" ? "ON" : "OFF");
    processed.replace("%BUTTON14_TEXT%", output14State == "off" ? "ON" : "OFF");
    processed.replace("%BUTTON12_ACTION%", output12State == "off" ? "/12/on" : "/12/off");
    processed.replace("%BUTTON14_ACTION%", output14State == "off" ? "/14/on" : "/14/off");

    // Key combo placeholders
    processed.replace("%CURRENT_KEY%", String(currentKeyCombo));

    // Set the selected option in dropdown
    processed.replace("%SELECTED_D%", currentKeyCombo == 'd' ? "selected" : "");
    processed.replace("%SELECTED_E%", currentKeyCombo == 'e' ? "selected" : "");
    processed.replace("%SELECTED_P%", currentKeyCombo == 'p' ? "selected" : "");
    processed.replace("%SELECTED_R%", currentKeyCombo == 'r' ? "selected" : "");
    processed.replace("%SELECTED_L%", currentKeyCombo == 'l' ? "selected" : "");

    return processed;
}

void setup()
{
    Serial.begin(115200);
    Serial.println("\nStarting");

    // Initialize USB and keyboard
    USB.begin();
    Keyboard.begin();
    delay(1000); // Wait for USB to initialize

    // Initialize pins
    pinMode(WIFI_BUTTON, INPUT_PULLUP);
    pinMode(BUTTON_1, INPUT_PULLUP);
    pinMode(output12, OUTPUT);
    pinMode(output14, OUTPUT);

    // Set outputs to LOW
    digitalWrite(output12, LOW);
    digitalWrite(output14, LOW);

    // Set WiFi to station mode
    WiFi.mode(WIFI_STA);

    // Initialize WiFiManager
    WiFiManager wm;

    // Auto-connect using saved credentials, if they exist
    // If connection fails, it starts an access point with the specified name
    if (!wm.autoConnect("ESP32_HID_Controller"))
    {
        Serial.println("Failed to connect and hit timeout");
        delay(3000);
        // Reset and try again
        ESP.restart();
        delay(5000);
    }

    // If you get here, you have connected to WiFi
    Serial.println("Connected to WiFi");

    // Print local IP address
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Start the server
    server.begin();
}

void loop()
{
    // Check if WiFi config button is pressed
    if (digitalRead(WIFI_BUTTON) == LOW)
    {
        if (millis() - lastDebounceTime > debounceDelay)
        {
            lastDebounceTime = millis();

            // Perform Win + D action (or whatever is currently configured)
            Keyboard.press(KEY_LEFT_GUI); // Windows key
            delay(50);
            Keyboard.press(currentKeyCombo);
            delay(100);
            Keyboard.releaseAll();

            Serial.print("Pressed Win + ");
            Serial.println(currentKeyCombo);
        }
    }

    // Check if Button 1 is pressed
    if (digitalRead(BUTTON_1) == LOW)
    {
        if (millis() - lastDebounceTime > debounceDelay)
        {
            lastDebounceTime = millis();

            // Perform Win + key action
            Keyboard.press(KEY_LEFT_GUI); // Windows key
            delay(50);
            Keyboard.press(currentKeyCombo);
            delay(100);
            Keyboard.releaseAll();

            Serial.print("Pressed Win + ");
            Serial.println(currentKeyCombo);
        }
    }

    // Web server code
    WiFiClient client = server.available(); // Listen for incoming clients

    if (client)
    { // If a new client connects,
        currentTime = millis();
        previousTime = currentTime;
        Serial.println("New Client."); // print a message out in the serial port
        String currentLine = "";       // make a String to hold incoming data from the client

        while (client.connected() && currentTime - previousTime <= timeoutTime)
        { // loop while the client's connected
            currentTime = millis();
            if (client.available())
            {                           // if there's bytes to read from the client,
                char c = client.read(); // read a byte, then
                Serial.write(c);        // print it out the serial monitor
                header += c;
                if (c == '\n')
                { // if the byte is a newline character
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0)
                    {
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();

                        // Process URL actions
                        if (header.indexOf("GET /12/on") >= 0)
                        {
                            Serial.println("GPIO 12 on");
                            output12State = "on";
                            digitalWrite(output12, HIGH);
                        }
                        else if (header.indexOf("GET /12/off") >= 0)
                        {
                            Serial.println("GPIO 12 off");
                            output12State = "off";
                            digitalWrite(output12, LOW);
                        }
                        else if (header.indexOf("GET /14/on") >= 0)
                        {
                            Serial.println("GPIO 14 on");
                            output14State = "on";
                            digitalWrite(output14, HIGH);
                        }
                        else if (header.indexOf("GET /14/off") >= 0)
                        {
                            Serial.println("GPIO 14 off");
                            output14State = "off";
                            digitalWrite(output14, LOW);
                        }
                        // Handle button key combination change
                        else if (header.indexOf("GET /set-button?keycombo=") >= 0)
                        {
                            int valueStart = header.indexOf("keycombo=") + 9;
                            char newKey = header.charAt(valueStart);

                            if (newKey == 'd' || newKey == 'e' || newKey == 'p' || newKey == 'r' || newKey == 'l')
                            {
                                currentKeyCombo = newKey;
                                Serial.print("Button key combo changed to Win + ");
                                Serial.println(currentKeyCombo);
                            }
                        }

                        // Process and send the HTML template
                        String html = processTemplate(INDEX_HTML);
                        client.println(html);

                        // The HTTP response ends with another blank line
                        client.println();
                        // Break out of the while loop
                        break;
                    }
                    else
                    { // if you got a newline, then clear currentLine
                        currentLine = "";
                    }
                }
                else if (c != '\r')
                {                     // if you got anything else but a carriage return character,
                    currentLine += c; // add it to the end of the currentLine
                }
            }
        }
        // Clear the header variable
        header = "";
        // Close the connection
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
    }
}