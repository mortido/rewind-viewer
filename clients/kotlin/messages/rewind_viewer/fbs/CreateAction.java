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
public final class CreateAction extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_24_3_25(); }
  public static CreateAction getRootAsCreateAction(ByteBuffer _bb) { return getRootAsCreateAction(_bb, new CreateAction()); }
  public static CreateAction getRootAsCreateAction(ByteBuffer _bb, CreateAction obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public CreateAction __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public String name() { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer nameAsByteBuffer() { return __vector_as_bytebuffer(4, 1); }
  public ByteBuffer nameInByteBuffer(ByteBuffer _bb) { return __vector_in_bytebuffer(_bb, 4, 1); }
  public byte inputType() { int o = __offset(6); return o != 0 ? bb.get(o + bb_pos) : 0; }
  public Table input(Table obj) { int o = __offset(8); return o != 0 ? __union(obj, o + bb_pos) : null; }

  public static int createCreateAction(FlatBufferBuilder builder,
      int nameOffset,
      byte inputType,
      int inputOffset) {
    builder.startTable(3);
    CreateAction.addInput(builder, inputOffset);
    CreateAction.addName(builder, nameOffset);
    CreateAction.addInputType(builder, inputType);
    return CreateAction.endCreateAction(builder);
  }

  public static void startCreateAction(FlatBufferBuilder builder) { builder.startTable(3); }
  public static void addName(FlatBufferBuilder builder, int nameOffset) { builder.addOffset(0, nameOffset, 0); }
  public static void addInputType(FlatBufferBuilder builder, byte inputType) { builder.addByte(1, inputType, 0); }
  public static void addInput(FlatBufferBuilder builder, int inputOffset) { builder.addOffset(2, inputOffset, 0); }
  public static int endCreateAction(FlatBufferBuilder builder) {
    int o = builder.endTable();
    builder.required(o, 4);  // name
    return o;
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public CreateAction get(int j) { return get(new CreateAction(), j); }
    public CreateAction get(CreateAction obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}

