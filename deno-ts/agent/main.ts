import { Connection, parseConfig } from "./parse_config.ts";
import { TextEnDe } from "@tunneltent/shared";

async function spawnProxy(info: Connection) {
    const surface = await Deno.connect({ ...info.surface });
    surface.write(TextEnDe.encode(info.secret));
    const forwardTo = await Deno.connect({ ...info.forwardTo });
    console.log(
        `middlemanning between ${info.surface.hostname}:${info.surface.port} <-> :${info.forwardTo.hostname}:${info.forwardTo.port}`,
    );

    surface.readable.pipeTo(forwardTo.writable);
    forwardTo.readable.pipeTo(surface.writable);
}

async function handleThread(info: Connection) {
    const operatorConnection = await Deno.connect({ ...info.surface });
    await operatorConnection.write(
        TextEnDe.encode(info.secret),
    );

    const buffer = new Uint8Array(256);
    while (true) {
        const ret = await operatorConnection.read(buffer);
        if (ret === null) {
            break;
        }
        const text = TextEnDe.decode(buffer);
        if (text.slice(0, "more_clients".length) === "more_clients") {
            spawnProxy(info);
        } else {
            console.log(
                `received invalid message '${text}' (${text.length})`,
            );
        }
    }
    console.log("thread closed");
}

async function main() {
    const { connections } = await parseConfig();
    for (const info of connections) {
        handleThread(info);
    }
}

if (import.meta.main) {
    await main();
}
