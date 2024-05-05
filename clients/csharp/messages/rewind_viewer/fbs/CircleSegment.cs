// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace rewind_viewer.fbs
{

using global::System;
using global::System.Collections.Generic;
using global::Google.FlatBuffers;

public struct CircleSegment : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_24_3_25(); }
  public static CircleSegment GetRootAsCircleSegment(ByteBuffer _bb) { return GetRootAsCircleSegment(_bb, new CircleSegment()); }
  public static CircleSegment GetRootAsCircleSegment(ByteBuffer _bb, CircleSegment obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public CircleSegment __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public rewind_viewer.fbs.Color? Color { get { int o = __p.__offset(4); return o != 0 ? (rewind_viewer.fbs.Color?)(new rewind_viewer.fbs.Color()).__assign(__p.__indirect(o + __p.bb_pos), __p.bb) : null; } }
  public rewind_viewer.fbs.Vector2f? Center { get { int o = __p.__offset(6); return o != 0 ? (rewind_viewer.fbs.Vector2f?)(new rewind_viewer.fbs.Vector2f()).__assign(o + __p.bb_pos, __p.bb) : null; } }
  public float Radius { get { int o = __p.__offset(8); return o != 0 ? __p.bb.GetFloat(o + __p.bb_pos) : (float)0.0f; } }
  public float StartAngle { get { int o = __p.__offset(10); return o != 0 ? __p.bb.GetFloat(o + __p.bb_pos) : (float)0.0f; } }
  public float EndAngle { get { int o = __p.__offset(12); return o != 0 ? __p.bb.GetFloat(o + __p.bb_pos) : (float)0.0f; } }

  public static Offset<rewind_viewer.fbs.CircleSegment> CreateCircleSegment(FlatBufferBuilder builder,
      Offset<rewind_viewer.fbs.Color> colorOffset = default(Offset<rewind_viewer.fbs.Color>),
      rewind_viewer.fbs.Vector2fT center = null,
      float radius = 0.0f,
      float start_angle = 0.0f,
      float end_angle = 0.0f) {
    builder.StartTable(5);
    CircleSegment.AddEndAngle(builder, end_angle);
    CircleSegment.AddStartAngle(builder, start_angle);
    CircleSegment.AddRadius(builder, radius);
    CircleSegment.AddCenter(builder, rewind_viewer.fbs.Vector2f.Pack(builder, center));
    CircleSegment.AddColor(builder, colorOffset);
    return CircleSegment.EndCircleSegment(builder);
  }

  public static void StartCircleSegment(FlatBufferBuilder builder) { builder.StartTable(5); }
  public static void AddColor(FlatBufferBuilder builder, Offset<rewind_viewer.fbs.Color> colorOffset) { builder.AddOffset(0, colorOffset.Value, 0); }
  public static void AddCenter(FlatBufferBuilder builder, Offset<rewind_viewer.fbs.Vector2f> centerOffset) { builder.AddStruct(1, centerOffset.Value, 0); }
  public static void AddRadius(FlatBufferBuilder builder, float radius) { builder.AddFloat(2, radius, 0.0f); }
  public static void AddStartAngle(FlatBufferBuilder builder, float startAngle) { builder.AddFloat(3, startAngle, 0.0f); }
  public static void AddEndAngle(FlatBufferBuilder builder, float endAngle) { builder.AddFloat(4, endAngle, 0.0f); }
  public static Offset<rewind_viewer.fbs.CircleSegment> EndCircleSegment(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    builder.Required(o, 6);  // center
    return new Offset<rewind_viewer.fbs.CircleSegment>(o);
  }
  public CircleSegmentT UnPack() {
    var _o = new CircleSegmentT();
    this.UnPackTo(_o);
    return _o;
  }
  public void UnPackTo(CircleSegmentT _o) {
    _o.Color = this.Color.HasValue ? this.Color.Value.UnPack() : null;
    _o.Center = this.Center.HasValue ? this.Center.Value.UnPack() : null;
    _o.Radius = this.Radius;
    _o.StartAngle = this.StartAngle;
    _o.EndAngle = this.EndAngle;
  }
  public static Offset<rewind_viewer.fbs.CircleSegment> Pack(FlatBufferBuilder builder, CircleSegmentT _o) {
    if (_o == null) return default(Offset<rewind_viewer.fbs.CircleSegment>);
    var _color = _o.Color == null ? default(Offset<rewind_viewer.fbs.Color>) : rewind_viewer.fbs.Color.Pack(builder, _o.Color);
    return CreateCircleSegment(
      builder,
      _color,
      _o.Center,
      _o.Radius,
      _o.StartAngle,
      _o.EndAngle);
  }
}

public class CircleSegmentT
{
  public rewind_viewer.fbs.ColorT Color { get; set; }
  public rewind_viewer.fbs.Vector2fT Center { get; set; }
  public float Radius { get; set; }
  public float StartAngle { get; set; }
  public float EndAngle { get; set; }

  public CircleSegmentT() {
    this.Color = null;
    this.Center = new rewind_viewer.fbs.Vector2fT();
    this.Radius = 0.0f;
    this.StartAngle = 0.0f;
    this.EndAngle = 0.0f;
  }
}


static public class CircleSegmentVerify
{
  static public bool Verify(Google.FlatBuffers.Verifier verifier, uint tablePos)
  {
    return verifier.VerifyTableStart(tablePos)
      && verifier.VerifyTable(tablePos, 4 /*Color*/, rewind_viewer.fbs.ColorVerify.Verify, false)
      && verifier.VerifyField(tablePos, 6 /*Center*/, 8 /*rewind_viewer.fbs.Vector2f*/, 4, true)
      && verifier.VerifyField(tablePos, 8 /*Radius*/, 4 /*float*/, 4, false)
      && verifier.VerifyField(tablePos, 10 /*StartAngle*/, 4 /*float*/, 4, false)
      && verifier.VerifyField(tablePos, 12 /*EndAngle*/, 4 /*float*/, 4, false)
      && verifier.VerifyTableEnd(tablePos);
  }
}

}
