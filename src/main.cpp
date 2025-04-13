#include "Arduino.h"

#define ROW1 9
#define ROW2 10
#define ROW3 11

#define COL1 4
#define COL2 5


void setup()
{
  Serial.begin(115200);
  pinMode(ROW1, INPUT_PULLDOWN);
  pinMode(ROW2, INPUT_PULLDOWN);
  pinMode(ROW3, INPUT_PULLDOWN);

  pinMode(COL1,OUTPUT);
  pinMode(COL2, OUTPUT);

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


  digitalWrite(COL1, HIGH);
  delay(10);
  if (digitalRead(ROW2) == 1)
  {
    Serial.println("Button 3 is pressed");
  }
  digitalWrite(COL1, LOW);
  delay(10);

  digitalWrite(COL2, HIGH);
  delay(10);
  if (digitalRead(ROW2) == 1)
  {
    Serial.println("Button 4 is pressed");
  }
  digitalWrite(COL2, LOW);
  delay(10);

  digitalWrite(COL1, HIGH);
  delay(10);
  if (digitalRead(ROW3) == 1)
  {
    Serial.println("Button 5 is pressed");
  }
  digitalWrite(COL1, LOW);
  delay(10);

  digitalWrite(COL2, HIGH);
  delay(10);
  if (digitalRead(ROW3) == 1)
  {
    Serial.println("Button 6 is pressed");
  }
  digitalWrite(COL2, LOW);
  delay(10);
}


