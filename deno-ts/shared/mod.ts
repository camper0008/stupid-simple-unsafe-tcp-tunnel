export class TextEnDe {
    private static decoder = new TextDecoder();
    private static encoder = new TextEncoder();

    public static encode(input: string) {
        return this.encoder.encode(input);
    }

    public static decode(input: AllowSharedBufferSource) {
        return this.decoder.decode(input);
    }
}
