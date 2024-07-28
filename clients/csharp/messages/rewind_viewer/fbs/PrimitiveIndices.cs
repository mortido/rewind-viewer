// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace rewind_viewer.fbs
{

using global::System;
using global::System.Collections.Generic;
using global::Google.FlatBuffers;

public struct PrimitiveIndices : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_24_3_25(); }
  public static PrimitiveIndices GetRootAsPrimitiveIndices(ByteBuffer _bb) { return GetRootAsPrimitiveIndices(_bb, new PrimitiveIndices()); }
  public static PrimitiveIndices GetRootAsPrimitiveIndices(ByteBuffer _bb, PrimitiveIndices obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public PrimitiveIndices __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public uint StencilCircles(int j) { int o = __p.__offset(4); return o != 0 ? __p.bb.GetUint(__p.__vector(o) + j * 4) : (uint)0; }
  public int StencilCirclesLength { get { int o = __p.__offset(4); return o != 0 ? __p.__vector_len(o) : 0; } }
#if ENABLE_SPAN_T
  public Span<uint> GetStencilCirclesBytes() { return __p.__vector_as_span<uint>(4, 4); }
#else
  public ArraySegment<byte>? GetStencilCirclesBytes() { return __p.__vector_as_arraysegment(4); }
#endif
  public uint[] GetStencilCirclesArray() { return __p.__vector_as_array<uint>(4); }
  public uint StencilSegments(int j) { int o = __p.__offset(6); return o != 0 ? __p.bb.GetUint(__p.__vector(o) + j * 4) : (uint)0; }
  public int StencilSegmentsLength { get { int o = __p.__offset(6); return o != 0 ? __p.__vector_len(o) : 0; } }
#if ENABLE_SPAN_T
  public Span<uint> GetStencilSegmentsBytes() { return __p.__vector_as_span<uint>(6, 4); }
#else
  public ArraySegment<byte>? GetStencilSegmentsBytes() { return __p.__vector_as_arraysegment(6); }
#endif
  public uint[] GetStencilSegmentsArray() { return __p.__vector_as_array<uint>(6); }
  public uint StencilTriangles(int j) { int o = __p.__offset(8); return o != 0 ? __p.bb.GetUint(__p.__vector(o) + j * 4) : (uint)0; }
  public int StencilTrianglesLength { get { int o = __p.__offset(8); return o != 0 ? __p.__vector_len(o) : 0; } }
#if ENABLE_SPAN_T
  public Span<uint> GetStencilTrianglesBytes() { return __p.__vector_as_span<uint>(8, 4); }
#else
  public ArraySegment<byte>? GetStencilTrianglesBytes() { return __p.__vector_as_arraysegment(8); }
#endif
  public uint[] GetStencilTrianglesArray() { return __p.__vector_as_array<uint>(8); }
  public uint FilledCircles(int j) { int o = __p.__offset(10); return o != 0 ? __p.bb.GetUint(__p.__vector(o) + j * 4) : (uint)0; }
  public int FilledCirclesLength { get { int o = __p.__offset(10); return o != 0 ? __p.__vector_len(o) : 0; } }
#if ENABLE_SPAN_T
  public Span<uint> GetFilledCirclesBytes() { return __p.__vector_as_span<uint>(10, 4); }
#else
  public ArraySegment<byte>? GetFilledCirclesBytes() { return __p.__vector_as_arraysegment(10); }
#endif
  public uint[] GetFilledCirclesArray() { return __p.__vector_as_array<uint>(10); }
  public uint FilledSegments(int j) { int o = __p.__offset(12); return o != 0 ? __p.bb.GetUint(__p.__vector(o) + j * 4) : (uint)0; }
  public int FilledSegmentsLength { get { int o = __p.__offset(12); return o != 0 ? __p.__vector_len(o) : 0; } }
#if ENABLE_SPAN_T
  public Span<uint> GetFilledSegmentsBytes() { return __p.__vector_as_span<uint>(12, 4); }
#else
  public ArraySegment<byte>? GetFilledSegmentsBytes() { return __p.__vector_as_arraysegment(12); }
#endif
  public uint[] GetFilledSegmentsArray() { return __p.__vector_as_array<uint>(12); }
  public uint ThinCircles(int j) { int o = __p.__offset(14); return o != 0 ? __p.bb.GetUint(__p.__vector(o) + j * 4) : (uint)0; }
  public int ThinCirclesLength { get { int o = __p.__offset(14); return o != 0 ? __p.__vector_len(o) : 0; } }
#if ENABLE_SPAN_T
  public Span<uint> GetThinCirclesBytes() { return __p.__vector_as_span<uint>(14, 4); }
#else
  public ArraySegment<byte>? GetThinCirclesBytes() { return __p.__vector_as_arraysegment(14); }
#endif
  public uint[] GetThinCirclesArray() { return __p.__vector_as_array<uint>(14); }
  public uint Triangles(int j) { int o = __p.__offset(16); return o != 0 ? __p.bb.GetUint(__p.__vector(o) + j * 4) : (uint)0; }
  public int TrianglesLength { get { int o = __p.__offset(16); return o != 0 ? __p.__vector_len(o) : 0; } }
#if ENABLE_SPAN_T
  public Span<uint> GetTrianglesBytes() { return __p.__vector_as_span<uint>(16, 4); }
#else
  public ArraySegment<byte>? GetTrianglesBytes() { return __p.__vector_as_arraysegment(16); }
#endif
  public uint[] GetTrianglesArray() { return __p.__vector_as_array<uint>(16); }
  public uint Lines(int j) { int o = __p.__offset(18); return o != 0 ? __p.bb.GetUint(__p.__vector(o) + j * 4) : (uint)0; }
  public int LinesLength { get { int o = __p.__offset(18); return o != 0 ? __p.__vector_len(o) : 0; } }
#if ENABLE_SPAN_T
  public Span<uint> GetLinesBytes() { return __p.__vector_as_span<uint>(18, 4); }
#else
  public ArraySegment<byte>? GetLinesBytes() { return __p.__vector_as_arraysegment(18); }
#endif
  public uint[] GetLinesArray() { return __p.__vector_as_array<uint>(18); }

  public static Offset<rewind_viewer.fbs.PrimitiveIndices> CreatePrimitiveIndices(FlatBufferBuilder builder,
      VectorOffset stencil_circlesOffset = default(VectorOffset),
      VectorOffset stencil_segmentsOffset = default(VectorOffset),
      VectorOffset stencil_trianglesOffset = default(VectorOffset),
      VectorOffset filled_circlesOffset = default(VectorOffset),
      VectorOffset filled_segmentsOffset = default(VectorOffset),
      VectorOffset thin_circlesOffset = default(VectorOffset),
      VectorOffset trianglesOffset = default(VectorOffset),
      VectorOffset linesOffset = default(VectorOffset)) {
    builder.StartTable(8);
    PrimitiveIndices.AddLines(builder, linesOffset);
    PrimitiveIndices.AddTriangles(builder, trianglesOffset);
    PrimitiveIndices.AddThinCircles(builder, thin_circlesOffset);
    PrimitiveIndices.AddFilledSegments(builder, filled_segmentsOffset);
    PrimitiveIndices.AddFilledCircles(builder, filled_circlesOffset);
    PrimitiveIndices.AddStencilTriangles(builder, stencil_trianglesOffset);
    PrimitiveIndices.AddStencilSegments(builder, stencil_segmentsOffset);
    PrimitiveIndices.AddStencilCircles(builder, stencil_circlesOffset);
    return PrimitiveIndices.EndPrimitiveIndices(builder);
  }

  public static void StartPrimitiveIndices(FlatBufferBuilder builder) { builder.StartTable(8); }
  public static void AddStencilCircles(FlatBufferBuilder builder, VectorOffset stencilCirclesOffset) { builder.AddOffset(0, stencilCirclesOffset.Value, 0); }
  public static VectorOffset CreateStencilCirclesVector(FlatBufferBuilder builder, uint[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddUint(data[i]); return builder.EndVector(); }
  public static VectorOffset CreateStencilCirclesVectorBlock(FlatBufferBuilder builder, uint[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateStencilCirclesVectorBlock(FlatBufferBuilder builder, ArraySegment<uint> data) { builder.StartVector(4, data.Count, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateStencilCirclesVectorBlock(FlatBufferBuilder builder, IntPtr dataPtr, int sizeInBytes) { builder.StartVector(1, sizeInBytes, 1); builder.Add<uint>(dataPtr, sizeInBytes); return builder.EndVector(); }
  public static void StartStencilCirclesVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static void AddStencilSegments(FlatBufferBuilder builder, VectorOffset stencilSegmentsOffset) { builder.AddOffset(1, stencilSegmentsOffset.Value, 0); }
  public static VectorOffset CreateStencilSegmentsVector(FlatBufferBuilder builder, uint[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddUint(data[i]); return builder.EndVector(); }
  public static VectorOffset CreateStencilSegmentsVectorBlock(FlatBufferBuilder builder, uint[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateStencilSegmentsVectorBlock(FlatBufferBuilder builder, ArraySegment<uint> data) { builder.StartVector(4, data.Count, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateStencilSegmentsVectorBlock(FlatBufferBuilder builder, IntPtr dataPtr, int sizeInBytes) { builder.StartVector(1, sizeInBytes, 1); builder.Add<uint>(dataPtr, sizeInBytes); return builder.EndVector(); }
  public static void StartStencilSegmentsVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static void AddStencilTriangles(FlatBufferBuilder builder, VectorOffset stencilTrianglesOffset) { builder.AddOffset(2, stencilTrianglesOffset.Value, 0); }
  public static VectorOffset CreateStencilTrianglesVector(FlatBufferBuilder builder, uint[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddUint(data[i]); return builder.EndVector(); }
  public static VectorOffset CreateStencilTrianglesVectorBlock(FlatBufferBuilder builder, uint[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateStencilTrianglesVectorBlock(FlatBufferBuilder builder, ArraySegment<uint> data) { builder.StartVector(4, data.Count, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateStencilTrianglesVectorBlock(FlatBufferBuilder builder, IntPtr dataPtr, int sizeInBytes) { builder.StartVector(1, sizeInBytes, 1); builder.Add<uint>(dataPtr, sizeInBytes); return builder.EndVector(); }
  public static void StartStencilTrianglesVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static void AddFilledCircles(FlatBufferBuilder builder, VectorOffset filledCirclesOffset) { builder.AddOffset(3, filledCirclesOffset.Value, 0); }
  public static VectorOffset CreateFilledCirclesVector(FlatBufferBuilder builder, uint[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddUint(data[i]); return builder.EndVector(); }
  public static VectorOffset CreateFilledCirclesVectorBlock(FlatBufferBuilder builder, uint[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateFilledCirclesVectorBlock(FlatBufferBuilder builder, ArraySegment<uint> data) { builder.StartVector(4, data.Count, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateFilledCirclesVectorBlock(FlatBufferBuilder builder, IntPtr dataPtr, int sizeInBytes) { builder.StartVector(1, sizeInBytes, 1); builder.Add<uint>(dataPtr, sizeInBytes); return builder.EndVector(); }
  public static void StartFilledCirclesVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static void AddFilledSegments(FlatBufferBuilder builder, VectorOffset filledSegmentsOffset) { builder.AddOffset(4, filledSegmentsOffset.Value, 0); }
  public static VectorOffset CreateFilledSegmentsVector(FlatBufferBuilder builder, uint[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddUint(data[i]); return builder.EndVector(); }
  public static VectorOffset CreateFilledSegmentsVectorBlock(FlatBufferBuilder builder, uint[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateFilledSegmentsVectorBlock(FlatBufferBuilder builder, ArraySegment<uint> data) { builder.StartVector(4, data.Count, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateFilledSegmentsVectorBlock(FlatBufferBuilder builder, IntPtr dataPtr, int sizeInBytes) { builder.StartVector(1, sizeInBytes, 1); builder.Add<uint>(dataPtr, sizeInBytes); return builder.EndVector(); }
  public static void StartFilledSegmentsVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static void AddThinCircles(FlatBufferBuilder builder, VectorOffset thinCirclesOffset) { builder.AddOffset(5, thinCirclesOffset.Value, 0); }
  public static VectorOffset CreateThinCirclesVector(FlatBufferBuilder builder, uint[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddUint(data[i]); return builder.EndVector(); }
  public static VectorOffset CreateThinCirclesVectorBlock(FlatBufferBuilder builder, uint[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateThinCirclesVectorBlock(FlatBufferBuilder builder, ArraySegment<uint> data) { builder.StartVector(4, data.Count, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateThinCirclesVectorBlock(FlatBufferBuilder builder, IntPtr dataPtr, int sizeInBytes) { builder.StartVector(1, sizeInBytes, 1); builder.Add<uint>(dataPtr, sizeInBytes); return builder.EndVector(); }
  public static void StartThinCirclesVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static void AddTriangles(FlatBufferBuilder builder, VectorOffset trianglesOffset) { builder.AddOffset(6, trianglesOffset.Value, 0); }
  public static VectorOffset CreateTrianglesVector(FlatBufferBuilder builder, uint[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddUint(data[i]); return builder.EndVector(); }
  public static VectorOffset CreateTrianglesVectorBlock(FlatBufferBuilder builder, uint[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateTrianglesVectorBlock(FlatBufferBuilder builder, ArraySegment<uint> data) { builder.StartVector(4, data.Count, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateTrianglesVectorBlock(FlatBufferBuilder builder, IntPtr dataPtr, int sizeInBytes) { builder.StartVector(1, sizeInBytes, 1); builder.Add<uint>(dataPtr, sizeInBytes); return builder.EndVector(); }
  public static void StartTrianglesVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static void AddLines(FlatBufferBuilder builder, VectorOffset linesOffset) { builder.AddOffset(7, linesOffset.Value, 0); }
  public static VectorOffset CreateLinesVector(FlatBufferBuilder builder, uint[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddUint(data[i]); return builder.EndVector(); }
  public static VectorOffset CreateLinesVectorBlock(FlatBufferBuilder builder, uint[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateLinesVectorBlock(FlatBufferBuilder builder, ArraySegment<uint> data) { builder.StartVector(4, data.Count, 4); builder.Add(data); return builder.EndVector(); }
  public static VectorOffset CreateLinesVectorBlock(FlatBufferBuilder builder, IntPtr dataPtr, int sizeInBytes) { builder.StartVector(1, sizeInBytes, 1); builder.Add<uint>(dataPtr, sizeInBytes); return builder.EndVector(); }
  public static void StartLinesVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static Offset<rewind_viewer.fbs.PrimitiveIndices> EndPrimitiveIndices(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<rewind_viewer.fbs.PrimitiveIndices>(o);
  }
}


static public class PrimitiveIndicesVerify
{
  static public bool Verify(Google.FlatBuffers.Verifier verifier, uint tablePos)
  {
    return verifier.VerifyTableStart(tablePos)
      && verifier.VerifyVectorOfData(tablePos, 4 /*StencilCircles*/, 4 /*uint*/, false)
      && verifier.VerifyVectorOfData(tablePos, 6 /*StencilSegments*/, 4 /*uint*/, false)
      && verifier.VerifyVectorOfData(tablePos, 8 /*StencilTriangles*/, 4 /*uint*/, false)
      && verifier.VerifyVectorOfData(tablePos, 10 /*FilledCircles*/, 4 /*uint*/, false)
      && verifier.VerifyVectorOfData(tablePos, 12 /*FilledSegments*/, 4 /*uint*/, false)
      && verifier.VerifyVectorOfData(tablePos, 14 /*ThinCircles*/, 4 /*uint*/, false)
      && verifier.VerifyVectorOfData(tablePos, 16 /*Triangles*/, 4 /*uint*/, false)
      && verifier.VerifyVectorOfData(tablePos, 18 /*Lines*/, 4 /*uint*/, false)
      && verifier.VerifyTableEnd(tablePos);
  }
}

}
