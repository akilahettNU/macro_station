#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager
#include "html_content.h" // Include the HTML header file

// For ESP32-S3, define the appropriate pins
#define TRIGGER_PIN 7 // Use the appropriate pin for your board to trigger config portal
#define RGB_LED 48    // Adjust if your board uses a different pin for the LED

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

int timeout = 120; // seconds to run for

// Auxiliary variables to store the current output state
String output12State = "off";
String output14State = "off";

// Assign output variables to GPIO pins - adjust if needed for your board
const int output12 = 4;
const int output14 = 14;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
// WiFiManager portal timeout in seconds
int wifiPortalTimeout = 120;

// Function to process template with placeholders
String processTemplate(const char *templ)
{
    String processed = String(templ);
    processed.replace("%OUTPUT12_STATE%", output12State);
    processed.replace("%OUTPUT14_STATE%", output14State);
    processed.replace("%BUTTON12_CLASS%", output12State == "off" ? "button" : "button button2");
    processed.replace("%BUTTON14_CLASS%", output14State == "off" ? "button" : "button button2");
    processed.replace("%BUTTON12_TEXT%", output12State == "off" ? "ON" : "OFF");
    processed.replace("%BUTTON14_TEXT%", output14State == "off" ? "ON" : "OFF");
    processed.replace("%BUTTON12_ACTION%", output12State == "off" ? "/12/on" : "/12/off");
    processed.replace("%BUTTON14_ACTION%", output14State == "off" ? "/14/on" : "/14/off");
    return processed;
}

void setup()
{
    Serial.begin(115200);
    Serial.println("\n Starting");

    // Initialize the output variables as outputs
    pinMode(output12, OUTPUT);
    pinMode(output14, OUTPUT);
    pinMode(TRIGGER_PIN, INPUT_PULLUP);

    // Set outputs to LOW
    digitalWrite(output12, LOW);
    digitalWrite(output14, LOW);

    // Set WiFi to station mode
    WiFi.mode(WIFI_STA);

    // Initialize WiFiManager
    WiFiManager wm;

    // Auto-connect using saved credentials, if they exist
    // If connection fails, it starts an access point with the specified name
    // and goes into a blocking loop awaiting configuration
    if (!wm.autoConnect("ESP32_AP"))
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
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Start the server
    server.begin();
}

void loop()
{
    // Check if config mode is requested via button press
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
        neopixelWrite(RGB_LED, 0, 255 * 0.05, 0);
    }

    // Regular web server code
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

                        // turns the GPIOs on and off
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