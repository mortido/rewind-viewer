// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace rewind_viewer.fbs
{

using global::System;
using global::System.Collections.Generic;
using global::Google.FlatBuffers;

public struct RewindMessage : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_24_3_25(); }
  public static RewindMessage GetRootAsRewindMessage(ByteBuffer _bb) { return GetRootAsRewindMessage(_bb, new RewindMessage()); }
  public static RewindMessage GetRootAsRewindMessage(ByteBuffer _bb, RewindMessage obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public static bool VerifyRewindMessage(ByteBuffer _bb) {Google.FlatBuffers.Verifier verifier = new Google.FlatBuffers.Verifier(_bb); return verifier.VerifyBuffer("", false, RewindMessageVerify.Verify); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public RewindMessage __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public rewind_viewer.fbs.Command CommandType { get { int o = __p.__offset(4); return o != 0 ? (rewind_viewer.fbs.Command)__p.bb.Get(o + __p.bb_pos) : rewind_viewer.fbs.Command.NONE; } }
  public TTable? Command<TTable>() where TTable : struct, IFlatbufferObject { int o = __p.__offset(6); return o != 0 ? (TTable?)__p.__union<TTable>(o + __p.bb_pos) : null; }
  public rewind_viewer.fbs.Arc CommandAsArc() { return Command<rewind_viewer.fbs.Arc>().Value; }
  public rewind_viewer.fbs.CameraView CommandAsCameraView() { return Command<rewind_viewer.fbs.CameraView>().Value; }
  public rewind_viewer.fbs.Circle CommandAsCircle() { return Command<rewind_viewer.fbs.Circle>().Value; }
  public rewind_viewer.fbs.CircleSegment CommandAsCircleSegment() { return Command<rewind_viewer.fbs.CircleSegment>().Value; }
  public rewind_viewer.fbs.LogText CommandAsLogText() { return Command<rewind_viewer.fbs.LogText>().Value; }
  public rewind_viewer.fbs.Options CommandAsOptions() { return Command<rewind_viewer.fbs.Options>().Value; }
  public rewind_viewer.fbs.Polyline CommandAsPolyline() { return Command<rewind_viewer.fbs.Polyline>().Value; }
  public rewind_viewer.fbs.Popup CommandAsPopup() { return Command<rewind_viewer.fbs.Popup>().Value; }
  public rewind_viewer.fbs.PopupRound CommandAsPopupRound() { return Command<rewind_viewer.fbs.PopupRound>().Value; }
  public rewind_viewer.fbs.ReadEvents CommandAsReadEvents() { return Command<rewind_viewer.fbs.ReadEvents>().Value; }
  public rewind_viewer.fbs.Rectangle CommandAsRectangle() { return Command<rewind_viewer.fbs.Rectangle>().Value; }
  public rewind_viewer.fbs.Subscribe CommandAsSubscribe() { return Command<rewind_viewer.fbs.Subscribe>().Value; }
  public rewind_viewer.fbs.Tiles CommandAsTiles() { return Command<rewind_viewer.fbs.Tiles>().Value; }
  public rewind_viewer.fbs.Triangle CommandAsTriangle() { return Command<rewind_viewer.fbs.Triangle>().Value; }
  public rewind_viewer.fbs.Unsubscribe CommandAsUnsubscribe() { return Command<rewind_viewer.fbs.Unsubscribe>().Value; }
  public rewind_viewer.fbs.Primitives CommandAsPrimitives() { return Command<rewind_viewer.fbs.Primitives>().Value; }
  public rewind_viewer.fbs.EndFrame CommandAsEndFrame() { return Command<rewind_viewer.fbs.EndFrame>().Value; }

  public static Offset<rewind_viewer.fbs.RewindMessage> CreateRewindMessage(FlatBufferBuilder builder,
      rewind_viewer.fbs.Command command_type = rewind_viewer.fbs.Command.NONE,
      int commandOffset = 0) {
    builder.StartTable(2);
    RewindMessage.AddCommand(builder, commandOffset);
    RewindMessage.AddCommandType(builder, command_type);
    return RewindMessage.EndRewindMessage(builder);
  }

  public static void StartRewindMessage(FlatBufferBuilder builder) { builder.StartTable(2); }
  public static void AddCommandType(FlatBufferBuilder builder, rewind_viewer.fbs.Command commandType) { builder.AddByte(0, (byte)commandType, 0); }
  public static void AddCommand(FlatBufferBuilder builder, int commandOffset) { builder.AddOffset(1, commandOffset, 0); }
  public static Offset<rewind_viewer.fbs.RewindMessage> EndRewindMessage(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    builder.Required(o, 6);  // command
    return new Offset<rewind_viewer.fbs.RewindMessage>(o);
  }
  public static void FinishRewindMessageBuffer(FlatBufferBuilder builder, Offset<rewind_viewer.fbs.RewindMessage> offset) { builder.Finish(offset.Value); }
  public static void FinishSizePrefixedRewindMessageBuffer(FlatBufferBuilder builder, Offset<rewind_viewer.fbs.RewindMessage> offset) { builder.FinishSizePrefixed(offset.Value); }
}


static public class RewindMessageVerify
{
  static public bool Verify(Google.FlatBuffers.Verifier verifier, uint tablePos)
  {
    return verifier.VerifyTableStart(tablePos)
      && verifier.VerifyField(tablePos, 4 /*CommandType*/, 1 /*rewind_viewer.fbs.Command*/, 1, false)
      && verifier.VerifyUnion(tablePos, 4, 6 /*Command*/, rewind_viewer.fbs.CommandVerify.Verify, true)
      && verifier.VerifyTableEnd(tablePos);
  }
}

}
