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
public final class SelectInput extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_24_3_25(); }
  public static SelectInput getRootAsSelectInput(ByteBuffer _bb) { return getRootAsSelectInput(_bb, new SelectInput()); }
  public static SelectInput getRootAsSelectInput(ByteBuffer _bb, SelectInput obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public SelectInput __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public String options(int j) { int o = __offset(4); return o != 0 ? __string(__vector(o) + j * 4) : null; }
  public int optionsLength() { int o = __offset(4); return o != 0 ? __vector_len(o) : 0; }
  public StringVector optionsVector() { return optionsVector(new StringVector()); }
  public StringVector optionsVector(StringVector obj) { int o = __offset(4); return o != 0 ? obj.__assign(__vector(o), 4, bb) : null; }
  public int selectedOption() { int o = __offset(6); return o != 0 ? bb.getShort(o + bb_pos) & 0xFFFF : 0; }

  public static int createSelectInput(FlatBufferBuilder builder,
      int optionsOffset,
      int selectedOption) {
    builder.startTable(2);
    SelectInput.addOptions(builder, optionsOffset);
    SelectInput.addSelectedOption(builder, selectedOption);
    return SelectInput.endSelectInput(builder);
  }

  public static void startSelectInput(FlatBufferBuilder builder) { builder.startTable(2); }
  public static void addOptions(FlatBufferBuilder builder, int optionsOffset) { builder.addOffset(0, optionsOffset, 0); }
  public static int createOptionsVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startOptionsVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static void addSelectedOption(FlatBufferBuilder builder, int selectedOption) { builder.addShort(1, (short) selectedOption, (short) 0); }
  public static int endSelectInput(FlatBufferBuilder builder) {
    int o = builder.endTable();
    builder.required(o, 4);  // options
    return o;
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public SelectInput get(int j) { return get(new SelectInput(), j); }
    public SelectInput get(SelectInput obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}

