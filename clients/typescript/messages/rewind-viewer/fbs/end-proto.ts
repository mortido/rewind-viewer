// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

export class EndProto {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):EndProto {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsEndProto(bb:flatbuffers.ByteBuffer, obj?:EndProto):EndProto {
  return (obj || new EndProto()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsEndProto(bb:flatbuffers.ByteBuffer, obj?:EndProto):EndProto {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new EndProto()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static startEndProto(builder:flatbuffers.Builder) {
  builder.startObject(0);
}

static endEndProto(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static createEndProto(builder:flatbuffers.Builder):flatbuffers.Offset {
  EndProto.startEndProto(builder);
  return EndProto.endEndProto(builder);
}
}
