import fs from 'fs';
import * as utils from "./utils.mjs";
import sharp from "sharp";

async function moveFile(input, output, cb) {
    try {
        const readStream = fs.createReadStream(input);
        const sh = sharp({unlimited: true});

        sh.resize({height: 64, width: 64});
        readStream.pipe(sh);
        sh.toFile(output, cb)
    } catch {}
}

async function moveFile32(input, output, cb) {
    try {
        const readStream = fs.createReadStream(input);
        const sh = sharp({unlimited: true});

        sh.resize({height: 32, width: 32});
        readStream.pipe(sh);
        sh.toFile(output, cb)
    } catch {}
}

setInterval(async () => {
    const isRCREnabled = fs.readFileSync("C:\\RCOC\\isEnabled.rco", 'utf8') == "t";
    if (isRCREnabled && fs.existsSync("C:\\RCOC\\selected.json")) {
        const selected = JSON.parse(fs.readFileSync("C:\\RCOC\\selected.json", 'utf8'));
        const locapp = process.env.LOCALAPPDATA;
        if (fs.existsSync(`${locapp}\\Roblox\\Versions`)) {
            const dirs = fs.readdirSync(`${locapp}\\Roblox\\Versions`).filter((ver) => fs.statSync(`${locapp}\\Roblox\\Versions\\${ver}`).isDirectory())
            for (const ver of dirs) {
                const rv = `${locapp}\\Roblox\\Versions\\${ver}\\content\\textures\\Cursors\\KeyboardMouse`;
                const r = `${locapp}\\Roblox\\Versions\\${ver}\\content\\textures`
                try {
                    const curhashes = await utils.getMD5s({path: selected["Arrow"], name: "Arrow"}, {path: selected["ArrowFar"], name: "ArrowFar"}, {path: selected["IBeam"], name: "IBeam"}, {path: selected["ShiftLock"], name: 'ShiftLock'});
                    const oldhashes = await utils.getMD5s({path: `${rv}\\ArrowCursor.png`, name: "Arrow"}, {path: `${rv}\\ArrowFarCursor.png`, name: "ArrowFar"}, {path: `${rv}\\IBeamCursor.png`, name: "IBeam"}, {path: `${r}\\MouseLockedCursor.png`, name: "ShiftLock"});
                    if (curhashes.find((val) => val.name == "Arrow").hash != oldhashes.find((val) => val.name == "Arrow").hash) {
                        moveFile(selected["Arrow"], `${rv}\\ArrowCursor.png`, (/** @type {Error} */error, /** @type {sharp.OutputInfo}*/ info) => {})
                    }

                    if (curhashes.find((val) => val.name == "ArrowFar").hash != oldhashes.find((val) => val.name == "ArrowFar").hash) {
                        moveFile(selected["ArrowFar"], `${rv}\\ArrowFarCursor.png`, (/** @type {Error} */error, /** @type {sharp.OutputInfo}*/ info) => {})
                    }

                    if (curhashes.find((val) => val.name == "IBeam").hash != oldhashes.find((val) => val.name == "IBeam").hash) {
                        moveFile(selected["IBeam"], `${rv}\\IBeamCursor.png`, (/** @type {Error} */error, /** @type {sharp.OutputInfo}*/ info) => {})
                    }

                    if (curhashes.find((val) => val.name == "ShiftLock").hash != oldhashes.find((val) => val.name == "ShiftLock").hash) {
                        moveFile32(selected["ShiftLock"], `${r}\\MouseLockedCursor.png`, (/** @type {Error} */error, /** @type {sharp.OutputInfo}*/ info) => {})
                    }
                }
                catch {}
            }
        }
        if (fs.existsSync("C:\\Program Files (x86)\\Roblox\\Versions")) {
            const dirs = fs.readdirSync("C:\\Program Files (x86)\\Roblox\\Versions").filter((ver) => fs.statSync(`C:\\Program Files (x86)\\Roblox\\Versions\\${ver}`).isDirectory())
            for (const ver of dirs) {
                const rv = `C:\\Program Files (x86)\\Roblox\\Versions\\${ver}\\content\\textures\\Cursors\\KeyboardMouse`;
                const r = `C:\\Program Files (x86)\\Roblox\\Versions\\${ver}\\content\\textures\\Cursors`
                try {
                    const curhashes = await utils.getMD5s({path: selected["Arrow"], name: "Arrow"}, {path: selected["ArrowFar"], name: "ArrowFar"}, {path: selected["IBeam"], name: "IBeam"}, {path: selected["ShiftLock"], name: 'ShiftLock'});
                    const oldhashes = await utils.getMD5s({path: `${rv}\\ArrowCursor.png`, name: "Arrow"}, {path: `${rv}\\ArrowFarCursor.png`, name: "ArrowFar"}, {path: `${rv}\\IBeamCursor.png`, name: "IBeam"}, {path: `${r}\\MouseLockedCursor.png`, name: "ShiftLock"});
                    if (curhashes.find((val) => val.name == "Arrow").hash != oldhashes.find((val) => val.name == "Arrow").hash) {
                        fs.copyFileSync(selected["Arrow"], `${rv}\\ArrowCursor.png`);
                        moveFile(selected["Arrow"], `${rv}\\ArrowCursor.png`, (/** @type {Error} */error, /** @type {sharp.OutputInfo}*/ info) => {})
                    }

                    if (curhashes.find((val) => val.name == "ArrowFar").hash != oldhashes.find((val) => val.name == "ArrowFar").hash) {
                        moveFile(selected["ArrowFar"], `${rv}\\ArrowFarCursor.png`, (/** @type {Error} */error, /** @type {sharp.OutputInfo}*/ info) => {})
                    }

                    if (curhashes.find((val) => val.name == "IBeam").hash != oldhashes.find((val) => val.name == "IBeam").hash) {
                        moveFile(selected["IBeam"], `${rv}\\IBeamCursor.png`, (/** @type {Error} */error, /** @type {sharp.OutputInfo}*/ info) => {})
                    }

                    if (curhashes.find((val) => val.name == "ShiftLock").hash != oldhashes.find((val) => val.name == "ShiftLock").hash) {
                        moveFile32(selected["ShiftLock"], `${r}\\MouseLockedCursor.png`, (/** @type {Error} */error, /** @type {sharp.OutputInfo}*/ info) => {})
                    }
                } catch {}
            }
        }
    }
}, 30000)
