// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace rewind_viewer.fbs
{

using global::System;
using global::System.Collections.Generic;
using global::Google.FlatBuffers;

public struct Options : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_24_3_25(); }
  public static Options GetRootAsOptions(ByteBuffer _bb) { return GetRootAsOptions(_bb, new Options()); }
  public static Options GetRootAsOptions(ByteBuffer _bb, Options obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public Options __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public rewind_viewer.fbs.Map? Map { get { int o = __p.__offset(4); return o != 0 ? (rewind_viewer.fbs.Map?)(new rewind_viewer.fbs.Map()).__assign(__p.__indirect(o + __p.bb_pos), __p.bb) : null; } }
  public rewind_viewer.fbs.Layer? Layer { get { int o = __p.__offset(6); return o != 0 ? (rewind_viewer.fbs.Layer?)(new rewind_viewer.fbs.Layer()).__assign(__p.__indirect(o + __p.bb_pos), __p.bb) : null; } }

  public static Offset<rewind_viewer.fbs.Options> CreateOptions(FlatBufferBuilder builder,
      Offset<rewind_viewer.fbs.Map> mapOffset = default(Offset<rewind_viewer.fbs.Map>),
      Offset<rewind_viewer.fbs.Layer> layerOffset = default(Offset<rewind_viewer.fbs.Layer>)) {
    builder.StartTable(2);
    Options.AddLayer(builder, layerOffset);
    Options.AddMap(builder, mapOffset);
    return Options.EndOptions(builder);
  }

  public static void StartOptions(FlatBufferBuilder builder) { builder.StartTable(2); }
  public static void AddMap(FlatBufferBuilder builder, Offset<rewind_viewer.fbs.Map> mapOffset) { builder.AddOffset(0, mapOffset.Value, 0); }
  public static void AddLayer(FlatBufferBuilder builder, Offset<rewind_viewer.fbs.Layer> layerOffset) { builder.AddOffset(1, layerOffset.Value, 0); }
  public static Offset<rewind_viewer.fbs.Options> EndOptions(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<rewind_viewer.fbs.Options>(o);
  }
  public OptionsT UnPack() {
    var _o = new OptionsT();
    this.UnPackTo(_o);
    return _o;
  }
  public void UnPackTo(OptionsT _o) {
    _o.Map = this.Map.HasValue ? this.Map.Value.UnPack() : null;
    _o.Layer = this.Layer.HasValue ? this.Layer.Value.UnPack() : null;
  }
  public static Offset<rewind_viewer.fbs.Options> Pack(FlatBufferBuilder builder, OptionsT _o) {
    if (_o == null) return default(Offset<rewind_viewer.fbs.Options>);
    var _map = _o.Map == null ? default(Offset<rewind_viewer.fbs.Map>) : rewind_viewer.fbs.Map.Pack(builder, _o.Map);
    var _layer = _o.Layer == null ? default(Offset<rewind_viewer.fbs.Layer>) : rewind_viewer.fbs.Layer.Pack(builder, _o.Layer);
    return CreateOptions(
      builder,
      _map,
      _layer);
  }
}

public class OptionsT
{
  public rewind_viewer.fbs.MapT Map { get; set; }
  public rewind_viewer.fbs.LayerT Layer { get; set; }

  public OptionsT() {
    this.Map = null;
    this.Layer = null;
  }
}


static public class OptionsVerify
{
  static public bool Verify(Google.FlatBuffers.Verifier verifier, uint tablePos)
  {
    return verifier.VerifyTableStart(tablePos)
      && verifier.VerifyTable(tablePos, 4 /*Map*/, rewind_viewer.fbs.MapVerify.Verify, false)
      && verifier.VerifyTable(tablePos, 6 /*Layer*/, rewind_viewer.fbs.LayerVerify.Verify, false)
      && verifier.VerifyTableEnd(tablePos);
  }
}

}
