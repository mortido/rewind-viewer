// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace rewind_viewer.fbs
{

using global::System;
using global::System.Collections.Generic;
using global::Google.FlatBuffers;

public struct Subscribe : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_24_3_25(); }
  public static Subscribe GetRootAsSubscribe(ByteBuffer _bb) { return GetRootAsSubscribe(_bb, new Subscribe()); }
  public static Subscribe GetRootAsSubscribe(ByteBuffer _bb, Subscribe obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public Subscribe __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public sbyte Key { get { int o = __p.__offset(4); return o != 0 ? __p.bb.GetSbyte(o + __p.bb_pos) : (sbyte)0; } }
  public string Name { get { int o = __p.__offset(6); return o != 0 ? __p.__string(o + __p.bb_pos) : null; } }
#if ENABLE_SPAN_T
  public Span<byte> GetNameBytes() { return __p.__vector_as_span<byte>(6, 1); }
#else
  public ArraySegment<byte>? GetNameBytes() { return __p.__vector_as_arraysegment(6); }
#endif
  public byte[] GetNameArray() { return __p.__vector_as_array<byte>(6); }
  public bool Continuous { get { int o = __p.__offset(8); return o != 0 ? 0!=__p.bb.Get(o + __p.bb_pos) : (bool)false; } }
  public bool CaptureMouse { get { int o = __p.__offset(10); return o != 0 ? 0!=__p.bb.Get(o + __p.bb_pos) : (bool)false; } }

  public static Offset<rewind_viewer.fbs.Subscribe> CreateSubscribe(FlatBufferBuilder builder,
      sbyte key = 0,
      StringOffset nameOffset = default(StringOffset),
      bool continuous = false,
      bool capture_mouse = false) {
    builder.StartTable(4);
    Subscribe.AddName(builder, nameOffset);
    Subscribe.AddCaptureMouse(builder, capture_mouse);
    Subscribe.AddContinuous(builder, continuous);
    Subscribe.AddKey(builder, key);
    return Subscribe.EndSubscribe(builder);
  }

  public static void StartSubscribe(FlatBufferBuilder builder) { builder.StartTable(4); }
  public static void AddKey(FlatBufferBuilder builder, sbyte key) { builder.AddSbyte(0, key, 0); }
  public static void AddName(FlatBufferBuilder builder, StringOffset nameOffset) { builder.AddOffset(1, nameOffset.Value, 0); }
  public static void AddContinuous(FlatBufferBuilder builder, bool continuous) { builder.AddBool(2, continuous, false); }
  public static void AddCaptureMouse(FlatBufferBuilder builder, bool captureMouse) { builder.AddBool(3, captureMouse, false); }
  public static Offset<rewind_viewer.fbs.Subscribe> EndSubscribe(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    builder.Required(o, 6);  // name
    return new Offset<rewind_viewer.fbs.Subscribe>(o);
  }
}


static public class SubscribeVerify
{
  static public bool Verify(Google.FlatBuffers.Verifier verifier, uint tablePos)
  {
    return verifier.VerifyTableStart(tablePos)
      && verifier.VerifyField(tablePos, 4 /*Key*/, 1 /*sbyte*/, 1, false)
      && verifier.VerifyString(tablePos, 6 /*Name*/, true)
      && verifier.VerifyField(tablePos, 8 /*Continuous*/, 1 /*bool*/, 1, false)
      && verifier.VerifyField(tablePos, 10 /*CaptureMouse*/, 1 /*bool*/, 1, false)
      && verifier.VerifyTableEnd(tablePos);
  }
}

}
