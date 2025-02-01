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

const app = express();
const port = 8000;

const server = http.createServer(app);
const io = new Server(server);

//Express to access Html 
app.use(express.static('public'));

//Connecting server to Database (MYSQL)
const db = mysql.createConnection({
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE
});

module.exports = db;

db.connect(err => {
    if (err) throw err;
    console.log("Connected to MYSQL Succefully")
})

//Connect Socket 
io.on("connection", socket => {
    console.log ('User Connected');

//Disconnect Socket
socket.on("disconnect", () => console.log("Client disconnected"));
})

//Port for server to be used on
server.listen(8000, () => console.log(`Server Runing on http://localhost:${port}`))