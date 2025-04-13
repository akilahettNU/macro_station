#include "Arduino.h"

#define ROW1 3
#define COL1 4
#define COL2 5
#define COL3 6
#define COL4 7


void setup()
{
  Serial.begin(115200);
  pinMode(ROW1, INPUT_PULLDOWN);

  pinMode(COL1,OUTPUT);
  pinMode(COL2, OUTPUT);
  pinMode(COL3, OUTPUT);
  pinMode(COL4, OUTPUT);

  Serial.println("Starting");
}

void loop()
{
  digitalWrite(COL1,HIGH);
  delay(10);
  if(digitalRead(ROW1)==1)
  {
    Serial.println("Button 1 is pressed");
  }
  digitalWrite(COL1, LOW);
  delay(10);

  digitalWrite(COL2, HIGH);
  delay(10);
  if (digitalRead(ROW1) == 1)
  {
    Serial.println("Button 2 is pressed");
  }
  digitalWrite(COL2, LOW);
  delay(10);

  digitalWrite(COL3, HIGH);
  delay(10);
  if (digitalRead(ROW1) == 1)
  {
    Serial.println("Button 3 is pressed");
  }
  digitalWrite(COL3, LOW);
  delay(10);

  digitalWrite(COL4, HIGH);
  delay(10);
  if (digitalRead(ROW1) == 1)
  {
    Serial.println("Button 4 is pressed");
  }
  digitalWrite(COL4, LOW);
  delay(10);
}
