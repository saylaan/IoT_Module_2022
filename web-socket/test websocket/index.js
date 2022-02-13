const express = require('express');
const app = express();
const http = require('http');
const server = http.createServer(app);
const { Server } = require("socket.io");
const io = new Server(server);

app.get('/', (req, res) => {
});

io.on('connection', (socket) => {    
    console.log(`${socket.id} connected !`)

    socket.on('message', (msg) => {
        console.log(`${socket.id} sent : ${msg}`);
        socket.emit("OK received");
    })

    socket.on('disconnect', () => {
        console.log(`${socket.id} disconnected !`)
    })
});

server.listen(3000, () => {
    console.log('listening on *:3000');
});