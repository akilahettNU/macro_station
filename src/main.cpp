#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "html_content.h"

// Initialize keyboard
USBHIDKeyboard Keyboard;

// Define GPIO pins for matrix keyboard
#define ROW1 9
#define ROW2 10
#define ROW3 11
#define COL1 4
#define COL2 5

// Define GPIO pin for WiFi configuration
#define WIFI_BUTTON 7

void checkMatrixKeyboard();

// Web server code
void handleWebClient();

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Current key combinations for each button (default values)
char keyCombo[6] = {'d', 'e', 'p', 'r', 'l', 'm'};

// Time variables for web server
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

// WiFiManager portal timeout in seconds
int wifiPortalTimeout = 120;

// Debounce variables for buttons
unsigned long lastDebounceTime[6] = {0, 0, 0, 0, 0, 0};
const long debounceDelay = 300;

// Button states for debouncing
bool buttonPressed[6] = {false, false, false, false, false, false};

// Function to process template with placeholders
String processTemplate(const char *templ)
{
  String processed = String(templ);

  // Key combo placeholders for all 6 buttons
  for (int i = 0; i < 6; i++)
  {
    String buttonIndex = String(i + 1);
    processed.replace("%CURRENT_KEY_" + buttonIndex + "%", String(keyCombo[i]));
  }

  // Set the selected options in dropdowns for all 6 buttons
  for (int i = 0; i < 6; i++)
  {
    String buttonIndex = String(i + 1);
    String currentKey = String(keyCombo[i]);

    processed.replace("%SELECTED_D_" + buttonIndex + "%", keyCombo[i] == 'd' ? "selected" : "");
    processed.replace("%SELECTED_E_" + buttonIndex + "%", keyCombo[i] == 'e' ? "selected" : "");
    processed.replace("%SELECTED_P_" + buttonIndex + "%", keyCombo[i] == 'p' ? "selected" : "");
    processed.replace("%SELECTED_R_" + buttonIndex + "%", keyCombo[i] == 'r' ? "selected" : "");
    processed.replace("%SELECTED_L_" + buttonIndex + "%", keyCombo[i] == 'l' ? "selected" : "");
    processed.replace("%SELECTED_M_" + buttonIndex + "%", keyCombo[i] == 'm' ? "selected" : "");
  }

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

  // Initialize matrix keyboard pins
  pinMode(ROW1, INPUT_PULLDOWN);
  pinMode(ROW2, INPUT_PULLDOWN);
  pinMode(ROW3, INPUT_PULLDOWN);
  pinMode(COL1, OUTPUT);
  pinMode(COL2, OUTPUT);

  // Initialize WiFi button
  pinMode(WIFI_BUTTON, INPUT_PULLUP);

  // Set both columns to LOW initially
  digitalWrite(COL1, LOW);
  digitalWrite(COL2, LOW);

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

void performKeyCombo(char key)
{
  // Perform Win + key action
  Keyboard.press(KEY_LEFT_GUI); // Windows key
  delay(50);
  Keyboard.press(key);
  delay(100);
  Keyboard.releaseAll();

  Serial.print("Pressed Win + ");
  Serial.println(key);
}

void loop()
{
  // Check matrix keyboard
  checkMatrixKeyboard();

  // Web server code
  handleWebClient();
}

void checkMatrixKeyboard()
{
  // Check button 1 (Row 1, Col 1)
  digitalWrite(COL1, HIGH);
  delay(10);
  if (digitalRead(ROW1) == HIGH)
  {
    if (!buttonPressed[0] && (millis() - lastDebounceTime[0] > debounceDelay))
    {
      buttonPressed[0] = true;
      lastDebounceTime[0] = millis();
      performKeyCombo(keyCombo[0]);
    }
  }
  else
  {
    buttonPressed[0] = false;
  }
  digitalWrite(COL1, LOW);
  delay(10);

  // Check button 2 (Row 1, Col 2)
  digitalWrite(COL2, HIGH);
  delay(10);
  if (digitalRead(ROW1) == HIGH)
  {
    if (!buttonPressed[1] && (millis() - lastDebounceTime[1] > debounceDelay))
    {
      buttonPressed[1] = true;
      lastDebounceTime[1] = millis();
      performKeyCombo(keyCombo[1]);
    }
  }
  else
  {
    buttonPressed[1] = false;
  }
  digitalWrite(COL2, LOW);
  delay(10);

  // Check button 3 (Row 2, Col 1)
  digitalWrite(COL1, HIGH);
  delay(10);
  if (digitalRead(ROW2) == HIGH)
  {
    if (!buttonPressed[2] && (millis() - lastDebounceTime[2] > debounceDelay))
    {
      buttonPressed[2] = true;
      lastDebounceTime[2] = millis();
      performKeyCombo(keyCombo[2]);
    }
  }
  else
  {
    buttonPressed[2] = false;
  }
  digitalWrite(COL1, LOW);
  delay(10);

  // Check button 4 (Row 2, Col 2)
  digitalWrite(COL2, HIGH);
  delay(10);
  if (digitalRead(ROW2) == HIGH)
  {
    if (!buttonPressed[3] && (millis() - lastDebounceTime[3] > debounceDelay))
    {
      buttonPressed[3] = true;
      lastDebounceTime[3] = millis();
      performKeyCombo(keyCombo[3]);
    }
  }
  else
  {
    buttonPressed[3] = false;
  }
  digitalWrite(COL2, LOW);
  delay(10);

  // Check button 5 (Row 3, Col 1)
  digitalWrite(COL1, HIGH);
  delay(10);
  if (digitalRead(ROW3) == HIGH)
  {
    if (!buttonPressed[4] && (millis() - lastDebounceTime[4] > debounceDelay))
    {
      buttonPressed[4] = true;
      lastDebounceTime[4] = millis();
      performKeyCombo(keyCombo[4]);
    }
  }
  else
  {
    buttonPressed[4] = false;
  }
  digitalWrite(COL1, LOW);
  delay(10);

  // Check button 6 (Row 3, Col 2)
  digitalWrite(COL2, HIGH);
  delay(10);
  if (digitalRead(ROW3) == HIGH)
  {
    if (!buttonPressed[5] && (millis() - lastDebounceTime[5] > debounceDelay))
    {
      buttonPressed[5] = true;
      lastDebounceTime[5] = millis();
      performKeyCombo(keyCombo[5]);
    }
  }
  else
  {
    buttonPressed[5] = false;
  }
  digitalWrite(COL2, LOW);
  delay(10);
}

void handleWebClient()
{
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
            if (header.indexOf("GET /set-button") >= 0)
            {
              for (int i = 0; i < 6; i++)
              {
                String buttonParam = "button" + String(i + 1) + "=";
                if (header.indexOf(buttonParam) >= 0)
                {
                  int valueStart = header.indexOf(buttonParam) + buttonParam.length();
                  char newKey = header.charAt(valueStart);

                  if (newKey == 'd' || newKey == 'e' || newKey == 'p' ||
                      newKey == 'r' || newKey == 'l' || newKey == 'm')
                  {
                    keyCombo[i] = newKey;
                    Serial.print("Button ");
                    Serial.print(i + 1);
                    Serial.print(" key combo changed to Win + ");
                    Serial.println(keyCombo[i]);
                  }
                }
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