// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace rewind_viewer.fbs
{

using global::System;
using global::System.Collections.Generic;
using global::Google.FlatBuffers;

public struct ButtonInput : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_24_3_25(); }
  public static ButtonInput GetRootAsButtonInput(ByteBuffer _bb) { return GetRootAsButtonInput(_bb, new ButtonInput()); }
  public static ButtonInput GetRootAsButtonInput(ByteBuffer _bb, ButtonInput obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public ButtonInput __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }


  public static void StartButtonInput(FlatBufferBuilder builder) { builder.StartTable(0); }
  public static Offset<rewind_viewer.fbs.ButtonInput> EndButtonInput(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<rewind_viewer.fbs.ButtonInput>(o);
  }
}


static public class ButtonInputVerify
{
  static public bool Verify(Google.FlatBuffers.Verifier verifier, uint tablePos)
  {
    return verifier.VerifyTableStart(tablePos)
      && verifier.VerifyTableEnd(tablePos);
  }
}

}
