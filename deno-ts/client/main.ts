import { Connection, parseConfig } from "./parse_config.ts";
import { TextEnDe } from "@tunneltent/shared";

async function spawnClient(info: Connection) {
    const operator = await Deno.connect({ ...info.server });
    operator.write(TextEnDe.encode(info.secret + ";"));
    const forwardTo = await Deno.connect({ ...info.forwardTo });
    console.log(
        `middlemanning between ${info.server.hostname}:${info.server.port} <-> :${info.forwardTo.hostname}:${info.forwardTo.port}`,
    );

    operator.readable.pipeTo(forwardTo.writable);
    forwardTo.readable.pipeTo(operator.writable);
}

async function handleThread(info: Connection) {
    const masterOperator = await Deno.connect({ ...info.server });
    await masterOperator.write(
        TextEnDe.encode(info.secret + ";"),
    );

    const buffer = new Uint8Array(256);
    while (true) {
        const ret = await masterOperator.read(buffer);
        if (ret === null) {
            break;
        }
        const text = TextEnDe.decode(buffer);
        if (text.slice(0, "more_clients".length) === "more_clients") {
            spawnClient(info);
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
