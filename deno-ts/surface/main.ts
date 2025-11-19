import { Connection, parseConfig } from "./parse_config.ts";
import { TextEnDe } from "@tunneltent/shared";

async function acquireAgentConnection(
    listener: Deno.TcpListener,
    secret: string,
): Promise<Deno.Conn<Deno.Addr>> {
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
        port: info.operatorPort,
    });

    const surfaceListener = Deno.listen({
        hostname: "localhost",
        port: info.surfacePort,
    });

    console.log(
        `listening on operator:${info.operatorPort} & surface:${info.surfacePort}`,
    );

    const operatorConnection = await acquireAgentConnection(
        operatorListener,
        info.secret,
    );

    console.log("operator found");

    while (true) {
        const surface = await surfaceListener.accept();
        console.log("surface connected");
        const bytesWritten = await operatorConnection.write(
            TextEnDe.encode("more_clients"),
        );
        if (bytesWritten === null) {
            break;
        }
        const agent = await acquireAgentConnection(
            operatorListener,
            info.secret,
        );
        console.log("agent connection found");
        surface.readable.pipeTo(agent.writable);
        agent.readable.pipeTo(surface.writable);
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
