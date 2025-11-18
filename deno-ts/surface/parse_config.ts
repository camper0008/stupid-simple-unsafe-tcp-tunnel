import * as z from "zod";

const Connection = z.strictObject({
    secret: z.string(),
    operative_port: z.int(),
    client_port: z.int(),
}).transform((x) => ({
    secret: x.secret,
    operativePort: x.operative_port,
    clientPort: x.client_port,
}));

const Config = z.strictObject({
    connections: z.array(Connection),
});

export type Connection = z.infer<typeof Connection>;
export type Config = z.infer<typeof Config>;

export async function parseConfig(): Promise<Config> {
    const raw = JSON.parse(await Deno.readTextFile("conf.jsonc"));
    return Config.parse(raw);
}
