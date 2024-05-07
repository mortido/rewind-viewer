use std::net::{TcpStream, ToSocketAddrs};
use std::io::{self, Write};
use byteorder::{ByteOrder, LittleEndian, WriteBytesExt};

const MESSAGE_SCHEMA_VERSION: u16 = 5;
const MAX_MESSAGE_SIZE: usize = 1024 * 1024; // 1MB

pub struct RewindClient {
    stream: TcpStream,
    builder: flatbuffers::FlatBufferBuilder,
    opacity: u32,
    is_little_endian: bool,
}

impl RewindClient {
    pub fn new<A: ToSocketAddrs>(addr: A) -> io::Result<Self> {
        let stream = TcpStream::connect(addr)?;
        stream.set_nodelay(true)?;
        let is_little_endian = cfg!(target_endian = "little");

        let mut client = Self {
            stream,
            builder: flatbuffers::FlatBufferBuilder::new_with_capacity(1024),
            opacity: 0xFF000000,
            is_little_endian,
        };

        client.send_protocol_version()?;
        Ok(client)
    }

    pub fn set_opacity(&mut self, opacity: u32) {
        self.opacity = opacity << 24;
    }

    pub fn end_frame(&mut self) -> io::Result<()> {
        self.builder.reset();
        let command = fbs::EndFrame::create(&mut self.builder, &fbs::EndFrameArgs {});
        let msg = fbs::RewindMessage::create(
            &mut self.builder,
            &fbs::RewindMessageArgs {
                command_type: fbs::Command::EndFrame,
                command: Some(command.as_union_value()),
            },
        );
        self.builder.finish(msg, None);
        self.send(self.builder.finished_data())
    }

    fn send_protocol_version(&mut self) -> io::Result<()> {
        let mut buffer = [0u8; 2];
        LittleEndian::write_u16(&mut buffer, MESSAGE_SCHEMA_VERSION);
        self.stream.write_all(&buffer)
    }

    fn send(&mut self, buf: &[u8]) -> io::Result<()> {
        if buf.len() > MAX_MESSAGE_SIZE {
            return Err(io::Error::new(
                io::ErrorKind::InvalidInput,
                "Rewind message size can't be more than 1MB",
            ));
        }

        let mut size_buffer = [0u8; 4];
        LittleEndian::write_u32(&mut size_buffer, buf.len() as u32);
        self.stream.write_all(&size_buffer)?;
        self.stream.write_all(buf)
    }
}

impl Drop for RewindClient {
    fn drop(&mut self) {
        let _ = self.stream.shutdown(std::net::Shutdown::Both);
        println!("Connection closed on drop.");
    }
}

// Ensure that the RewindClient is not cloneable or copyable.
impl Clone for RewindClient {
    fn clone(&self) -> Self {
        panic!("RewindClient should not be cloned");
    }
}
