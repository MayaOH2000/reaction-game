// RUN IN ARDUINO IDE

// RGB LED
#define BLUE 11
#define GREEN 12
#define RED 13

// Buttons

void setup()
{
  // RGB setup
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);

  // Buttons setup
}

void setColourRgb(int iRed, int iGreen, int iBlue)
{
  analogWrite(RED, iRed);
  analogWrite(GREEN, iGreen);
  analogWrite(BLUE, iBlue);
}

void clearRgb()
{
  setColourRgb(0, 0, 0);
}

// main loop
void loop()
{
  delay(1000);
  setColourRgb(0, 255, 0);
}

