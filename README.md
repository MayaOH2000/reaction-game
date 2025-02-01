# reaction-game

## Dependencies
Install dependencies by typing npm install dependency name
- For example for express : npm install express 
- express
- socket.io 
- serialport 
- mysql2

## Arduino Code
Run this code on an Arduino Mega 2560 board in the Arduino IDE

Here is a list of the events on the hardware:
- Waiting for game mode selection: RGB lights up cyan, while red, yellow and green LEDs light up
- Game mode selected: RGB lights up magenta
- Game counts down: Red, yellow and green LED light up one by one
- Game is ready: White LED lights up, a sound cue will indicate the start of the game
- Game ends: RGB lights up blue