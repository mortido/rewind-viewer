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

public class LayerT {
  private long id;
  private boolean usePermanentFrame;

  public long getId() { return id; }

  public void setId(long id) { this.id = id; }

  public boolean getUsePermanentFrame() { return usePermanentFrame; }

  public void setUsePermanentFrame(boolean usePermanentFrame) { this.usePermanentFrame = usePermanentFrame; }


  public LayerT() {
    this.id = 0L;
    this.usePermanentFrame = false;
  }
}

