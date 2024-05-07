// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace rewind_viewer.fbs
{

using global::System;
using global::System.Collections.Generic;
using global::Google.FlatBuffers;

public struct CameraView : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_24_3_25(); }
  public static CameraView GetRootAsCameraView(ByteBuffer _bb) { return GetRootAsCameraView(_bb, new CameraView()); }
  public static CameraView GetRootAsCameraView(ByteBuffer _bb, CameraView obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public CameraView __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public string Name { get { int o = __p.__offset(4); return o != 0 ? __p.__string(o + __p.bb_pos) : null; } }
#if ENABLE_SPAN_T
  public Span<byte> GetNameBytes() { return __p.__vector_as_span<byte>(4, 1); }
#else
  public ArraySegment<byte>? GetNameBytes() { return __p.__vector_as_arraysegment(4); }
#endif
  public byte[] GetNameArray() { return __p.__vector_as_array<byte>(4); }
  public rewind_viewer.fbs.Vector2f? Position { get { int o = __p.__offset(6); return o != 0 ? (rewind_viewer.fbs.Vector2f?)(new rewind_viewer.fbs.Vector2f()).__assign(o + __p.bb_pos, __p.bb) : null; } }
  public float ViewRadius { get { int o = __p.__offset(8); return o != 0 ? __p.bb.GetFloat(o + __p.bb_pos) : (float)0.0f; } }

  public static void StartCameraView(FlatBufferBuilder builder) { builder.StartTable(3); }
  public static void AddName(FlatBufferBuilder builder, StringOffset nameOffset) { builder.AddOffset(0, nameOffset.Value, 0); }
  public static void AddPosition(FlatBufferBuilder builder, Offset<rewind_viewer.fbs.Vector2f> positionOffset) { builder.AddStruct(1, positionOffset.Value, 0); }
  public static void AddViewRadius(FlatBufferBuilder builder, float viewRadius) { builder.AddFloat(2, viewRadius, 0.0f); }
  public static Offset<rewind_viewer.fbs.CameraView> EndCameraView(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    builder.Required(o, 4);  // name
    builder.Required(o, 6);  // position
    return new Offset<rewind_viewer.fbs.CameraView>(o);
  }
}


static public class CameraViewVerify
{
  static public bool Verify(Google.FlatBuffers.Verifier verifier, uint tablePos)
  {
    return verifier.VerifyTableStart(tablePos)
      && verifier.VerifyString(tablePos, 4 /*Name*/, true)
      && verifier.VerifyField(tablePos, 6 /*Position*/, 8 /*rewind_viewer.fbs.Vector2f*/, 4, true)
      && verifier.VerifyField(tablePos, 8 /*ViewRadius*/, 4 /*float*/, 4, false)
      && verifier.VerifyTableEnd(tablePos);
  }
}

}
