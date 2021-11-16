import Express, { Request, Response, NextFunction } from "express";
import { Server as SocketServer, Socket } from "socket.io";
import { Server as HttpServer } from "http";

const port = 8000;

const app = Express();
const server = new HttpServer(app);
const socketServer = new SocketServer(server);

app.get('/', (req: Request, res: Response, next: NextFunction) => {
    res.status(200).send("Hello, i'm working!");
});

socketServer.on('connection', (client: Socket) => {
    console.log('CLIENT CONNECTED');
    client.on('join', (data) => {
        console.log(`DATA ${data}`);
    });
});

server.listen(port, () => {
    console.log(`Server running on port ${port}`);
});
