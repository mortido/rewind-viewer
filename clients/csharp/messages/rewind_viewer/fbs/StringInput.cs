// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace rewind_viewer.fbs
{

using global::System;
using global::System.Collections.Generic;
using global::Google.FlatBuffers;

public struct StringInput : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_24_3_25(); }
  public static StringInput GetRootAsStringInput(ByteBuffer _bb) { return GetRootAsStringInput(_bb, new StringInput()); }
  public static StringInput GetRootAsStringInput(ByteBuffer _bb, StringInput obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public StringInput __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public string DefaultValue { get { int o = __p.__offset(4); return o != 0 ? __p.__string(o + __p.bb_pos) : null; } }
#if ENABLE_SPAN_T
  public Span<byte> GetDefaultValueBytes() { return __p.__vector_as_span<byte>(4, 1); }
#else
  public ArraySegment<byte>? GetDefaultValueBytes() { return __p.__vector_as_arraysegment(4); }
#endif
  public byte[] GetDefaultValueArray() { return __p.__vector_as_array<byte>(4); }

  public static Offset<rewind_viewer.fbs.StringInput> CreateStringInput(FlatBufferBuilder builder,
      StringOffset default_valueOffset = default(StringOffset)) {
    builder.StartTable(1);
    StringInput.AddDefaultValue(builder, default_valueOffset);
    return StringInput.EndStringInput(builder);
  }

  public static void StartStringInput(FlatBufferBuilder builder) { builder.StartTable(1); }
  public static void AddDefaultValue(FlatBufferBuilder builder, StringOffset defaultValueOffset) { builder.AddOffset(0, defaultValueOffset.Value, 0); }
  public static Offset<rewind_viewer.fbs.StringInput> EndStringInput(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<rewind_viewer.fbs.StringInput>(o);
  }
}


static public class StringInputVerify
{
  static public bool Verify(Google.FlatBuffers.Verifier verifier, uint tablePos)
  {
    return verifier.VerifyTableStart(tablePos)
      && verifier.VerifyString(tablePos, 4 /*DefaultValue*/, false)
      && verifier.VerifyTableEnd(tablePos);
  }
}

}
