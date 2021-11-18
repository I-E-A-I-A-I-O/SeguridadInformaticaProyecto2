import Express, { Request, Response, NextFunction } from "express";
import { Server as HttpServer } from "http";
import multer from "multer";
import helmet from "helmet";
import fse from "fs-extra";
import admZip from "adm-zip";

const port = 8000;

const app = Express();
const server = new HttpServer(app);

const forms = multer();

app.use(Express.static('public'));
app.use(helmet());
app.use(forms.single("textFile"));

app.get('/folders', async (req: Request, res: Response, next: NextFunction) =>
{
    const files = await fse.readdir('textFiles');
    res.status(200).send(files);
});

app.post('/file', async (req: Request, res: Response, next: NextFunction) =>
{
    let { folderName } = req.body;
    if (!req.file || !folderName)
    {
        console.error("file missing from request.");
        return res.sendStatus(400);
    }
    folderName = (folderName as string).replace('\n', '');
    folderName = (folderName as string).concat(' - ', req.ip.substring(req.ip.lastIndexOf(':') + 1, req.ip.length));
    folderName = (folderName as string).replaceAll(':', '-');
    const path = `textFiles/${folderName}`;
    try
    {
        await fse.ensureDir(path);

        console.info(`Copying file ${req.file.originalname}.`);
        await fse.writeFile(`${path}/${req.file.originalname}`, req.file.buffer);
        console.info(`file ${req.file.originalname} copied.`);

        res.sendStatus(200);
    } catch (err)
    {
        console.error(err);
        res.sendStatus(500);
    }
});

app.get('/:folder', async (req: Request, res: Response, next: NextFunction) =>
{
    const { folder } = req.params;

    if (!folder)
    {
        return res.sendStatus(400);
    }

    try
    {
        const stat = await fse.stat(`textFiles/${folder}`);

        if (!stat.isDirectory())
        {
            return res.sendStatus(400);
        }

        const zip = new admZip();
        zip.addLocalFolder(`textFiles/${folder}`);
        const buf = zip.toBuffer();

        res.writeHead(200, {
            'Content-Disposition': `attachment; filename="${folder}.zip"`,
            'Content-Type': 'application/zip',
        });

        return res.end(buf);
    } catch (err)
    {
        res.sendStatus(400);
    }
});

server.listen(port, () =>
{
    console.log(`Server running on port ${port}`);
});
