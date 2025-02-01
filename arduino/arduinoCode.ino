/*
    RGB LED: THIS IS A VISUAL CUE ON THE WINNER
*/
#define BLUE 11
#define GREEN 12
#define RED 13

/*  
    LED: THIS IS TO SET THE GAME STATUS
*/
#define WAIT 22
#define READY 24
#define SINGLE_PLAYER 26
#define MULTI_PLAYER 28

/*
    BUTTONS: THESE ARE USED TO GAGE REACTION TIME
*/


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
  pinMode(SINGLE_PLAYER, OUTPUT);
  pinMode(MULTI_PLAYER, OUTPUT);

  digitalWrite(WAIT, LOW);
  digitalWrite(READY, LOW);
  digitalWrite(SINGLE_PLAYER, LOW);
  digitalWrite(MULTI_PLAYER, LOW);

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

void setGameStatusReady(bool bIsReady)
{
  if (bIsReady)
  {
    digitalWrite(READY, HIGH);
    digitalWrite(WAIT, LOW);
    Serial.print("ready\n");
  }
  else
  {
    digitalWrite(READY, LOW);
    digitalWrite(WAIT, HIGH);
    Serial.print("wait\n");
  }
}

void setPlayerMode(int iMode)
{
  if (iMode == 1)
  {
    digitalWrite(SINGLE_PLAYER, HIGH);
    digitalWrite(MULTI_PLAYER, LOW);

    Serial.print("SinglePlayer\n");
  }
  if (iMode == 2)
  {
    digitalWrite(SINGLE_PLAYER, LOW);
    digitalWrite(MULTI_PLAYER, HIGH);

    Serial.print("MultiPlayer\n");
  }
  if (iMode == 3)
  {
    digitalWrite(SINGLE_PLAYER, LOW);
    digitalWrite(MULTI_PLAYER, LOW);
    setGameStatusReady(false);

    Serial.print("Waiting\n");
  }
}

// main loop
void loop()
{
  delay(5000);
  setColourRgb(0, 255, 0);
  setPlayerMode(g_GameMode);

  delay(5000);
  setGameStatusReady(g_isReady);

  delay(5000);
  g_GameMode = 1;
  setPlayerMode(g_GameMode);

  delay(5000);
  g_GameMode = 2;
  setPlayerMode(g_GameMode);

  delay(5000);
  g_isReady = true;
  setGameStatusReady(g_isReady);
}

