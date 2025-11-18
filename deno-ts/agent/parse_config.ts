import * as z from "zod";

const Connection = z.strictObject({
    secret: z.string(),
    server: z.strictObject({
        hostname: z.string(),
        port: z.int(),
    }),
    forward_to: z.strictObject({
        hostname: z.string(),
        port: z.int(),
    }),
}).transform((x) => ({
    secret: x.secret,
    server: x.server,
    forwardTo: x.forward_to,
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
