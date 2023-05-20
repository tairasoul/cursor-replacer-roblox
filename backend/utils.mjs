import crypto from 'crypto';
import fs from 'fs';
import {default as axios} from 'axios';
import path from 'path';

function getMD5(file, name) {
    return new Promise((resolve, reject) => {
        const stream = fs.createReadStream(file);
        const hash = crypto.createHash('md5', {encoding: "binary"});
        stream.on('data', (data) => hash.update(data));
        stream.on('end', () => resolve({name: name, hash: hash.digest()}));
    })
}

export async function getMD5s(...files) {
    const funcs = [];
    for (const prop in files) {
        const file = files[prop];
        funcs.push(getMD5(file.path, file.name));
    }
    return await Promise.all(funcs);
}

export async function getRobloxVersion() {
    return (await axios.get("https://setup.rbxcdn.com/version")).data;
}

export async function getDefaultCursors() {
    const arrow = (await axios.get("https://raw.githubusercontent.com/fheahdythdr/random-shit-again/main/ArrowCursor.png")).data;
    const arrowfar = (await axios.get("https://raw.githubusercontent.com/fheahdythdr/random-shit-again/main/ArrowFarCursor.png")).data;
    const ibeam = (await axios.get("https://raw.githubusercontent.com/fheahdythdr/random-shit-again/main/IBeamCursor.png")).data;
    return {
        arrow: arrow,
        arrowfar: arrowfar,
        ibeam: ibeam
    }
}