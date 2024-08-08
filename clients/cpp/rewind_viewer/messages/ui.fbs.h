// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_UI_REWIND_VIEWER_FBS_H_
#define FLATBUFFERS_GENERATED_UI_REWIND_VIEWER_FBS_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 25,
             "Non-compatible flatbuffers version included");

#include "vector2f.fbs.h"

namespace rewind_viewer {
namespace fbs {

struct LogText;
struct LogTextBuilder;

struct Popup;
struct PopupBuilder;

struct PopupRound;
struct PopupRoundBuilder;

struct CameraView;
struct CameraViewBuilder;

struct Text;
struct TextBuilder;

struct LogText FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef LogTextBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TEXT = 4
  };
  const ::flatbuffers::String *text() const {
    return GetPointer<const ::flatbuffers::String *>(VT_TEXT);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffsetRequired(verifier, VT_TEXT) &&
           verifier.VerifyString(text()) &&
           verifier.EndTable();
  }
};

struct LogTextBuilder {
  typedef LogText Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_text(::flatbuffers::Offset<::flatbuffers::String> text) {
    fbb_.AddOffset(LogText::VT_TEXT, text);
  }
  explicit LogTextBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<LogText> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<LogText>(end);
    fbb_.Required(o, LogText::VT_TEXT);
    return o;
  }
};

inline ::flatbuffers::Offset<LogText> CreateLogText(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> text = 0) {
  LogTextBuilder builder_(_fbb);
  builder_.add_text(text);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<LogText> CreateLogTextDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *text = nullptr) {
  auto text__ = text ? _fbb.CreateString(text) : 0;
  return rewind_viewer::fbs::CreateLogText(
      _fbb,
      text__);
}

struct Popup FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef PopupBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TEXT = 4,
    VT_AREA_POSITION = 6,
    VT_AREA_SIZE = 8
  };
  const ::flatbuffers::String *text() const {
    return GetPointer<const ::flatbuffers::String *>(VT_TEXT);
  }
  const rewind_viewer::fbs::Vector2f *area_position() const {
    return GetStruct<const rewind_viewer::fbs::Vector2f *>(VT_AREA_POSITION);
  }
  const rewind_viewer::fbs::Vector2f *area_size() const {
    return GetStruct<const rewind_viewer::fbs::Vector2f *>(VT_AREA_SIZE);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffsetRequired(verifier, VT_TEXT) &&
           verifier.VerifyString(text()) &&
           VerifyFieldRequired<rewind_viewer::fbs::Vector2f>(verifier, VT_AREA_POSITION, 4) &&
           VerifyFieldRequired<rewind_viewer::fbs::Vector2f>(verifier, VT_AREA_SIZE, 4) &&
           verifier.EndTable();
  }
};

struct PopupBuilder {
  typedef Popup Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_text(::flatbuffers::Offset<::flatbuffers::String> text) {
    fbb_.AddOffset(Popup::VT_TEXT, text);
  }
  void add_area_position(const rewind_viewer::fbs::Vector2f *area_position) {
    fbb_.AddStruct(Popup::VT_AREA_POSITION, area_position);
  }
  void add_area_size(const rewind_viewer::fbs::Vector2f *area_size) {
    fbb_.AddStruct(Popup::VT_AREA_SIZE, area_size);
  }
  explicit PopupBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Popup> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Popup>(end);
    fbb_.Required(o, Popup::VT_TEXT);
    fbb_.Required(o, Popup::VT_AREA_POSITION);
    fbb_.Required(o, Popup::VT_AREA_SIZE);
    return o;
  }
};

