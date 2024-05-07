package rewindviewer

import (
	"encoding/binary"
	"fmt"
	"net"
)

const (
	messageSchemaVersion = 5
	maxMessageSize       = 1024 * 1024 // 1MB
)

type RewindClient struct {
	conn      net.Conn
	builder   *flatbuffers.Builder
	opacity   uint32
}

func NewRewindClient(host string, port int) (*RewindClient, error) {
	address := fmt.Sprintf("%s:%d", host, port)
	conn, err := net.Dial("tcp", address)
	if err != nil {
		fmt.Fprintf(os.Stderr, "RewindClient:: Cannot open viewer socket. Launch viewer before behavior\n")
		return nil, err
	}
	conn.(*net.TCPConn).SetNoDelay(true)

	client := &RewindClient{
		conn:      conn,
		builder:   flatbuffers.NewBuilder(0),
		opacity:   0xFF000000,
	}

	client.sendProtocolVersion()
	return client, nil
}

func (c *RewindClient) SetOpacity(opacity uint32) {
	c.opacity = opacity << 24
}

func (c *RewindClient) EndFrame() {
	c.builder.Reset()
	command := fbs.CreateEndFrame(c.builder)
	msg := fbs.CreateRewindMessage(c.builder, fbs.CommandEndFrame, command)
	c.builder.Finish(msg)
	c.send(c.builder.FinishedBytes())
}

func (c *RewindClient) Close() {
	if c.conn != nil {
		c.conn.Close()
	}
}

func (c *RewindClient) sendProtocolVersion() {
	buffer := make([]byte, 2)
	binary.LittleEndian.PutUint16(buffer, messageSchemaVersion)
	c.conn.Write(buffer)
}

func (c *RewindClient) send(buf []byte) {
	if len(buf) > maxMessageSize {
		panic("Rewind message size can't be more than 1MB")
	}
	header := make([]byte, 4)
	binary.LittleEndian.PutUint32(header, uint32(len(buf)))
	c.conn.Write(header)
	c.conn.Write(buf)
}

func reverse(bytes []byte) {
	for i, j := 0, len(bytes)-1; i < j; i, j = i+1, j-1 {
		bytes[i], bytes[j] = bytes[j], bytes[i]
	}
}
