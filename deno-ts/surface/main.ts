import { Connection, parseConfig } from "./parse_config.ts";
import { TextEnDe } from "@tunneltent/shared";

async function seekOperator(listener: Deno.TcpListener, secret: string) {
    let operator;
    while (operator === undefined) {
        operator = await listener.accept();
        const buffer = new Uint8Array(256);
        await operator.read(buffer);
        const decoded = TextEnDe.decode(buffer.slice(0, secret.length));
        if (decoded !== secret) {
            operator.close();
            operator = undefined;
            continue;
        }
    }
    return operator;
}

async function establishConnection(info: Connection) {
    const operatorListener = Deno.listen({
        hostname: "localhost",
        port: info.operativePort,
    });

    const clientListener = Deno.listen({
        hostname: "localhost",
        port: info.clientPort,
    });

    console.log(
        `listening on operator:${info.operativePort} & client:${info.clientPort}`,
    );

    const masterOperator = await seekOperator(
        operatorListener,
        info.secret,
    );

    console.log("master operator found");

    while (true) {
        const client = await clientListener.accept();
        console.log("client connected");
        const bytesWritten = await masterOperator.write(
            TextEnDe.encode("more_clients"),
        );
        if (bytesWritten === null) {
            break;
        }
        const slaveOperator = await seekOperator(operatorListener, info.secret);
        console.log("slave operator found");
        client.readable.pipeTo(slaveOperator.writable);
        slaveOperator.readable.pipeTo(client.writable);
    }
    console.log("master operator connection severed");
}

async function main() {
    const { connections } = await parseConfig();
    for (const info of connections) {
        establishConnection(info);
    }
}

if (import.meta.main) {
    await main();
}
