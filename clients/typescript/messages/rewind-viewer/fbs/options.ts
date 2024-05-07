// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

import { Layer } from '../../rewind-viewer/fbs/layer.js';
import { Map } from '../../rewind-viewer/fbs/map.js';


export class Options {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):Options {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsOptions(bb:flatbuffers.ByteBuffer, obj?:Options):Options {
  return (obj || new Options()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsOptions(bb:flatbuffers.ByteBuffer, obj?:Options):Options {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new Options()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

map(obj?:Map):Map|null {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? (obj || new Map()).__init(this.bb!.__indirect(this.bb_pos + offset), this.bb!) : null;
}

layer(obj?:Layer):Layer|null {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? (obj || new Layer()).__init(this.bb!.__indirect(this.bb_pos + offset), this.bb!) : null;
}

static startOptions(builder:flatbuffers.Builder) {
  builder.startObject(2);
}

static addMap(builder:flatbuffers.Builder, mapOffset:flatbuffers.Offset) {
  builder.addFieldOffset(0, mapOffset, 0);
}

static addLayer(builder:flatbuffers.Builder, layerOffset:flatbuffers.Offset) {
  builder.addFieldOffset(1, layerOffset, 0);
}

static endOptions(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

}
