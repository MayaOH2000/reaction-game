/*
    RGB LED: THIS IS A VISUAL CUE
*/
#define BLUE 11
#define GREEN 12
#define RED 13

/*  
    LED: THIS IS FOR GAME COUNTDOWN
*/
#define WAIT 22
#define READY 24
#define SET 26
#define START 28

/*
    BUTTONS: THESE ARE USED TO GAGE REACTION TIME
*/

/*
  BUZZER
*/
#define BUZZER 10

// GLOBAL VARIALES, RECEIVED FROM SERIAL CONNECTION
int g_GameMode = 3;
bool g_isReady = false;


void setup()
{
  Serial.begin(9600);

  // RGB setup
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);

  // LED setup
  pinMode(WAIT, OUTPUT);
  pinMode(READY, OUTPUT);
  pinMode(SET, OUTPUT);
  pinMode(START, OUTPUT);

  digitalWrite(WAIT, LOW);
  digitalWrite(READY, LOW);
  digitalWrite(SET, LOW);
  digitalWrite(START, LOW);

  // Buttons setup

  // Buzzer setup
  pinMode(BUZZER, OUTPUT);
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

void setPlayerMode(int iMode)
{
  if (iMode == 1)
  {
    g_GameMode = iMode;
    Serial.print("SinglePlayer\n");
    g_isReady = true;
    setGameStatusReady(g_isReady);
  }
  else if (iMode == 2)
  {
    g_GameMode = iMode;
    Serial.print("MultiPlayer\n");
    g_isReady = true;
    setGameStatusReady(g_isReady);
  }
  else if (iMode == 3)
  {
    g_GameMode = iMode;
    Serial.print("Waiting\n");
    g_isReady = false;
    setGameStatusReady(g_isReady);
  }
  else
  {
    Serial.print("InvalidGameMode\n");
  }
}

void countdown()
{
  delay(4000);

  clearRgb();

  digitalWrite(WAIT, HIGH);

  delay(2000);

  digitalWrite(READY, HIGH);

  delay(2000);
 
  digitalWrite(SET, HIGH);

  delay(2000);


  digitalWrite(WAIT, LOW);
  digitalWrite(READY, LOW);
  digitalWrite(SET, LOW);

  tone(BUZZER, 2500, 10000);

  digitalWrite(START, HIGH);
  delay(1000);
  noTone(BUZZER);

  Serial.print("START\n");
  
  delay(2000);
}

void setGameStatusReady(bool bIsReady)
{
  if (bIsReady)
  {
    digitalWrite(WAIT, LOW);
    digitalWrite(READY, LOW);
    digitalWrite(SET, LOW);
    digitalWrite(START, LOW);

    clearRgb();
    setColourRgb(255, 0, 255);

    countdown();
  }
  else
  {
    digitalWrite(WAIT, HIGH);
    digitalWrite(READY, HIGH);
    digitalWrite(SET, HIGH);
    digitalWrite(START, LOW);

    setColourRgb(255, 0, 0);
  }
}

// main loop
void loop()
{
  delay(2000);
  setColourRgb(0, 255, 0);
  setPlayerMode(3);

  setPlayerMode(1);

  delay(1000);

  setPlayerMode(2);

  delay(1000);
}
