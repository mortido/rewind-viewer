// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

import { Color } from '../../rewind-viewer/fbs/color.js';
import { Vector2f } from '../../rewind-viewer/fbs/vector2f.js';


export class Polyline {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):Polyline {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsPolyline(bb:flatbuffers.ByteBuffer, obj?:Polyline):Polyline {
  return (obj || new Polyline()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsPolyline(bb:flatbuffers.ByteBuffer, obj?:Polyline):Polyline {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new Polyline()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

color(obj?:Color):Color|null {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? (obj || new Color()).__init(this.bb!.__indirect(this.bb_pos + offset), this.bb!) : null;
}

points(index: number, obj?:Vector2f):Vector2f|null {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? (obj || new Vector2f()).__init(this.bb!.__vector(this.bb_pos + offset) + index * 8, this.bb!) : null;
}

pointsLength():number {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? this.bb!.__vector_len(this.bb_pos + offset) : 0;
}

static startPolyline(builder:flatbuffers.Builder) {
  builder.startObject(2);
}

static addColor(builder:flatbuffers.Builder, colorOffset:flatbuffers.Offset) {
  builder.addFieldOffset(0, colorOffset, 0);
}

static addPoints(builder:flatbuffers.Builder, pointsOffset:flatbuffers.Offset) {
  builder.addFieldOffset(1, pointsOffset, 0);
}

static startPointsVector(builder:flatbuffers.Builder, numElems:number) {
  builder.startVector(8, numElems, 4);
}

static endPolyline(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  builder.requiredField(offset, 6) // points
  return offset;
}

static createPolyline(builder:flatbuffers.Builder, colorOffset:flatbuffers.Offset, pointsOffset:flatbuffers.Offset):flatbuffers.Offset {
  Polyline.startPolyline(builder);
  Polyline.addColor(builder, colorOffset);
  Polyline.addPoints(builder, pointsOffset);
  return Polyline.endPolyline(builder);
}
}
