import { FlatBufferBuilder } from "flatbuffers";
import * as fbs from './messages/rewind_message_generated'; // Assuming the FlatBuffers generated file
import * as net from 'net';

const MESSAGE_SCHEMA_VERSION = 5;
const MAX_MESSAGE_SIZE = 1024 * 1024; // 1MB

export class RewindClient {
    private socket: net.Socket;
    private builder: FlatBufferBuilder;
    private opacity: number = 0xFF000000;
    private isLittleEndian: boolean;

    constructor(host: string, port: number) {
        this.socket = new net.Socket();
        this.socket.connect(port, host, () => {
            console.log("Connected to server.");
            this.sendProtocolVersion();
        });

        this.socket.on('error', (err) => {
            console.error('RewindClient: Cannot open viewer socket. Launch viewer before behavior', err);
        });

        this.builder = new FlatBufferBuilder();

        // Determine system endianness
        const buffer = new ArrayBuffer(2);
        new DataView(buffer).setInt16(0, 256, true); // little-endian write
        this.isLittleEndian = new Int16Array(buffer)[0] === 256;

        this.socket.on('close', () => {
            console.log("Connection closed.");
        });
    }

    setOpacity(opacity: number): void {
        this.opacity = (opacity << 24);
    }

    endFrame(): void {
        this.builder.clear();
        let command = fbs.EndFrame.createEndFrame(this.builder);
        let msg = fbs.RewindMessage.createRewindMessage(this.builder, fbs.Command.EndFrame, command);
        this.builder.finish(msg);
        this.send(this.builder.asUint8Array());
    }

    private sendProtocolVersion(): void {
        const buffer = Buffer.alloc(2);
        buffer.writeInt16BE(MESSAGE_SCHEMA_VERSION, 0);
        if (!this.isLittleEndian) {
            buffer.swap16(); // swap bytes if system is not little-endian
        }
        this.socket.write(buffer);
    }

    private send(buf: Uint8Array): void {
        if (buf.length > MAX_MESSAGE_SIZE) {
            throw new Error("Rewind message size can't be more than 1MB");
        }
        const buffer = Buffer.alloc(4);
        buffer.writeUInt32BE(buf.length, 0);
        if (!this.isLittleEndian) {
            buffer.swap32(); // swap bytes if system is not little-endian
        }
        this.socket.write(buffer);
        this.socket.write(buf);
    }

    close(): void {
        if (!this.socket.destroyed) {
            this.socket.end(() => {
                this.socket.destroy();
            });
        }
    }
}
