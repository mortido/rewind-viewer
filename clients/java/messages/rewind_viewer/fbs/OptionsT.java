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

public class OptionsT {
  private rewind_viewer.fbs.MapT map;
  private rewind_viewer.fbs.LayerT layer;

  public rewind_viewer.fbs.MapT getMap() { return map; }

  public void setMap(rewind_viewer.fbs.MapT map) { this.map = map; }

  public rewind_viewer.fbs.LayerT getLayer() { return layer; }

  public void setLayer(rewind_viewer.fbs.LayerT layer) { this.layer = layer; }


  public OptionsT() {
    this.map = null;
    this.layer = null;
  }
}

