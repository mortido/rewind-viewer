// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace rewind_viewer.fbs
{

using global::System;
using global::System.Collections.Generic;
using global::Google.FlatBuffers;

public struct Triangle : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_24_3_25(); }
  public static Triangle GetRootAsTriangle(ByteBuffer _bb) { return GetRootAsTriangle(_bb, new Triangle()); }
  public static Triangle GetRootAsTriangle(ByteBuffer _bb, Triangle obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public Triangle __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public rewind_viewer.fbs.Color? Color { get { int o = __p.__offset(4); return o != 0 ? (rewind_viewer.fbs.Color?)(new rewind_viewer.fbs.Color()).__assign(__p.__indirect(o + __p.bb_pos), __p.bb) : null; } }
  public rewind_viewer.fbs.Vector2f? Points(int j) { int o = __p.__offset(6); return o != 0 ? (rewind_viewer.fbs.Vector2f?)(new rewind_viewer.fbs.Vector2f()).__assign(__p.__vector(o) + j * 8, __p.bb) : null; }
  public int PointsLength { get { int o = __p.__offset(6); return o != 0 ? __p.__vector_len(o) : 0; } }

  public static Offset<rewind_viewer.fbs.Triangle> CreateTriangle(FlatBufferBuilder builder,
      Offset<rewind_viewer.fbs.Color> colorOffset = default(Offset<rewind_viewer.fbs.Color>),
      VectorOffset pointsOffset = default(VectorOffset)) {
    builder.StartTable(2);
    Triangle.AddPoints(builder, pointsOffset);
    Triangle.AddColor(builder, colorOffset);
    return Triangle.EndTriangle(builder);
  }

  public static void StartTriangle(FlatBufferBuilder builder) { builder.StartTable(2); }
  public static void AddColor(FlatBufferBuilder builder, Offset<rewind_viewer.fbs.Color> colorOffset) { builder.AddOffset(0, colorOffset.Value, 0); }
  public static void AddPoints(FlatBufferBuilder builder, VectorOffset pointsOffset) { builder.AddOffset(1, pointsOffset.Value, 0); }
  public static void StartPointsVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(8, numElems, 4); }
  public static Offset<rewind_viewer.fbs.Triangle> EndTriangle(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    builder.Required(o, 4);  // color
    builder.Required(o, 6);  // points
    return new Offset<rewind_viewer.fbs.Triangle>(o);
  }
  public TriangleT UnPack() {
    var _o = new TriangleT();
    this.UnPackTo(_o);
    return _o;
  }
  public void UnPackTo(TriangleT _o) {
    _o.Color = this.Color.HasValue ? this.Color.Value.UnPack() : null;
    _o.Points = new List<rewind_viewer.fbs.Vector2fT>();
    for (var _j = 0; _j < this.PointsLength; ++_j) {_o.Points.Add(this.Points(_j).HasValue ? this.Points(_j).Value.UnPack() : null);}
  }
  public static Offset<rewind_viewer.fbs.Triangle> Pack(FlatBufferBuilder builder, TriangleT _o) {
    if (_o == null) return default(Offset<rewind_viewer.fbs.Triangle>);
    var _color = _o.Color == null ? default(Offset<rewind_viewer.fbs.Color>) : rewind_viewer.fbs.Color.Pack(builder, _o.Color);
    var _points = default(VectorOffset);
    if (_o.Points != null) {
      StartPointsVector(builder, _o.Points.Count);
      for (var _j = _o.Points.Count - 1; _j >= 0; --_j) { rewind_viewer.fbs.Vector2f.Pack(builder, _o.Points[_j]); }
      _points = builder.EndVector();
    }
    return CreateTriangle(
      builder,
      _color,
      _points);
  }
}

public class TriangleT
{
  public rewind_viewer.fbs.ColorT Color { get; set; }
  public List<rewind_viewer.fbs.Vector2fT> Points { get; set; }

  public TriangleT() {
    this.Color = null;
    this.Points = null;
  }
}


static public class TriangleVerify
{
  static public bool Verify(Google.FlatBuffers.Verifier verifier, uint tablePos)
  {
    return verifier.VerifyTableStart(tablePos)
      && verifier.VerifyTable(tablePos, 4 /*Color*/, rewind_viewer.fbs.ColorVerify.Verify, true)
      && verifier.VerifyVectorOfData(tablePos, 6 /*Points*/, 8 /*rewind_viewer.fbs.Vector2f*/, true)
      && verifier.VerifyTableEnd(tablePos);
  }
}

}