inline ::flatbuffers::Offset<Popup> CreatePopup(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> text = 0,
    const rewind_viewer::fbs::Vector2f *area_position = nullptr,
    const rewind_viewer::fbs::Vector2f *area_size = nullptr) {
  PopupBuilder builder_(_fbb);
  builder_.add_area_size(area_size);
  builder_.add_area_position(area_position);
  builder_.add_text(text);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Popup> CreatePopupDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *text = nullptr,
    const rewind_viewer::fbs::Vector2f *area_position = nullptr,
    const rewind_viewer::fbs::Vector2f *area_size = nullptr) {
  auto text__ = text ? _fbb.CreateString(text) : 0;
  return rewind_viewer::fbs::CreatePopup(
      _fbb,
      text__,
      area_position,
      area_size);
}

struct PopupRound FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef PopupRoundBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TEXT = 4,
    VT_AREA_CENTER = 6,
    VT_AREA_RADIUS = 8
  };
  const ::flatbuffers::String *text() const {
    return GetPointer<const ::flatbuffers::String *>(VT_TEXT);
  }
  const rewind_viewer::fbs::Vector2f *area_center() const {
    return GetStruct<const rewind_viewer::fbs::Vector2f *>(VT_AREA_CENTER);
  }
  float area_radius() const {
    return GetField<float>(VT_AREA_RADIUS, 0.0f);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffsetRequired(verifier, VT_TEXT) &&
           verifier.VerifyString(text()) &&
           VerifyFieldRequired<rewind_viewer::fbs::Vector2f>(verifier, VT_AREA_CENTER, 4) &&
           VerifyField<float>(verifier, VT_AREA_RADIUS, 4) &&
           verifier.EndTable();
  }
};

struct PopupRoundBuilder {
  typedef PopupRound Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_text(::flatbuffers::Offset<::flatbuffers::String> text) {
    fbb_.AddOffset(PopupRound::VT_TEXT, text);
  }
  void add_area_center(const rewind_viewer::fbs::Vector2f *area_center) {
    fbb_.AddStruct(PopupRound::VT_AREA_CENTER, area_center);
  }
  void add_area_radius(float area_radius) {
    fbb_.AddElement<float>(PopupRound::VT_AREA_RADIUS, area_radius, 0.0f);
  }
  explicit PopupRoundBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<PopupRound> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<PopupRound>(end);
    fbb_.Required(o, PopupRound::VT_TEXT);
    fbb_.Required(o, PopupRound::VT_AREA_CENTER);
    return o;
  }
};

inline ::flatbuffers::Offset<PopupRound> CreatePopupRound(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> text = 0,
    const rewind_viewer::fbs::Vector2f *area_center = nullptr,
    float area_radius = 0.0f) {
  PopupRoundBuilder builder_(_fbb);
  builder_.add_area_radius(area_radius);
  builder_.add_area_center(area_center);
  builder_.add_text(text);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<PopupRound> CreatePopupRoundDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *text = nullptr,
    const rewind_viewer::fbs::Vector2f *area_center = nullptr,
    float area_radius = 0.0f) {
  auto text__ = text ? _fbb.CreateString(text) : 0;
  return rewind_viewer::fbs::CreatePopupRound(
      _fbb,
      text__,
      area_center,
      area_radius);
}

struct CameraView FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef CameraViewBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_POSITION = 6,
    VT_VIEW_RADIUS = 8
  };
  const ::flatbuffers::String *name() const {
    return GetPointer<const ::flatbuffers::String *>(VT_NAME);
  }
  const rewind_viewer::fbs::Vector2f *position() const {
    return GetStruct<const rewind_viewer::fbs::Vector2f *>(VT_POSITION);
  }
  float view_radius() const {
    return GetField<float>(VT_VIEW_RADIUS, 0.0f);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffsetRequired(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyFieldRequired<rewind_viewer::fbs::Vector2f>(verifier, VT_POSITION, 4) &&
           VerifyField<float>(verifier, VT_VIEW_RADIUS, 4) &&
           verifier.EndTable();
  }
};

