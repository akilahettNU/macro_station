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
#define ACTION_BUTTON 5

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

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
  pinMode(ACTION_BUTTON, INPUT_PULLUP);

  // Set WiFi to station mode
  WiFi.mode(WIFI_STA);

  // Initialize WiFiManager
  WiFiManager wm;

  // Check if WiFi provisioning button is pressed during boot
  if (digitalRead(WIFI_BUTTON) == LOW)
  {
    Serial.println("WiFi button pressed - starting portal");
    // Start portal with timeout
    wm.setConfigPortalTimeout(wifiPortalTimeout);
    wm.startConfigPortal("ESP32_HID_Controller");
  }
  else
  {
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
  // Check if Action button is pressed
  if (digitalRead(ACTION_BUTTON) == LOW)
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
      {                         // if there's bytes to read from the client,
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

            // Handle button key combination change
            if (header.indexOf("GET /set-button?keycombo=") >= 0)
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
        {                   // if you got anything else but a carriage return character,
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