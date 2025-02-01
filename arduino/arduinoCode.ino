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
#define PLAYER_1 7
#define PLAYER_2 6

/*
  BUZZER
*/
#define BUZZER 10

// GLOBAL VARIALES, RECEIVED FROM SERIAL CONNECTION
int g_GameMode = 3;
bool g_isReady = false;
unsigned long startTime;

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
  pinMode(PLAYER_1, INPUT_PULLUP);
  pinMode(PLAYER_2, INPUT_PULLUP);
  digitalWrite(PLAYER_1, HIGH);
  digitalWrite(PLAYER_2, HIGH);

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
  delay(2000);
  clearRgb();

  // start countdown
  digitalWrite(WAIT, HIGH);
  delay(2000);
  digitalWrite(READY, HIGH);
  delay(2000);
  digitalWrite(SET, HIGH);
  delay(2000);

  // Standby
  digitalWrite(WAIT, LOW);
  digitalWrite(READY, LOW);
  digitalWrite(SET, LOW);
  digitalWrite(START, HIGH);
  delay(random(2000, 5000));  

  // Start game
  tone(BUZZER, 2500, 1000);
  delay(300);
  noTone(BUZZER);
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

void reaction()
{
  

  unsigned long reactionTime;

  reactionTime = millis() - startTime;
  setColourRgb(0, 0, 128);

  Serial.print(reactionTime);
  Serial.println(" ms\n");

  delay(2000);

  digitalWrite(START, LOW);
  clearRgb();

  setGameStatusReady(false);
}

// main loop
// currently keeps game on single mode player
void loop()
{
  // Keep game mode on single player
  if (g_GameMode == 3)
  {
    setGameStatusReady(false);
    setPlayerMode(1);
    startTime = millis(); // resets time
  }

  // button is pressed by player
  if (digitalRead(PLAYER_1) == LOW && g_isReady == true)
  {
    Serial.print("PLAYER 1 BUTTON\n");
    reaction();
  }

}