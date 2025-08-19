# Reaction Game

A real-time reaction game with Arduino integration, featuring single-player and multiplayer modes.

## Project Goal
Demonstrates real-time hardware-software integration, multiplayer networking, and adaptive AI in a reaction game.

## 🚀 Features
- Real-time gameplay using Socket.IO
- Arduino integration for physical interaction
- Single-player and multiplayer modes
- Leaderboard with MySQL database integration
- Responsive web interface
- Adaptive rule-based AI opponent for single-player

## 🛠️ Technologies Used
- Node.js
- Express.js
- Socket.IO
- MySQL
- Arduino

## 📋 Prerequisites
- Node.js (v14+ recommended)
- MySQL server
- Arduino IDE
- Arduino Mega 2560 board

## 🔧 Installation

### 1. Clone Repository
```bash
git clone https://github.com/yourusername/reaction-game.git
cd reaction-game
```

### 2. Install Dependencies
```bash
npm install express socket.io serialport mysql2 cors dotenv
```

### 3. Set up your environment variables in a `.env` file:
```env
DB_HOST=your_host
DB_USER=your_username
DB_PASSWORD=your_password
DB_DATABASE=your_database
```

### 4. Database Setup
Set up your MySQL database using `database_schema.sql`.

## 🎮 Running the Game
1. Start Server:
   
   ```bash
   node server.js
   ```
3. Open `http://localhost:8000` in your web browser.

## 🔌 Arduino Code / Setup
Run this code on an Arduino Mega 2560 board in the Arduino IDE

Here is a list of the events on the hardware:
- Waiting for game mode selection: RGB lights up cyan, while red, yellow and green LEDs light up
- Game mode selected: RGB lights up magenta
- Game counts down: Red, yellow and green LED lights up one by one
- Game is ready: White LED lights up, a sound cue will indicate the start of the game
- Game ends: RGB lights up blue

## 📊 Leaderboard

The leaderboard displays the top 10 reaction times. It updates in real-time as new scores are added.
