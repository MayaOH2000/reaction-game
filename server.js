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

// //Serial Port Set up
// const adruinoPort = new SerialPort({path: 'COM3', baudRate: 9600});
// const parser = adruinoPort.pipe(new DelimiterParser({delimiter: "\n"}));

//Express to access web page
app.use(express.static('public'));

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
})

//Connect Socket 
io.on("connection", socket => {
    console.log ('User Connected');

    //Insert Player score into database
    socket.on('score', ({ playername, reaction}) => {
        const sql = "INSERT INTO players (name, reaction_time) VALUES (?,?)";
        db.query(sql, [playername,reaction], (error,results) => {
            if (error){
                console.error("Error Inserting Score: ", error);
                return;
            }
            console.log(" Score Added!");   // For Testing
            io.emit("updatedScoreBoard");
        });
    });

    //Get score board data from lowest to highest result
    socket.on('getScoreBoard', () => {
        db.query("SELECT * FROM players ORDER BY reaction_time asc", (error, results) => {
            if (error){
                console.error("Error Getting Scoreboard: ", error);
                return;
            } 
            socket.emit("ScoreData", results) 
        });
    })
 

    //Disconnect Socket
    socket.on("disconnect", () => console.log("Client disconnected"));
});

//Port for server to be used on
server.listen(port, () => console.log(`Server Runing on http://localhost:${port}`))