import {default as inquirer} from 'inquirer';
import { dialog } from "node-file-dialog";
import fs from 'fs';
import interrupt from 'inquirer-interrupted-prompt';
import * as utils from "./utils.mjs";
interrupt.fromAll(inquirer);
const dcp = "C:\\RCOC\\default_cursors";

const tbl = {
    "Reset Cursors": {
        execute: async () => {
            if (!fs.existsSync(`${dcp}\\arrow.png`) || !fs.existsSync(`${dcp}\\arrowfar.png`) || !fs.existsSync(`${dcp}\\ibeam.png`)) {
                const dc = await utils.getDefaultCursors();
                if (!fs.existsSync(dcp)) fs.mkdirSync(dcp);
                fs.writeFileSync(`${dcp}\\arrow.png`, dc.arrow);
                fs.writeFileSync(`${dcp}\\arrowfar.png`, dc.arrowfar);
                fs.writeFileSync(`${dcp}\\ibeam.png`, dc.ibeam);
            }
            fs.writeFileSync("C:\\RCOC\\selected.json", JSON.stringify({
                "ArrowFar": `${dcp}\\arrowfar.png`,
                "Arrow": `${dcp}\\arrow.png`,
                "IBeam": `${dcp}\\ibeam.png`
            }));
            askMode();
        }
        
    },
    "Change ArrowCursor": {
        execute: async () => {
            try {
                const paths = await dialog({
                    type: 'open-file',
                    extra: {
                        types: {
                            "all image files": "*.png;*.jpg;*.jpeg;*.gif;*.bmp;*.ico",
                            "all files": '*.*'
                        },
                        title: "pick a cursor to use"
                    }
                })
                const currentActive = JSON.parse(fs.readFileSync("C:\\RCOC\\selected.json"));
                currentActive["Arrow"] = paths[0];
                fs.writeFileSync("C:\\RCOC\\selected.json", JSON.stringify(currentActive));
            } catch {}
            askMode();
        }
    },
    "Change ArrowFarCursor": {
        execute: async () => {
            try {
                const paths = await dialog({
                    type: 'open-file',
                    extra: {
                        types: {
                            "all image files": "*.png;*.jpg;*.jpeg;*.gif;*.bmp;*.ico",
                            "all files": '*.*'
                        },
                        title: "pick a cursor to use"
                    }
                })
                const currentActive = JSON.parse(fs.readFileSync("C:\\RCOC\\selected.json"));
                currentActive["ArrowFar"] = paths[0];
                fs.writeFileSync("C:\\RCOC\\selected.json", JSON.stringify(currentActive));
            } catch {}
            askMode();
        }
    },
    "Change IBeamCursor": {
        execute: async () => {
            try {
                const paths = await dialog({
                    type: 'open-file',
                    extra: {
                        types: {
                            "all image files": "*.png;*.jpg;*.jpeg;*.gif;*.bmp;*.ico",
                            "all files": '*.*'
                        },
                        title: "pick a cursor to use"
                    }
                })
                const currentActive = JSON.parse(fs.readFileSync("C:\\RCOC\\selected.json"));
                currentActive["IBeam"] = paths[0];
                fs.writeFileSync("C:\\RCOC\\selected.json", JSON.stringify(currentActive));
            } catch {}
            askMode();
        }
    },
    "Exit": {
        execute: () => {
            process.exit(0);
        }
    }
}

const choices = Object.keys(tbl);

const bottombar = new inquirer.ui.BottomBar();
bottombar.updateBottomBar('Press B to go back one page.\n')

function askMode() {
    inquirer.prompt({type: 'list', name: 'mode', message: 'Select an option.', choices: choices, loop: false}).then(async (awnser) =>{
        await tbl[awnser.mode].execute()
    })
}

askMode();
