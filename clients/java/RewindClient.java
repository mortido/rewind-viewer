import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class RewindClient {
    private static final int MESSAGE_SCHEMA_VERSION = 5;
    private static final long MAX_MESSAGE_SIZE = 1024 * 1024; // 1MB

    private final Socket socket;
    private final OutputStream outputStream;
    private int opacity = 0xFF000000;
    private com.google.flatbuffers.FlatBufferBuilder builder = new com.google.flatbuffers.FlatBufferBuilder(1024);

    public RewindClient(String host, int port) throws IOException {
        socket = new Socket(host, port);
        socket.setTcpNoDelay(true);
        outputStream = socket.getOutputStream();
        sendProtocolVersion();
    }

    public void close() {
        try {
            socket.close();
        } catch (IOException e) {
            throw new RuntimeException("Can't close socket");
        }
    }

    private void sendProtocolVersion() throws IOException {
        ByteBuffer buffer = ByteBuffer.allocate(2);
        buffer.order(ByteOrder.LITTLE_ENDIAN);
        buffer.putShort((short) MESSAGE_SCHEMA_VERSION); // uint16?
        outputStream.write(buffer.array());
        outputStream.flush();
    }

    public void setOpacity(int opacity) {
        this.opacity = (opacity << 24);
    }

    public void endFrame() throws IOException {
        builder.clear();

        int message = 1; // TODO: build message
        builder.finish(message);
        send(builder.sizedByteArray());
    }

    private void send(byte[] data) throws IOException {
        if (data.length > MAX_MESSAGE_SIZE) {
            throw new RuntimeException("Rewind message size can't be more than 1MB");
        }

        ByteBuffer buffer = ByteBuffer.allocate(4);
        buffer.order(ByteOrder.LITTLE_ENDIAN);
        buffer.putInt(data.length); // uint32?
        try {
            outputStream.write(buffer.array());
            outputStream.write(data);
            outputStream.flush();
        } catch (IOException e) {
            throw new RuntimeException("Can't send data");
        }
    }
}
