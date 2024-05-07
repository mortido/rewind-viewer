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
public final class CameraView extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_24_3_25(); }
  public static CameraView getRootAsCameraView(ByteBuffer _bb) { return getRootAsCameraView(_bb, new CameraView()); }
  public static CameraView getRootAsCameraView(ByteBuffer _bb, CameraView obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public CameraView __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public String name() { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer nameAsByteBuffer() { return __vector_as_bytebuffer(4, 1); }
  public ByteBuffer nameInByteBuffer(ByteBuffer _bb) { return __vector_in_bytebuffer(_bb, 4, 1); }
  public rewind_viewer.fbs.Vector2f position() { return position(new rewind_viewer.fbs.Vector2f()); }
  public rewind_viewer.fbs.Vector2f position(rewind_viewer.fbs.Vector2f obj) { int o = __offset(6); return o != 0 ? obj.__assign(o + bb_pos, bb) : null; }
  public float viewRadius() { int o = __offset(8); return o != 0 ? bb.getFloat(o + bb_pos) : 0.0f; }

  public static void startCameraView(FlatBufferBuilder builder) { builder.startTable(3); }
  public static void addName(FlatBufferBuilder builder, int nameOffset) { builder.addOffset(0, nameOffset, 0); }
  public static void addPosition(FlatBufferBuilder builder, int positionOffset) { builder.addStruct(1, positionOffset, 0); }
  public static void addViewRadius(FlatBufferBuilder builder, float viewRadius) { builder.addFloat(2, viewRadius, 0.0f); }
  public static int endCameraView(FlatBufferBuilder builder) {
    int o = builder.endTable();
    builder.required(o, 4);  // name
    builder.required(o, 6);  // position
    return o;
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public CameraView get(int j) { return get(new CameraView(), j); }
    public CameraView get(CameraView obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}

