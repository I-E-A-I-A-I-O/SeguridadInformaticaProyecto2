"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const express_1 = __importDefault(require("express"));
const socket_io_1 = require("socket.io");
const http_1 = require("http");
const socket_io_client_1 = __importDefault(require("socket.io-client"));
const multer_1 = __importDefault(require("multer"));
const helmet_1 = __importDefault(require("helmet"));
const fs_extra_1 = __importDefault(require("fs-extra"));
const port = 8000;
const app = (0, express_1.default)();
const server = new http_1.Server(app);
const socketServer = new socket_io_1.Server(server);
const forms = (0, multer_1.default)();
app.use((0, helmet_1.default)());
app.use(forms.single("textFile"));
app.get('/', (req, res, next) => {
    const client = (0, socket_io_client_1.default)('http://localhost:8000');
    res.status(200).send("Hello, i'm working!");
});
app.post('/file', async (req, res, next) => {
    let { folderName } = req.body;
    if (!req.file || !folderName) {
        console.error("file missing from request.");
        return res.sendStatus(400);
    }
    folderName = folderName.replace('\n', '');
    folderName = folderName.concat(' - ', req.ip.substring(req.ip.lastIndexOf(':') + 1, req.ip.length));
    folderName = folderName.replaceAll(':', '-');
    const path = `textFiles/${folderName}`;
    try {
        await fs_extra_1.default.ensureDir(path);
        console.info(`Copying file ${req.file.originalname}.`);
        await fs_extra_1.default.writeFile(`${path}/${req.file.originalname}`, req.file.buffer);
        console.info(`file ${req.file.originalname} copied.`);
        res.sendStatus(200);
    }
    catch (err) {
        console.error(err);
        res.sendStatus(500);
    }
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