// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

export class StartProto {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):StartProto {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsStartProto(bb:flatbuffers.ByteBuffer, obj?:StartProto):StartProto {
  return (obj || new StartProto()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsStartProto(bb:flatbuffers.ByteBuffer, obj?:StartProto):StartProto {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new StartProto()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static startStartProto(builder:flatbuffers.Builder) {
  builder.startObject(0);
}

static endStartProto(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static createStartProto(builder:flatbuffers.Builder):flatbuffers.Offset {
  StartProto.startStartProto(builder);
  return StartProto.endStartProto(builder);
}
}
