"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const express_1 = __importDefault(require("express"));
const socket_io_1 = require("socket.io");
const http_1 = require("http");
const port = 8000;
const app = (0, express_1.default)();
const server = new http_1.Server(app);
const socketServer = new socket_io_1.Server(server);
app.get('/', (req, res, next) => {
    res.status(200).send("Hello, i'm working!");
});
socketServer.on('connection', (client) => {
    console.log('CLIENT CONNECTED');
    client.on('join', (data) => {
        console.log(`DATA ${data}`);
    });
});
server.listen(port, () => {
    console.log(`Server running on port ${port}`);
});
//# sourceMappingURL=server.js.map