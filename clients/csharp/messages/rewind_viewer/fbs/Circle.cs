// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace rewind_viewer.fbs
{

using global::System;
using global::System.Collections.Generic;
using global::Google.FlatBuffers;

public struct Circle : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_24_3_25(); }
  public static Circle GetRootAsCircle(ByteBuffer _bb) { return GetRootAsCircle(_bb, new Circle()); }
  public static Circle GetRootAsCircle(ByteBuffer _bb, Circle obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public Circle __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public rewind_viewer.fbs.Color? Color { get { int o = __p.__offset(4); return o != 0 ? (rewind_viewer.fbs.Color?)(new rewind_viewer.fbs.Color()).__assign(__p.__indirect(o + __p.bb_pos), __p.bb) : null; } }
  public rewind_viewer.fbs.Vector2f? Center { get { int o = __p.__offset(6); return o != 0 ? (rewind_viewer.fbs.Vector2f?)(new rewind_viewer.fbs.Vector2f()).__assign(o + __p.bb_pos, __p.bb) : null; } }
  public float Radius { get { int o = __p.__offset(8); return o != 0 ? __p.bb.GetFloat(o + __p.bb_pos) : (float)0.0f; } }

  public static void StartCircle(FlatBufferBuilder builder) { builder.StartTable(3); }
  public static void AddColor(FlatBufferBuilder builder, Offset<rewind_viewer.fbs.Color> colorOffset) { builder.AddOffset(0, colorOffset.Value, 0); }
  public static void AddCenter(FlatBufferBuilder builder, Offset<rewind_viewer.fbs.Vector2f> centerOffset) { builder.AddStruct(1, centerOffset.Value, 0); }
  public static void AddRadius(FlatBufferBuilder builder, float radius) { builder.AddFloat(2, radius, 0.0f); }
  public static Offset<rewind_viewer.fbs.Circle> EndCircle(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    builder.Required(o, 6);  // center
    return new Offset<rewind_viewer.fbs.Circle>(o);
  }
}


static public class CircleVerify
{
  static public bool Verify(Google.FlatBuffers.Verifier verifier, uint tablePos)
  {
    return verifier.VerifyTableStart(tablePos)
      && verifier.VerifyTable(tablePos, 4 /*Color*/, rewind_viewer.fbs.ColorVerify.Verify, false)
      && verifier.VerifyField(tablePos, 6 /*Center*/, 8 /*rewind_viewer.fbs.Vector2f*/, 4, true)
      && verifier.VerifyField(tablePos, 8 /*Radius*/, 4 /*float*/, 4, false)
      && verifier.VerifyTableEnd(tablePos);
  }
}

}
