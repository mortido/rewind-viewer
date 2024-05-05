// automatically generated by the FlatBuffers compiler, do not modify

package rewind_viewer.fbs;

import com.google.flatbuffers.BaseVector;
import com.google.flatbuffers.BooleanVector;
import com.google.flatbuffers.ByteVector;
import com.google.flatbuffers.Constants;
import com.google.flatbuffers.DoubleVector;
import com.google.flatbuffers.FlatBufferBuilder;
import com.google.flatbuffers.FloatVector;
import com.google.flatbuffers.IntVector;
import com.google.flatbuffers.LongVector;
import com.google.flatbuffers.ShortVector;
import com.google.flatbuffers.StringVector;
import com.google.flatbuffers.Struct;
import com.google.flatbuffers.Table;
import com.google.flatbuffers.UnionVector;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

@SuppressWarnings("unused")
public final class Arc extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_24_3_25(); }
  public static Arc getRootAsArc(ByteBuffer _bb) { return getRootAsArc(_bb, new Arc()); }
  public static Arc getRootAsArc(ByteBuffer _bb, Arc obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public Arc __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public rewind_viewer.fbs.Color color() { return color(new rewind_viewer.fbs.Color()); }
  public rewind_viewer.fbs.Color color(rewind_viewer.fbs.Color obj) { int o = __offset(4); return o != 0 ? obj.__assign(__indirect(o + bb_pos), bb) : null; }
  public rewind_viewer.fbs.Vector2f center() { return center(new rewind_viewer.fbs.Vector2f()); }
  public rewind_viewer.fbs.Vector2f center(rewind_viewer.fbs.Vector2f obj) { int o = __offset(6); return o != 0 ? obj.__assign(o + bb_pos, bb) : null; }
  public float radius() { int o = __offset(8); return o != 0 ? bb.getFloat(o + bb_pos) : 0.0f; }
  public float startAngle() { int o = __offset(10); return o != 0 ? bb.getFloat(o + bb_pos) : 0.0f; }
  public float endAngle() { int o = __offset(12); return o != 0 ? bb.getFloat(o + bb_pos) : 0.0f; }

  public static void startArc(FlatBufferBuilder builder) { builder.startTable(5); }
  public static void addColor(FlatBufferBuilder builder, int colorOffset) { builder.addOffset(0, colorOffset, 0); }
  public static void addCenter(FlatBufferBuilder builder, int centerOffset) { builder.addStruct(1, centerOffset, 0); }
  public static void addRadius(FlatBufferBuilder builder, float radius) { builder.addFloat(2, radius, 0.0f); }
  public static void addStartAngle(FlatBufferBuilder builder, float startAngle) { builder.addFloat(3, startAngle, 0.0f); }
  public static void addEndAngle(FlatBufferBuilder builder, float endAngle) { builder.addFloat(4, endAngle, 0.0f); }
  public static int endArc(FlatBufferBuilder builder) {
    int o = builder.endTable();
    builder.required(o, 6);  // center
    return o;
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public Arc get(int j) { return get(new Arc(), j); }
    public Arc get(Arc obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
  public ArcT unpack() {
    ArcT _o = new ArcT();
    unpackTo(_o);
    return _o;
  }
  public void unpackTo(ArcT _o) {
    if (color() != null) _o.setColor(color().unpack());
    else _o.setColor(null);
    if (center() != null) center().unpackTo(_o.getCenter());
    else _o.setCenter(null);
    float _oRadius = radius();
    _o.setRadius(_oRadius);
    float _oStartAngle = startAngle();
    _o.setStartAngle(_oStartAngle);
    float _oEndAngle = endAngle();
    _o.setEndAngle(_oEndAngle);
  }
  public static int pack(FlatBufferBuilder builder, ArcT _o) {
    if (_o == null) return 0;
    int _color = _o.getColor() == null ? 0 : rewind_viewer.fbs.Color.pack(builder, _o.getColor());
    startArc(builder);
    addColor(builder, _color);
    addCenter(builder, rewind_viewer.fbs.Vector2f.pack(builder, _o.getCenter()));
    addRadius(builder, _o.getRadius());
    addStartAngle(builder, _o.getStartAngle());
    addEndAngle(builder, _o.getEndAngle());
    return endArc(builder);
  }
}

