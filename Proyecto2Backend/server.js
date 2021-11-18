"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const express_1 = __importDefault(require("express"));
const http_1 = require("http");
const multer_1 = __importDefault(require("multer"));
const helmet_1 = __importDefault(require("helmet"));
const fs_extra_1 = __importDefault(require("fs-extra"));
const adm_zip_1 = __importDefault(require("adm-zip"));
const port = 8000;
const app = (0, express_1.default)();
const server = new http_1.Server(app);
const forms = (0, multer_1.default)();
app.use(express_1.default.static('public'));
app.use((0, helmet_1.default)());
app.use(forms.single("textFile"));
app.get('/folders', async (req, res, next) => {
    const files = await fs_extra_1.default.readdir('textFiles');
    res.status(200).send(files);
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
app.get('/:folder', async (req, res, next) => {
    const { folder } = req.params;
    if (!folder) {
        return res.sendStatus(400);
    }
    try {
        const stat = await fs_extra_1.default.stat(`textFiles/${folder}`);
        if (!stat.isDirectory()) {
            return res.sendStatus(400);
        }
        const zip = new adm_zip_1.default();
        zip.addLocalFolder(`textFiles/${folder}`);
        const buf = zip.toBuffer();
        res.writeHead(200, {
            'Content-Disposition': `attachment; filename="${folder}.zip"`,
            'Content-Type': 'application/zip',
        });
        return res.end(buf);
    }
    catch (err) {
        res.sendStatus(400);
    }
});
server.listen(port, () => {
    console.log(`Server running on port ${port}`);
});
//# sourceMappingURL=server.js.map