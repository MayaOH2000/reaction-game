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

int g_GameMode = 3; // no modes have been selected
bool g_isReady = false; // when true, game is in play

unsigned long g_startTime;
unsigned long g_reactionPlayer1;
unsigned long g_reactionPlayer2;

bool g_isPlayer1Done = false;
bool g_isPlayer2Done = false;

bool g_AskGameMode = false;

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

  digitalWrite(WAIT, HIGH);
  digitalWrite(READY, HIGH);
  digitalWrite(SET, HIGH);
  digitalWrite(START, LOW);

  setColourRgb(0, 255, 255);

  // Buttons setup
  pinMode(PLAYER_1, INPUT_PULLUP);
  pinMode(PLAYER_2, INPUT_PULLUP);
  digitalWrite(PLAYER_1, HIGH);
  digitalWrite(PLAYER_2, HIGH);

  // Buzzer setup
  pinMode(BUZZER, OUTPUT);
}

// Set RGB colour
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

// Start game with selected player mode
void setPlayerMode(int iMode)
{
  if (iMode == 1)
  {
    g_GameMode = iMode;

    g_reactionPlayer1 = 0;
    g_reactionPlayer2 = 0;

    g_isPlayer1Done = false;
    g_isPlayer2Done = false;

    Serial.print("SinglePlayer\n");
    setGameStatusReady(true);
  }
  else if (iMode == 2)
  {
    g_GameMode = iMode;

    g_reactionPlayer1 = 0;
    g_reactionPlayer2 = 0;

    g_isPlayer1Done = false;
    g_isPlayer2Done = false;

    Serial.print("MultiPlayer\n");
    setGameStatusReady(true);
  }
  else if (iMode == 3)
  {
    g_GameMode = iMode;
    Serial.print("Waiting\n");
    setGameStatusReady(false);
  }
  else
  {
    Serial.print("InvalidGameMode\n");
  }
}

// Start countdown for the game
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
  g_startTime = millis(); // start game and resets time
  tone(BUZZER, 2500, 1000);
  delay(300);
  noTone(BUZZER);

  
}

// Sets status of the game
void setGameStatusReady(bool bIsReady)
{
  if (bIsReady)
  {
    g_isReady = true;
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
    g_isReady = false;
    digitalWrite(WAIT, HIGH);
    digitalWrite(READY, HIGH);
    digitalWrite(SET, HIGH);
    digitalWrite(START, LOW);

    setColourRgb(0, 255, 255);
  }
}

// Evaluate reaction time and end game
void reaction(int iPlayer)
{
  if (g_GameMode == 1)
  {
    g_reactionPlayer1 = millis() - g_startTime;

    setColourRgb(0, 0, 128);

    delay(2000);

    Serial.print("Player 1: ");
    Serial.print(g_reactionPlayer1);
    Serial.println(" ms\n");

    // Reset game
    digitalWrite(START, LOW);
    clearRgb();

    setGameStatusReady(false);
    g_AskGameMode = false;
  }

  // Get times if players have not pressed their buttons
  // Save first press for each player
  if (g_GameMode == 2 && (!g_isPlayer1Done || !g_isPlayer2Done))
  {
    if (!g_isPlayer1Done && iPlayer == PLAYER_1)
    {
      g_reactionPlayer1 = millis() - g_startTime;
      g_isPlayer1Done = true;
    }
    else if (!g_isPlayer2Done && iPlayer == PLAYER_2)
    {
      g_reactionPlayer2 = millis() - g_startTime;
      g_isPlayer2Done = true;
    }
  }

  // End game when both players pressed their buttons
  if (g_GameMode == 2 && g_isPlayer1Done && g_isPlayer2Done)
  {
    setColourRgb(0, 0, 128);

    delay(2000);

    Serial.print("Player 1: ");
    Serial.print(g_reactionPlayer1);
    Serial.println(" ms");
    Serial.print("Player 2: ");
    Serial.print(g_reactionPlayer2);
    Serial.println(" ms\n");

    // Reset game
    digitalWrite(START, LOW);
    clearRgb();

    Serial.print(g_reactionPlayer1 < g_reactionPlayer2 ? "Player 1 wins!\n" : "Player 2 wins!\n");

    setGameStatusReady(false);
    g_AskGameMode = false;
  }
}

// main loop
void loop()
{
  // Keep game mode on single player
  if (!g_isReady)
  {
    if (!g_AskGameMode)
    {
      setGameStatusReady(false);
      Serial.print("Press button 1 to play single mode and button 2 to play multiplayer mode\n");
      g_AskGameMode = true;
    }    

    if (digitalRead(PLAYER_1) == LOW)
    {
      setPlayerMode(1);
    }

    else if (digitalRead(PLAYER_2) == LOW)
    {
      setPlayerMode(2);
    }

    else if (Serial.available() > 0){
      String command = Serial.readStringUntil('\n');
      command.trim(); 
      command.toUpperCase();
      if (command == "SINGLE") {
        setPlayerMode(1);
      } 
      else if (command == "MULTI") {
        setPlayerMode(2);
      } 
      else 
      {
        Serial.println("Unknown game mode! Please enter a valid mode!");
      }
    }

  }

  if (g_GameMode == 1 && digitalRead(PLAYER_1) == LOW && g_isReady == true)
  {
      reaction(PLAYER_1);
  }

  if (g_GameMode == 2)
  {
    if (digitalRead(PLAYER_1) == LOW && g_isReady == true)
    {
      reaction(PLAYER_1);
    }

    else if (digitalRead(PLAYER_2) == LOW && g_isReady == true)
    {
      reaction(PLAYER_2);
    }    
  }
}