struct CameraViewBuilder {
  typedef CameraView Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_name(::flatbuffers::Offset<::flatbuffers::String> name) {
    fbb_.AddOffset(CameraView::VT_NAME, name);
  }
  void add_position(const rewind_viewer::fbs::Vector2f *position) {
    fbb_.AddStruct(CameraView::VT_POSITION, position);
  }
  void add_view_radius(float view_radius) {
    fbb_.AddElement<float>(CameraView::VT_VIEW_RADIUS, view_radius, 0.0f);
  }
  explicit CameraViewBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<CameraView> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<CameraView>(end);
    fbb_.Required(o, CameraView::VT_NAME);
    fbb_.Required(o, CameraView::VT_POSITION);
    return o;
  }
};

inline ::flatbuffers::Offset<CameraView> CreateCameraView(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> name = 0,
    const rewind_viewer::fbs::Vector2f *position = nullptr,
    float view_radius = 0.0f) {
  CameraViewBuilder builder_(_fbb);
  builder_.add_view_radius(view_radius);
  builder_.add_position(position);
  builder_.add_name(name);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<CameraView> CreateCameraViewDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    const rewind_viewer::fbs::Vector2f *position = nullptr,
    float view_radius = 0.0f) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  return rewind_viewer::fbs::CreateCameraView(
      _fbb,
      name__,
      position,
      view_radius);
}

struct Text FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef TextBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TEXT = 4,
    VT_POSITION = 6,
    VT_SIZE = 8,
    VT_COLOR = 10
  };
  const ::flatbuffers::String *text() const {
    return GetPointer<const ::flatbuffers::String *>(VT_TEXT);
  }
  const rewind_viewer::fbs::Vector2f *position() const {
    return GetStruct<const rewind_viewer::fbs::Vector2f *>(VT_POSITION);
  }
  float size() const {
    return GetField<float>(VT_SIZE, 0.0f);
  }
  uint32_t color() const {
    return GetField<uint32_t>(VT_COLOR, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffsetRequired(verifier, VT_TEXT) &&
           verifier.VerifyString(text()) &&
           VerifyFieldRequired<rewind_viewer::fbs::Vector2f>(verifier, VT_POSITION, 4) &&
           VerifyField<float>(verifier, VT_SIZE, 4) &&
           VerifyField<uint32_t>(verifier, VT_COLOR, 4) &&
           verifier.EndTable();
  }
};

struct TextBuilder {
  typedef Text Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_text(::flatbuffers::Offset<::flatbuffers::String> text) {
    fbb_.AddOffset(Text::VT_TEXT, text);
  }
  void add_position(const rewind_viewer::fbs::Vector2f *position) {
    fbb_.AddStruct(Text::VT_POSITION, position);
  }
  void add_size(float size) {
    fbb_.AddElement<float>(Text::VT_SIZE, size, 0.0f);
  }
  void add_color(uint32_t color) {
    fbb_.AddElement<uint32_t>(Text::VT_COLOR, color, 0);
  }
  explicit TextBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Text> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Text>(end);
    fbb_.Required(o, Text::VT_TEXT);
    fbb_.Required(o, Text::VT_POSITION);
    return o;
  }
};

inline ::flatbuffers::Offset<Text> CreateText(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> text = 0,
    const rewind_viewer::fbs::Vector2f *position = nullptr,
    float size = 0.0f,
    uint32_t color = 0) {
  TextBuilder builder_(_fbb);
  builder_.add_color(color);
  builder_.add_size(size);
  builder_.add_position(position);
  builder_.add_text(text);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Text> CreateTextDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *text = nullptr,
    const rewind_viewer::fbs::Vector2f *position = nullptr,
    float size = 0.0f,
    uint32_t color = 0) {
  auto text__ = text ? _fbb.CreateString(text) : 0;
  return rewind_viewer::fbs::CreateText(
      _fbb,
      text__,
      position,
      size,
      color);
}

}  // namespace fbs
}  // namespace rewind_viewer

#endif  // FLATBUFFERS_GENERATED_UI_REWIND_VIEWER_FBS_H_
