// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';



export class LogText implements flatbuffers.IUnpackableObject<LogTextT> {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):LogText {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsLogText(bb:flatbuffers.ByteBuffer, obj?:LogText):LogText {
  return (obj || new LogText()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsLogText(bb:flatbuffers.ByteBuffer, obj?:LogText):LogText {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new LogText()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

text():string|null
text(optionalEncoding:flatbuffers.Encoding):string|Uint8Array|null
text(optionalEncoding?:any):string|Uint8Array|null {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.__string(this.bb_pos + offset, optionalEncoding) : null;
}

static startLogText(builder:flatbuffers.Builder) {
  builder.startObject(1);
}

static addText(builder:flatbuffers.Builder, textOffset:flatbuffers.Offset) {
  builder.addFieldOffset(0, textOffset, 0);
}

static endLogText(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  builder.requiredField(offset, 4) // text
  return offset;
}

static createLogText(builder:flatbuffers.Builder, textOffset:flatbuffers.Offset):flatbuffers.Offset {
  LogText.startLogText(builder);
  LogText.addText(builder, textOffset);
  return LogText.endLogText(builder);
}

unpack(): LogTextT {
  return new LogTextT(
    this.text()
  );
}


unpackTo(_o: LogTextT): void {
  _o.text = this.text();
}
}

export class LogTextT implements flatbuffers.IGeneratedObject {
constructor(
  public text: string|Uint8Array|null = null
){}


pack(builder:flatbuffers.Builder): flatbuffers.Offset {
  const text = (this.text !== null ? builder.createString(this.text!) : 0);

  return LogText.createLogText(builder,
    text
  );
}
}
