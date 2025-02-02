// Load environment variables
require('dotenv').config(); 

//Express 
const express = require ('express');

//Server using socket.io and http request
const http = require('http');
const { Server } = require('socket.io');

//Serial Port Usage from serial port library
const { SerialPort, DelimiterParser } = require('serialport');

const { disconnect } = require('process');

//Mysql for databse connection
const mysql = require ('mysql2');


//Expresss server set up
const app = express();
const port = 8000;

const server = http.createServer(app);

//CORS (Cross-Origin Resource Sharing) for backend + front end different port usage
const io = new Server(server, {
    cors: {
        origin: "*",
        methods: ["GET", "POST"]
    }
});

//Express to access web page
app.use(express.static('public'));


let gameMode = "MULTI";


//Connecting server to Database (MYSQL)
const db = mysql.createConnection({
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE
});

module.exports = db;

db.connect(error => {
    if (error){
        console.error("Problem Connecting to Database: ", error);
        return;
    }
    console.log("Connected to MYSQL Succefully")
});

//Connect Socket 
io.on("connection", socket => {
    console.log ('User Connected');

    // // Handle score data for mutiplayer request
    // socket.on('getScoreBoard', () => {
    //     const query = 'SELECT round_number, player1_reaction_time, player2_reaction_time, winner FROM Rounds ORDER BY round_number asc limit 10';
    //     db.query(query, (err, results) => {
    //         if (err) throw err;
    //         socket.emit('ScoreData', { rounds: results });
    //     });
    // });


    socket.emit("gameMode", gameMode);

    //Selecting game mode
    socket.on("selectGameMode", (mode) => {
        if (mode === "SINGLE" || mode === "MULTI") {
            gameMode = mode;
            io.emit("gameMode", gameMode);

            //writing mode selection to adruino
            adruinoPort.write(mode + "\n"); 
        }
    });
    
    //Insert Player score into database
    socket.on('score', (data) => {
        let sql = `INSERT INTO players (name, reaction_time) VALUES ('${data.name}', ${data.time})`;
        db.query(sql, [data.name, data.time], (error) => {
            if (error){
                console.error("Error Inserting Score: ", error);
                return;
            }
            console.log(" Score Added!");   // For Testing
            io.emit("updatedScoreBoard");
        });
    });

    //Get score board data from lowest to highest result displays only top 10 results
    socket.on('getScoreBoard', () => {
        db.query("SELECT DISTINCT reaction_time, name FROM players ORDER BY reaction_time asc limit 10", (error, results) => {
            if (error){
                console.error("Error Getting Scoreboard: ", error);
                return;
            } 
            socket.emit("ScoreData", results) 
        });
    });

    //Disconnect Socket
    socket.on("disconnect", () => console.log("Client disconnected"));
});

//Serial Port Set up
const adruinoPort = new SerialPort({path: 'COM6', baudRate: 9600});
const parser = adruinoPort.pipe(new DelimiterParser({delimiter: "\n"}));

// Variables to store reaction times
let player1ReactionTime = null;
let aiReactionTime = null;
let player1Recorded = false;

//Listen for Arduino data
parser.on("data", (data) => {
    const trimmedData = data.toString().trim();
    console.log(`Arduino data: ${trimmedData}`);

    // Check in the Arduino port for messages starting with any of these
    const parts = trimmedData.split(":");
    if (parts.length === 2) {
        const playerName = parts[0].trim();
        const time = parseFloat(parts[1].trim(), 10);

        // Update reaction times
        if (playerName === "Player 1") {
            player1ReactionTime = time;

            if(!player1Recorded){
            // Insert the data directly into the database
            let sql = `INSERT INTO players (name, reaction_time) VALUES (?, ?)`;
            db.query(sql, [playerName, time], (error) => {
                if (error) {
                    console.error("Error Inserting Score: ", error);
                    return;
                }
            console.log("Score Added to Database!");
            io.emit("updatedScoreBoard"); // Emit updated scoreboard
        });
         
         player1Recorded = true;
        }

     } else if (playerName === "AI") {
            aiReactionTime = time;
        }

        // Determine winner after both times are received
        if (player1ReactionTime !== null && aiReactionTime !== null) {
            // Emit the reaction time to the frontend
            io.emit("reactions", { player1: player1ReactionTime, ai: aiReactionTime });

            //Determine Winner
            const winner = player1ReactionTime < aiReactionTime ? "Player 1" : "AI";
            console.log(`${winner} wins!`);
            io.emit("winner", { winner });

            // Reset reaction times for the next round
            player1ReactionTime = null;
            aiReactionTime = null;
            player1Recorded = false;
        }
    }
});

//Port for server to be used on
server.listen(port, () => console.log(`Server Runing on http://localhost:${port}`))