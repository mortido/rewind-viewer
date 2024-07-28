// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

import { Vector2f } from '../../rewind-viewer/fbs/vector2f.js';


export class ColorPoint {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):ColorPoint {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsColorPoint(bb:flatbuffers.ByteBuffer, obj?:ColorPoint):ColorPoint {
  return (obj || new ColorPoint()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsColorPoint(bb:flatbuffers.ByteBuffer, obj?:ColorPoint):ColorPoint {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new ColorPoint()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

color():number {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readUint32(this.bb_pos + offset) : 0;
}

position(obj?:Vector2f):Vector2f|null {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? (obj || new Vector2f()).__init(this.bb_pos + offset, this.bb!) : null;
}

static startColorPoint(builder:flatbuffers.Builder) {
  builder.startObject(2);
}

static addColor(builder:flatbuffers.Builder, color:number) {
  builder.addFieldInt32(0, color, 0);
}

static addPosition(builder:flatbuffers.Builder, positionOffset:flatbuffers.Offset) {
  builder.addFieldStruct(1, positionOffset, 0);
}

static endColorPoint(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  builder.requiredField(offset, 6) // position
  return offset;
}

}
