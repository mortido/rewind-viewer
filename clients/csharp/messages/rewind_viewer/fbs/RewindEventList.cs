// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace rewind_viewer.fbs
{

using global::System;
using global::System.Collections.Generic;
using global::Google.FlatBuffers;

public struct RewindEventList : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_24_3_25(); }
  public static RewindEventList GetRootAsRewindEventList(ByteBuffer _bb) { return GetRootAsRewindEventList(_bb, new RewindEventList()); }
  public static RewindEventList GetRootAsRewindEventList(ByteBuffer _bb, RewindEventList obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public static bool VerifyRewindEventList(ByteBuffer _bb) {Google.FlatBuffers.Verifier verifier = new Google.FlatBuffers.Verifier(_bb); return verifier.VerifyBuffer("", false, RewindEventListVerify.Verify); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public RewindEventList __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public rewind_viewer.fbs.KeyEvent? KeyEvents(int j) { int o = __p.__offset(4); return o != 0 ? (rewind_viewer.fbs.KeyEvent?)(new rewind_viewer.fbs.KeyEvent()).__assign(__p.__indirect(__p.__vector(o) + j * 4), __p.bb) : null; }
  public int KeyEventsLength { get { int o = __p.__offset(4); return o != 0 ? __p.__vector_len(o) : 0; } }
  public rewind_viewer.fbs.ActionEvent? ActionEvents(int j) { int o = __p.__offset(6); return o != 0 ? (rewind_viewer.fbs.ActionEvent?)(new rewind_viewer.fbs.ActionEvent()).__assign(__p.__indirect(__p.__vector(o) + j * 4), __p.bb) : null; }
  public int ActionEventsLength { get { int o = __p.__offset(6); return o != 0 ? __p.__vector_len(o) : 0; } }

  public static Offset<rewind_viewer.fbs.RewindEventList> CreateRewindEventList(FlatBufferBuilder builder,
      VectorOffset key_eventsOffset = default(VectorOffset),
      VectorOffset action_eventsOffset = default(VectorOffset)) {
    builder.StartTable(2);
    RewindEventList.AddActionEvents(builder, action_eventsOffset);
    RewindEventList.AddKeyEvents(builder, key_eventsOffset);
    return RewindEventList.EndRewindEventList(builder);
  }

  public static void StartRewindEventList(FlatBufferBuilder builder) { builder.StartTable(2); }
  public static void AddKeyEvents(FlatBufferBuilder builder, VectorOffset keyEventsOffset) { builder.AddOffset(0, keyEventsOffset.Value, 0); }
  public static VectorOffset CreateKeyEventsVector(FlatBufferBuilder builder, Offset<rewind_viewer.fbs.KeyEvent>[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddOffset(data[i].Value); return builder.EndVector(); }
  public static VectorOffset CreateKeyEventsVectorBlock(FlatBufferBuilder builder, Offset<rewind_viewer.fbs.KeyEvent>[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateKeyEventsVectorBlock(FlatBufferBuilder builder, ArraySegment<Offset<rewind_viewer.fbs.KeyEvent>> data) { builder.StartVector(4, data.Count, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateKeyEventsVectorBlock(FlatBufferBuilder builder, IntPtr dataPtr, int sizeInBytes) { builder.StartVector(1, sizeInBytes, 1); builder.Add<Offset<rewind_viewer.fbs.KeyEvent>>(dataPtr, sizeInBytes); return builder.EndVector(); }
  public static void StartKeyEventsVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static void AddActionEvents(FlatBufferBuilder builder, VectorOffset actionEventsOffset) { builder.AddOffset(1, actionEventsOffset.Value, 0); }
  public static VectorOffset CreateActionEventsVector(FlatBufferBuilder builder, Offset<rewind_viewer.fbs.ActionEvent>[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddOffset(data[i].Value); return builder.EndVector(); }
  public static VectorOffset CreateActionEventsVectorBlock(FlatBufferBuilder builder, Offset<rewind_viewer.fbs.ActionEvent>[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateActionEventsVectorBlock(FlatBufferBuilder builder, ArraySegment<Offset<rewind_viewer.fbs.ActionEvent>> data) { builder.StartVector(4, data.Count, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateActionEventsVectorBlock(FlatBufferBuilder builder, IntPtr dataPtr, int sizeInBytes) { builder.StartVector(1, sizeInBytes, 1); builder.Add<Offset<rewind_viewer.fbs.ActionEvent>>(dataPtr, sizeInBytes); return builder.EndVector(); }
  public static void StartActionEventsVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static Offset<rewind_viewer.fbs.RewindEventList> EndRewindEventList(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<rewind_viewer.fbs.RewindEventList>(o);
  }
  public static void FinishRewindEventListBuffer(FlatBufferBuilder builder, Offset<rewind_viewer.fbs.RewindEventList> offset) { builder.Finish(offset.Value); }
  public static void FinishSizePrefixedRewindEventListBuffer(FlatBufferBuilder builder, Offset<rewind_viewer.fbs.RewindEventList> offset) { builder.FinishSizePrefixed(offset.Value); }
}


static public class RewindEventListVerify
{
  static public bool Verify(Google.FlatBuffers.Verifier verifier, uint tablePos)
  {
    return verifier.VerifyTableStart(tablePos)
      && verifier.VerifyVectorOfTables(tablePos, 4 /*KeyEvents*/, rewind_viewer.fbs.KeyEventVerify.Verify, false)
      && verifier.VerifyVectorOfTables(tablePos, 6 /*ActionEvents*/, rewind_viewer.fbs.ActionEventVerify.Verify, false)
      && verifier.VerifyTableEnd(tablePos);
  }
}

}
