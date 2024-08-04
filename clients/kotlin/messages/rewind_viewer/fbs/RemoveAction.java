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
public final class RemoveAction extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_24_3_25(); }
  public static RemoveAction getRootAsRemoveAction(ByteBuffer _bb) { return getRootAsRemoveAction(_bb, new RemoveAction()); }
  public static RemoveAction getRootAsRemoveAction(ByteBuffer _bb, RemoveAction obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public RemoveAction __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public String name() { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer nameAsByteBuffer() { return __vector_as_bytebuffer(4, 1); }
  public ByteBuffer nameInByteBuffer(ByteBuffer _bb) { return __vector_in_bytebuffer(_bb, 4, 1); }

  public static int createRemoveAction(FlatBufferBuilder builder,
      int nameOffset) {
    builder.startTable(1);
    RemoveAction.addName(builder, nameOffset);
    return RemoveAction.endRemoveAction(builder);
  }

  public static void startRemoveAction(FlatBufferBuilder builder) { builder.startTable(1); }
  public static void addName(FlatBufferBuilder builder, int nameOffset) { builder.addOffset(0, nameOffset, 0); }
  public static int endRemoveAction(FlatBufferBuilder builder) {
    int o = builder.endTable();
    builder.required(o, 4);  // name
    return o;
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public RemoveAction get(int j) { return get(new RemoveAction(), j); }
    public RemoveAction get(RemoveAction obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}

