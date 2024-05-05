// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

import { Arc, ArcT } from '../../rewind-viewer/fbs/arc.js';
import { CameraView, CameraViewT } from '../../rewind-viewer/fbs/camera-view.js';
import { Circle, CircleT } from '../../rewind-viewer/fbs/circle.js';
import { CircleSegment, CircleSegmentT } from '../../rewind-viewer/fbs/circle-segment.js';
import { Command, unionToCommand, unionListToCommand } from '../../rewind-viewer/fbs/command.js';
import { EndFrame, EndFrameT } from '../../rewind-viewer/fbs/end-frame.js';
import { LogText, LogTextT } from '../../rewind-viewer/fbs/log-text.js';
import { Options, OptionsT } from '../../rewind-viewer/fbs/options.js';
import { Polyline, PolylineT } from '../../rewind-viewer/fbs/polyline.js';
import { Popup, PopupT } from '../../rewind-viewer/fbs/popup.js';
import { PopupRound, PopupRoundT } from '../../rewind-viewer/fbs/popup-round.js';
import { Rectangle, RectangleT } from '../../rewind-viewer/fbs/rectangle.js';
import { Tiles, TilesT } from '../../rewind-viewer/fbs/tiles.js';
import { Triangle, TriangleT } from '../../rewind-viewer/fbs/triangle.js';


export class RewindMessage implements flatbuffers.IUnpackableObject<RewindMessageT> {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):RewindMessage {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsRewindMessage(bb:flatbuffers.ByteBuffer, obj?:RewindMessage):RewindMessage {
  return (obj || new RewindMessage()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsRewindMessage(bb:flatbuffers.ByteBuffer, obj?:RewindMessage):RewindMessage {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new RewindMessage()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

commandType():Command {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readUint8(this.bb_pos + offset) : Command.NONE;
}

command<T extends flatbuffers.Table>(obj:any):any|null {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? this.bb!.__union(obj, this.bb_pos + offset) : null;
}

static startRewindMessage(builder:flatbuffers.Builder) {
  builder.startObject(2);
}

static addCommandType(builder:flatbuffers.Builder, commandType:Command) {
  builder.addFieldInt8(0, commandType, Command.NONE);
}

static addCommand(builder:flatbuffers.Builder, commandOffset:flatbuffers.Offset) {
  builder.addFieldOffset(1, commandOffset, 0);
}

static endRewindMessage(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  builder.requiredField(offset, 6) // command
  return offset;
}

static finishRewindMessageBuffer(builder:flatbuffers.Builder, offset:flatbuffers.Offset) {
  builder.finish(offset);
}

static finishSizePrefixedRewindMessageBuffer(builder:flatbuffers.Builder, offset:flatbuffers.Offset) {
  builder.finish(offset, undefined, true);
}

static createRewindMessage(builder:flatbuffers.Builder, commandType:Command, commandOffset:flatbuffers.Offset):flatbuffers.Offset {
  RewindMessage.startRewindMessage(builder);
  RewindMessage.addCommandType(builder, commandType);
  RewindMessage.addCommand(builder, commandOffset);
  return RewindMessage.endRewindMessage(builder);
}

unpack(): RewindMessageT {
  return new RewindMessageT(
    this.commandType(),
    (() => {
      const temp = unionToCommand(this.commandType(), this.command.bind(this));
      if(temp === null) { return null; }
      return temp.unpack()
  })()
  );
}


unpackTo(_o: RewindMessageT): void {
  _o.commandType = this.commandType();
  _o.command = (() => {
      const temp = unionToCommand(this.commandType(), this.command.bind(this));
      if(temp === null) { return null; }
      return temp.unpack()
  })();
}
}

export class RewindMessageT implements flatbuffers.IGeneratedObject {
constructor(
  public commandType: Command = Command.NONE,
  public command: ArcT|CameraViewT|CircleSegmentT|CircleT|EndFrameT|LogTextT|OptionsT|PolylineT|PopupRoundT|PopupT|RectangleT|TilesT|TriangleT|null = null
){}


pack(builder:flatbuffers.Builder): flatbuffers.Offset {
  const command = builder.createObjectOffset(this.command);

  return RewindMessage.createRewindMessage(builder,
    this.commandType,
    command
  );
}
}
