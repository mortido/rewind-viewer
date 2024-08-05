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
public final class Layer extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_24_3_25(); }
  public static Layer getRootAsLayer(ByteBuffer _bb) { return getRootAsLayer(_bb, new Layer()); }
  public static Layer getRootAsLayer(ByteBuffer _bb, Layer obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public Layer __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public long id() { int o = __offset(4); return o != 0 ? (long)bb.getInt(o + bb_pos) & 0xFFFFFFFFL : 0L; }
  public String name() { int o = __offset(6); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer nameAsByteBuffer() { return __vector_as_bytebuffer(6, 1); }
  public ByteBuffer nameInByteBuffer(ByteBuffer _bb) { return __vector_in_bytebuffer(_bb, 6, 1); }
  public boolean usePermanentFrame() { int o = __offset(8); return o != 0 ? 0!=bb.get(o + bb_pos) : false; }
  public byte origin() { int o = __offset(10); return o != 0 ? bb.get(o + bb_pos) : 0; }

  public static int createLayer(FlatBufferBuilder builder,
      long id,
      int nameOffset,
      boolean usePermanentFrame,
      byte origin) {
    builder.startTable(4);
    Layer.addName(builder, nameOffset);
    Layer.addId(builder, id);
    Layer.addOrigin(builder, origin);
    Layer.addUsePermanentFrame(builder, usePermanentFrame);
    return Layer.endLayer(builder);
  }

  public static void startLayer(FlatBufferBuilder builder) { builder.startTable(4); }
  public static void addId(FlatBufferBuilder builder, long id) { builder.addInt(0, (int) id, (int) 0L); }
  public static void addName(FlatBufferBuilder builder, int nameOffset) { builder.addOffset(1, nameOffset, 0); }
  public static void addUsePermanentFrame(FlatBufferBuilder builder, boolean usePermanentFrame) { builder.addBoolean(2, usePermanentFrame, false); }
  public static void addOrigin(FlatBufferBuilder builder, byte origin) { builder.addByte(3, origin, 0); }
  public static int endLayer(FlatBufferBuilder builder) {
    int o = builder.endTable();
    return o;
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public Layer get(int j) { return get(new Layer(), j); }
    public Layer get(Layer obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}

