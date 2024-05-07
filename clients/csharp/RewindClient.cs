using System;
using System.Net.Sockets;
using System.Text;
using FlatBuffers;

namespace RewindViewer
{
    public class RewindClient : IDisposable
    {
        private const ushort MessageSchemaVersion = 5;
        private const int MaxMessageSize = 1024 * 1024; // 1MB

        private TcpClient socket;
        private NetworkStream stream;
        private FlatBufferBuilder builder;
        private bool isLittleEndian;
        private uint opacity = 0xFF000000;

        public RewindClient(string host, int port)
        {
            socket = new TcpClient();
            try
            {
                socket.Connect(host, port);
                stream = socket.GetStream();
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine($"RewindClient:: Cannot open viewer socket. Launch viewer before behavior. Error: {ex.Message}");
            }

            isLittleEndian = BitConverter.IsLittleEndian;
            SendProtocolVersion();
        }

        public void SetOpacity(uint opacity)
        {
            this.opacity = opacity << 24;
        }

        public void EndFrame()
        {
            builder.Clear();
            var command = fbs.EndFrame.CreateEndFrame(builder);
            var msg = fbs.RewindMessage.CreateRewindMessage(builder, fbs.Command.EndFrame, command.Value);
            builder.Finish(msg.Value);
            Send(builder.SizedByteArray());
        }

        public void Dispose()
        {
            Close();
            socket?.Dispose();
            stream?.Dispose();
        }

        private void Close()
        {
            if (socket != null && socket.Connected)
            {
                stream.Close();
                socket.Close();
            }
        }

        private void SendProtocolVersion()
        {
            byte[] buffer = BitConverter.GetBytes(MessageSchemaVersion);
            if (!isLittleEndian)
                Array.Reverse(buffer);
            stream.Write(buffer, 0, buffer.Length);
        }

        private void Send(byte[] buf)
        {
            if (buf.Length > MaxMessageSize)
                throw new InvalidOperationException("Rewind message size can't be more than 1MB");

            byte[] buffer = BitConverter.GetBytes(buf.Length);
            if (!isLittleEndian)
                Array.Reverse(buffer);
            stream.Write(buffer, 0, buffer.Length);
            stream.Write(buf, 0, buf.Length);
        }
    }
}
