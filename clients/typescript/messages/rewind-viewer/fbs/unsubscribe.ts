// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

export class Unsubscribe {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):Unsubscribe {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsUnsubscribe(bb:flatbuffers.ByteBuffer, obj?:Unsubscribe):Unsubscribe {
  return (obj || new Unsubscribe()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsUnsubscribe(bb:flatbuffers.ByteBuffer, obj?:Unsubscribe):Unsubscribe {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new Unsubscribe()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

key():number {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readInt8(this.bb_pos + offset) : 0;
}

static startUnsubscribe(builder:flatbuffers.Builder) {
  builder.startObject(1);
}

static addKey(builder:flatbuffers.Builder, key:number) {
  builder.addFieldInt8(0, key, 0);
}

static endUnsubscribe(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static createUnsubscribe(builder:flatbuffers.Builder, key:number):flatbuffers.Offset {
  Unsubscribe.startUnsubscribe(builder);
  Unsubscribe.addKey(builder, key);
  return Unsubscribe.endUnsubscribe(builder);
}
}
