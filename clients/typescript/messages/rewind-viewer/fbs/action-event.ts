// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

import { ActionValue, unionToActionValue, unionListToActionValue } from '../../rewind-viewer/fbs/action-value.js';


export class ActionEvent {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):ActionEvent {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsActionEvent(bb:flatbuffers.ByteBuffer, obj?:ActionEvent):ActionEvent {
  return (obj || new ActionEvent()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsActionEvent(bb:flatbuffers.ByteBuffer, obj?:ActionEvent):ActionEvent {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new ActionEvent()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

name():string|null
name(optionalEncoding:flatbuffers.Encoding):string|Uint8Array|null
name(optionalEncoding?:any):string|Uint8Array|null {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.__string(this.bb_pos + offset, optionalEncoding) : null;
}

valueType():ActionValue {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? this.bb!.readUint8(this.bb_pos + offset) : ActionValue.NONE;
}

value<T extends flatbuffers.Table>(obj:any):any|null {
  const offset = this.bb!.__offset(this.bb_pos, 8);
  return offset ? this.bb!.__union(obj, this.bb_pos + offset) : null;
}

static startActionEvent(builder:flatbuffers.Builder) {
  builder.startObject(3);
}

static addName(builder:flatbuffers.Builder, nameOffset:flatbuffers.Offset) {
  builder.addFieldOffset(0, nameOffset, 0);
}

static addValueType(builder:flatbuffers.Builder, valueType:ActionValue) {
  builder.addFieldInt8(1, valueType, ActionValue.NONE);
}

static addValue(builder:flatbuffers.Builder, valueOffset:flatbuffers.Offset) {
  builder.addFieldOffset(2, valueOffset, 0);
}

static endActionEvent(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static createActionEvent(builder:flatbuffers.Builder, nameOffset:flatbuffers.Offset, valueType:ActionValue, valueOffset:flatbuffers.Offset):flatbuffers.Offset {
  ActionEvent.startActionEvent(builder);
  ActionEvent.addName(builder, nameOffset);
  ActionEvent.addValueType(builder, valueType);
  ActionEvent.addValue(builder, valueOffset);
  return ActionEvent.endActionEvent(builder);
}
}
