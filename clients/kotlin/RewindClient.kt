import java.io.IOException
import java.net.Socket
import java.nio.ByteBuffer
import java.nio.ByteOrder

class RewindClient(host: String, port: Int) {
    private companion object {
        const val MESSAGE_SCHEMA_VERSION = 5
        const val MAX_MESSAGE_SIZE = 1024 * 1024L // 1MB
    }

    private val socket = Socket(host, port).apply { tcpNoDelay = true }
    private val outputStream = socket.getOutputStream()
    private val builder = com.google.flatbuffers.FlatBufferBuilder(1024)

    init {
        sendProtocolVersion()
    }

    fun close() {
        try {
            socket.close()
        } catch (e: IOException) {
            throw RuntimeException("Can't close socket", e)
        }
    }

    @Throws(IOException::class)
    private fun sendProtocolVersion() {
        val buffer = ByteBuffer.allocate(2).apply {
            order(ByteOrder.LITTLE_ENDIAN)
            putShort(MESSAGE_SCHEMA_VERSION.toShort())  // uint16?
        }
        outputStream.write(buffer.array())
        outputStream.flush()
    }

    fun setOpacity(opacity: Int) {
        this.opacity = opacity shl 24
    }

    @Throws(IOException::class)
    fun endFrame() {
        builder.clear()

        val message = 1 // TODO: Replace with actual message creation logic
        builder.finish(message)
        send(builder.sizedByteArray())
    }

    @Throws(IOException::class)
    private fun send(data: ByteArray) {
        if (data.size > MAX_MESSAGE_SIZE) {
            throw RuntimeException("Rewind message size can't be more than 1MB")
        }

        val buffer = ByteBuffer.allocate(4).apply {
            order(ByteOrder.LITTLE_ENDIAN)
            putInt(data.size) // uint32?
        }
        outputStream.apply {
            write(buffer.array())
            write(data)
            flush()
        }
    }
}
