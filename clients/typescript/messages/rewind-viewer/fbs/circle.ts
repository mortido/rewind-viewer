// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

import { Color } from '../../rewind-viewer/fbs/color.js';
import { Vector2f } from '../../rewind-viewer/fbs/vector2f.js';


export class Circle {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):Circle {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsCircle(bb:flatbuffers.ByteBuffer, obj?:Circle):Circle {
  return (obj || new Circle()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsCircle(bb:flatbuffers.ByteBuffer, obj?:Circle):Circle {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new Circle()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

color(obj?:Color):Color|null {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? (obj || new Color()).__init(this.bb!.__indirect(this.bb_pos + offset), this.bb!) : null;
}

center(obj?:Vector2f):Vector2f|null {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? (obj || new Vector2f()).__init(this.bb_pos + offset, this.bb!) : null;
}

radius():number {
  const offset = this.bb!.__offset(this.bb_pos, 8);
  return offset ? this.bb!.readFloat32(this.bb_pos + offset) : 0.0;
}

static startCircle(builder:flatbuffers.Builder) {
  builder.startObject(3);
}

static addColor(builder:flatbuffers.Builder, colorOffset:flatbuffers.Offset) {
  builder.addFieldOffset(0, colorOffset, 0);
}

static addCenter(builder:flatbuffers.Builder, centerOffset:flatbuffers.Offset) {
  builder.addFieldStruct(1, centerOffset, 0);
}

static addRadius(builder:flatbuffers.Builder, radius:number) {
  builder.addFieldFloat32(2, radius, 0.0);
}

static endCircle(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  builder.requiredField(offset, 6) // center
  return offset;
}

}
