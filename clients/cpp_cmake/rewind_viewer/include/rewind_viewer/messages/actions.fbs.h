// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_ACTIONS_REWIND_VIEWER_FBS_H_
#define FLATBUFFERS_GENERATED_ACTIONS_REWIND_VIEWER_FBS_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 25,
             "Non-compatible flatbuffers version included");

namespace rewind_viewer {
namespace fbs {

struct Subscribe;
struct SubscribeBuilder;

struct Unsubscribe;
struct UnsubscribeBuilder;

struct IntInput;
struct IntInputBuilder;

struct FloatInput;
struct FloatInputBuilder;

struct SelectInput;
struct SelectInputBuilder;

struct StringInput;
struct StringInputBuilder;

struct BoolInput;
struct BoolInputBuilder;

struct ButtonInput;
struct ButtonInputBuilder;

struct CreateAction;
struct CreateActionBuilder;

struct RemoveAction;
struct RemoveActionBuilder;

struct ReadEvents;
struct ReadEventsBuilder;

enum ActionInput : uint8_t {
  ActionInput_NONE = 0,
  ActionInput_BoolInput = 1,
  ActionInput_ButtonInput = 2,
  ActionInput_FloatInput = 3,
  ActionInput_IntInput = 4,
  ActionInput_SelectInput = 5,
  ActionInput_StringInput = 6,
  ActionInput_MIN = ActionInput_NONE,
  ActionInput_MAX = ActionInput_StringInput
};

inline const ActionInput (&EnumValuesActionInput())[7] {
  static const ActionInput values[] = {
    ActionInput_NONE,
    ActionInput_BoolInput,
    ActionInput_ButtonInput,
    ActionInput_FloatInput,
    ActionInput_IntInput,
    ActionInput_SelectInput,
    ActionInput_StringInput
  };
  return values;
}

inline const char * const *EnumNamesActionInput() {
  static const char * const names[8] = {
    "NONE",
    "BoolInput",
    "ButtonInput",
    "FloatInput",
    "IntInput",
    "SelectInput",
    "StringInput",
    nullptr
  };
  return names;
}

inline const char *EnumNameActionInput(ActionInput e) {
  if (::flatbuffers::IsOutRange(e, ActionInput_NONE, ActionInput_StringInput)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesActionInput()[index];
}

template<typename T> struct ActionInputTraits {
  static const ActionInput enum_value = ActionInput_NONE;
};

template<> struct ActionInputTraits<rewind_viewer::fbs::BoolInput> {
  static const ActionInput enum_value = ActionInput_BoolInput;
};

template<> struct ActionInputTraits<rewind_viewer::fbs::ButtonInput> {
  static const ActionInput enum_value = ActionInput_ButtonInput;
};

template<> struct ActionInputTraits<rewind_viewer::fbs::FloatInput> {
  static const ActionInput enum_value = ActionInput_FloatInput;
};

template<> struct ActionInputTraits<rewind_viewer::fbs::IntInput> {
  static const ActionInput enum_value = ActionInput_IntInput;
};

template<> struct ActionInputTraits<rewind_viewer::fbs::SelectInput> {
  static const ActionInput enum_value = ActionInput_SelectInput;
};

template<> struct ActionInputTraits<rewind_viewer::fbs::StringInput> {
  static const ActionInput enum_value = ActionInput_StringInput;
};

bool VerifyActionInput(::flatbuffers::Verifier &verifier, const void *obj, ActionInput type);
bool VerifyActionInputVector(::flatbuffers::Verifier &verifier, const ::flatbuffers::Vector<::flatbuffers::Offset<void>> *values, const ::flatbuffers::Vector<uint8_t> *types);

struct Subscribe FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef SubscribeBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_KEY = 4,
    VT_NAME = 6,
    VT_CONTINUOUS = 8,
    VT_CAPTURE_MOUSE = 10
  };
  int8_t key() const {
    return GetField<int8_t>(VT_KEY, 0);
  }
  const ::flatbuffers::String *name() const {
    return GetPointer<const ::flatbuffers::String *>(VT_NAME);
  }
  bool continuous() const {
    return GetField<uint8_t>(VT_CONTINUOUS, 0) != 0;
  }
  bool capture_mouse() const {
    return GetField<uint8_t>(VT_CAPTURE_MOUSE, 0) != 0;
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int8_t>(verifier, VT_KEY, 1) &&
           VerifyOffsetRequired(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyField<uint8_t>(verifier, VT_CONTINUOUS, 1) &&
           VerifyField<uint8_t>(verifier, VT_CAPTURE_MOUSE, 1) &&
           verifier.EndTable();
  }
};

struct SubscribeBuilder {
  typedef Subscribe Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_key(int8_t key) {
    fbb_.AddElement<int8_t>(Subscribe::VT_KEY, key, 0);
  }
  void add_name(::flatbuffers::Offset<::flatbuffers::String> name) {
    fbb_.AddOffset(Subscribe::VT_NAME, name);
  }
  void add_continuous(bool continuous) {
    fbb_.AddElement<uint8_t>(Subscribe::VT_CONTINUOUS, static_cast<uint8_t>(continuous), 0);
  }
  void add_capture_mouse(bool capture_mouse) {
    fbb_.AddElement<uint8_t>(Subscribe::VT_CAPTURE_MOUSE, static_cast<uint8_t>(capture_mouse), 0);
  }
  explicit SubscribeBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Subscribe> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Subscribe>(end);
    fbb_.Required(o, Subscribe::VT_NAME);
    return o;
  }
};

inline ::flatbuffers::Offset<Subscribe> CreateSubscribe(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    int8_t key = 0,
    ::flatbuffers::Offset<::flatbuffers::String> name = 0,
    bool continuous = false,
    bool capture_mouse = false) {
  SubscribeBuilder builder_(_fbb);
  builder_.add_name(name);
  builder_.add_capture_mouse(capture_mouse);
  builder_.add_continuous(continuous);
  builder_.add_key(key);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Subscribe> CreateSubscribeDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    int8_t key = 0,
    const char *name = nullptr,
    bool continuous = false,
    bool capture_mouse = false) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  return rewind_viewer::fbs::CreateSubscribe(
      _fbb,
      key,
      name__,
      continuous,
      capture_mouse);
}

struct Unsubscribe FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef UnsubscribeBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_KEY = 4
  };
  int8_t key() const {
    return GetField<int8_t>(VT_KEY, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int8_t>(verifier, VT_KEY, 1) &&
           verifier.EndTable();
  }
};

struct UnsubscribeBuilder {
  typedef Unsubscribe Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_key(int8_t key) {
    fbb_.AddElement<int8_t>(Unsubscribe::VT_KEY, key, 0);
  }
  explicit UnsubscribeBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Unsubscribe> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Unsubscribe>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Unsubscribe> CreateUnsubscribe(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    int8_t key = 0) {
  UnsubscribeBuilder builder_(_fbb);
  builder_.add_key(key);
  return builder_.Finish();
}

struct IntInput FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef IntInputBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_DEFAULT_VALUE = 4,
    VT_MIN_VALUE = 6,
    VT_MAX_VALUE = 8
  };
  int32_t default_value() const {
    return GetField<int32_t>(VT_DEFAULT_VALUE, 0);
  }
  int32_t min_value() const {
    return GetField<int32_t>(VT_MIN_VALUE, 0);
  }
  int32_t max_value() const {
    return GetField<int32_t>(VT_MAX_VALUE, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_DEFAULT_VALUE, 4) &&
           VerifyField<int32_t>(verifier, VT_MIN_VALUE, 4) &&
           VerifyField<int32_t>(verifier, VT_MAX_VALUE, 4) &&
           verifier.EndTable();
  }
};

struct IntInputBuilder {
  typedef IntInput Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_default_value(int32_t default_value) {
    fbb_.AddElement<int32_t>(IntInput::VT_DEFAULT_VALUE, default_value, 0);
  }
  void add_min_value(int32_t min_value) {
    fbb_.AddElement<int32_t>(IntInput::VT_MIN_VALUE, min_value, 0);
  }
  void add_max_value(int32_t max_value) {
    fbb_.AddElement<int32_t>(IntInput::VT_MAX_VALUE, max_value, 0);
  }
  explicit IntInputBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<IntInput> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<IntInput>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<IntInput> CreateIntInput(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    int32_t default_value = 0,
    int32_t min_value = 0,
    int32_t max_value = 0) {
  IntInputBuilder builder_(_fbb);
  builder_.add_max_value(max_value);
  builder_.add_min_value(min_value);
  builder_.add_default_value(default_value);
  return builder_.Finish();
}

struct FloatInput FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef FloatInputBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_DEFAULT_VALUE = 4,
    VT_MIN_VALUE = 6,
    VT_MAX_VALUE = 8
  };
  float default_value() const {
    return GetField<float>(VT_DEFAULT_VALUE, 0.0f);
  }
  float min_value() const {
    return GetField<float>(VT_MIN_VALUE, 0.0f);
  }
  float max_value() const {
    return GetField<float>(VT_MAX_VALUE, 0.0f);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<float>(verifier, VT_DEFAULT_VALUE, 4) &&
           VerifyField<float>(verifier, VT_MIN_VALUE, 4) &&
           VerifyField<float>(verifier, VT_MAX_VALUE, 4) &&
           verifier.EndTable();
  }
};

struct FloatInputBuilder {
  typedef FloatInput Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_default_value(float default_value) {
    fbb_.AddElement<float>(FloatInput::VT_DEFAULT_VALUE, default_value, 0.0f);
  }
  void add_min_value(float min_value) {
    fbb_.AddElement<float>(FloatInput::VT_MIN_VALUE, min_value, 0.0f);
  }
  void add_max_value(float max_value) {
    fbb_.AddElement<float>(FloatInput::VT_MAX_VALUE, max_value, 0.0f);
  }
  explicit FloatInputBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<FloatInput> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<FloatInput>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<FloatInput> CreateFloatInput(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    float default_value = 0.0f,
    float min_value = 0.0f,
    float max_value = 0.0f) {
  FloatInputBuilder builder_(_fbb);
  builder_.add_max_value(max_value);
  builder_.add_min_value(min_value);
  builder_.add_default_value(default_value);
  return builder_.Finish();
}

struct SelectInput FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef SelectInputBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_OPTIONS = 4,
    VT_SELECTED_OPTION = 6
  };
  const ::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>> *options() const {
    return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>> *>(VT_OPTIONS);
  }
  uint16_t selected_option() const {
    return GetField<uint16_t>(VT_SELECTED_OPTION, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffsetRequired(verifier, VT_OPTIONS) &&
           verifier.VerifyVector(options()) &&
           verifier.VerifyVectorOfStrings(options()) &&
           VerifyField<uint16_t>(verifier, VT_SELECTED_OPTION, 2) &&
           verifier.EndTable();
  }
};

struct SelectInputBuilder {
  typedef SelectInput Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_options(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>>> options) {
    fbb_.AddOffset(SelectInput::VT_OPTIONS, options);
  }
  void add_selected_option(uint16_t selected_option) {
    fbb_.AddElement<uint16_t>(SelectInput::VT_SELECTED_OPTION, selected_option, 0);
  }
  explicit SelectInputBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<SelectInput> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<SelectInput>(end);
    fbb_.Required(o, SelectInput::VT_OPTIONS);
    return o;
  }
};

inline ::flatbuffers::Offset<SelectInput> CreateSelectInput(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>>> options = 0,
    uint16_t selected_option = 0) {
  SelectInputBuilder builder_(_fbb);
  builder_.add_options(options);
  builder_.add_selected_option(selected_option);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<SelectInput> CreateSelectInputDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<::flatbuffers::Offset<::flatbuffers::String>> *options = nullptr,
    uint16_t selected_option = 0) {
  auto options__ = options ? _fbb.CreateVector<::flatbuffers::Offset<::flatbuffers::String>>(*options) : 0;
  return rewind_viewer::fbs::CreateSelectInput(
      _fbb,
      options__,
      selected_option);
}

struct StringInput FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef StringInputBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_DEFAULT_VALUE = 4
  };
  const ::flatbuffers::String *default_value() const {
    return GetPointer<const ::flatbuffers::String *>(VT_DEFAULT_VALUE);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_DEFAULT_VALUE) &&
           verifier.VerifyString(default_value()) &&
           verifier.EndTable();
  }
};

struct StringInputBuilder {
  typedef StringInput Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_default_value(::flatbuffers::Offset<::flatbuffers::String> default_value) {
    fbb_.AddOffset(StringInput::VT_DEFAULT_VALUE, default_value);
  }
  explicit StringInputBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<StringInput> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<StringInput>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<StringInput> CreateStringInput(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> default_value = 0) {
  StringInputBuilder builder_(_fbb);
  builder_.add_default_value(default_value);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<StringInput> CreateStringInputDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *default_value = nullptr) {
  auto default_value__ = default_value ? _fbb.CreateString(default_value) : 0;
  return rewind_viewer::fbs::CreateStringInput(
      _fbb,
      default_value__);
}

struct BoolInput FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef BoolInputBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_DEFAULT_VALUE = 4
  };
  bool default_value() const {
    return GetField<uint8_t>(VT_DEFAULT_VALUE, 0) != 0;
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_DEFAULT_VALUE, 1) &&
           verifier.EndTable();
  }
};

struct BoolInputBuilder {
  typedef BoolInput Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_default_value(bool default_value) {
    fbb_.AddElement<uint8_t>(BoolInput::VT_DEFAULT_VALUE, static_cast<uint8_t>(default_value), 0);
  }
  explicit BoolInputBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<BoolInput> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<BoolInput>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<BoolInput> CreateBoolInput(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    bool default_value = false) {
  BoolInputBuilder builder_(_fbb);
  builder_.add_default_value(default_value);
  return builder_.Finish();
}

struct ButtonInput FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef ButtonInputBuilder Builder;
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           verifier.EndTable();
  }
};

struct ButtonInputBuilder {
  typedef ButtonInput Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  explicit ButtonInputBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<ButtonInput> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<ButtonInput>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<ButtonInput> CreateButtonInput(
    ::flatbuffers::FlatBufferBuilder &_fbb) {
  ButtonInputBuilder builder_(_fbb);
  return builder_.Finish();
}

struct CreateAction FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef CreateActionBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_INPUT_TYPE = 6,
    VT_INPUT = 8
  };
  const ::flatbuffers::String *name() const {
    return GetPointer<const ::flatbuffers::String *>(VT_NAME);
  }
  rewind_viewer::fbs::ActionInput input_type() const {
    return static_cast<rewind_viewer::fbs::ActionInput>(GetField<uint8_t>(VT_INPUT_TYPE, 0));
  }
  const void *input() const {
    return GetPointer<const void *>(VT_INPUT);
  }
  template<typename T> const T *input_as() const;
  const rewind_viewer::fbs::BoolInput *input_as_BoolInput() const {
    return input_type() == rewind_viewer::fbs::ActionInput_BoolInput ? static_cast<const rewind_viewer::fbs::BoolInput *>(input()) : nullptr;
  }
  const rewind_viewer::fbs::ButtonInput *input_as_ButtonInput() const {
    return input_type() == rewind_viewer::fbs::ActionInput_ButtonInput ? static_cast<const rewind_viewer::fbs::ButtonInput *>(input()) : nullptr;
  }
  const rewind_viewer::fbs::FloatInput *input_as_FloatInput() const {
    return input_type() == rewind_viewer::fbs::ActionInput_FloatInput ? static_cast<const rewind_viewer::fbs::FloatInput *>(input()) : nullptr;
  }
  const rewind_viewer::fbs::IntInput *input_as_IntInput() const {
    return input_type() == rewind_viewer::fbs::ActionInput_IntInput ? static_cast<const rewind_viewer::fbs::IntInput *>(input()) : nullptr;
  }
  const rewind_viewer::fbs::SelectInput *input_as_SelectInput() const {
    return input_type() == rewind_viewer::fbs::ActionInput_SelectInput ? static_cast<const rewind_viewer::fbs::SelectInput *>(input()) : nullptr;
  }
  const rewind_viewer::fbs::StringInput *input_as_StringInput() const {
    return input_type() == rewind_viewer::fbs::ActionInput_StringInput ? static_cast<const rewind_viewer::fbs::StringInput *>(input()) : nullptr;
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffsetRequired(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyField<uint8_t>(verifier, VT_INPUT_TYPE, 1) &&
           VerifyOffset(verifier, VT_INPUT) &&
           VerifyActionInput(verifier, input(), input_type()) &&
           verifier.EndTable();
  }
};

template<> inline const rewind_viewer::fbs::BoolInput *CreateAction::input_as<rewind_viewer::fbs::BoolInput>() const {
  return input_as_BoolInput();
}

template<> inline const rewind_viewer::fbs::ButtonInput *CreateAction::input_as<rewind_viewer::fbs::ButtonInput>() const {
  return input_as_ButtonInput();
}

template<> inline const rewind_viewer::fbs::FloatInput *CreateAction::input_as<rewind_viewer::fbs::FloatInput>() const {
  return input_as_FloatInput();
}

template<> inline const rewind_viewer::fbs::IntInput *CreateAction::input_as<rewind_viewer::fbs::IntInput>() const {
  return input_as_IntInput();
}

template<> inline const rewind_viewer::fbs::SelectInput *CreateAction::input_as<rewind_viewer::fbs::SelectInput>() const {
  return input_as_SelectInput();
}

template<> inline const rewind_viewer::fbs::StringInput *CreateAction::input_as<rewind_viewer::fbs::StringInput>() const {
  return input_as_StringInput();
}

struct CreateActionBuilder {
  typedef CreateAction Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_name(::flatbuffers::Offset<::flatbuffers::String> name) {
    fbb_.AddOffset(CreateAction::VT_NAME, name);
  }
  void add_input_type(rewind_viewer::fbs::ActionInput input_type) {
    fbb_.AddElement<uint8_t>(CreateAction::VT_INPUT_TYPE, static_cast<uint8_t>(input_type), 0);
  }
  void add_input(::flatbuffers::Offset<void> input) {
    fbb_.AddOffset(CreateAction::VT_INPUT, input);
  }
  explicit CreateActionBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<CreateAction> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<CreateAction>(end);
    fbb_.Required(o, CreateAction::VT_NAME);
    return o;
  }
};

inline ::flatbuffers::Offset<CreateAction> CreateCreateAction(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> name = 0,
    rewind_viewer::fbs::ActionInput input_type = rewind_viewer::fbs::ActionInput_NONE,
    ::flatbuffers::Offset<void> input = 0) {
  CreateActionBuilder builder_(_fbb);
  builder_.add_input(input);
  builder_.add_name(name);
  builder_.add_input_type(input_type);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<CreateAction> CreateCreateActionDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    rewind_viewer::fbs::ActionInput input_type = rewind_viewer::fbs::ActionInput_NONE,
    ::flatbuffers::Offset<void> input = 0) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  return rewind_viewer::fbs::CreateCreateAction(
      _fbb,
      name__,
      input_type,
      input);
}

struct RemoveAction FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef RemoveActionBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4
  };
  const ::flatbuffers::String *name() const {
    return GetPointer<const ::flatbuffers::String *>(VT_NAME);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffsetRequired(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           verifier.EndTable();
  }
};

struct RemoveActionBuilder {
  typedef RemoveAction Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_name(::flatbuffers::Offset<::flatbuffers::String> name) {
    fbb_.AddOffset(RemoveAction::VT_NAME, name);
  }
  explicit RemoveActionBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<RemoveAction> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<RemoveAction>(end);
    fbb_.Required(o, RemoveAction::VT_NAME);
    return o;
  }
};

inline ::flatbuffers::Offset<RemoveAction> CreateRemoveAction(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> name = 0) {
  RemoveActionBuilder builder_(_fbb);
  builder_.add_name(name);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<RemoveAction> CreateRemoveActionDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  return rewind_viewer::fbs::CreateRemoveAction(
      _fbb,
      name__);
}

struct ReadEvents FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef ReadEventsBuilder Builder;
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           verifier.EndTable();
  }
};

struct ReadEventsBuilder {
  typedef ReadEvents Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  explicit ReadEventsBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<ReadEvents> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<ReadEvents>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<ReadEvents> CreateReadEvents(
    ::flatbuffers::FlatBufferBuilder &_fbb) {
  ReadEventsBuilder builder_(_fbb);
  return builder_.Finish();
}

inline bool VerifyActionInput(::flatbuffers::Verifier &verifier, const void *obj, ActionInput type) {
  switch (type) {
    case ActionInput_NONE: {
      return true;
    }
    case ActionInput_BoolInput: {
      auto ptr = reinterpret_cast<const rewind_viewer::fbs::BoolInput *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case ActionInput_ButtonInput: {
      auto ptr = reinterpret_cast<const rewind_viewer::fbs::ButtonInput *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case ActionInput_FloatInput: {
      auto ptr = reinterpret_cast<const rewind_viewer::fbs::FloatInput *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case ActionInput_IntInput: {
      auto ptr = reinterpret_cast<const rewind_viewer::fbs::IntInput *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case ActionInput_SelectInput: {
      auto ptr = reinterpret_cast<const rewind_viewer::fbs::SelectInput *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case ActionInput_StringInput: {
      auto ptr = reinterpret_cast<const rewind_viewer::fbs::StringInput *>(obj);
      return verifier.VerifyTable(ptr);
    }
    default: return true;
  }
}

inline bool VerifyActionInputVector(::flatbuffers::Verifier &verifier, const ::flatbuffers::Vector<::flatbuffers::Offset<void>> *values, const ::flatbuffers::Vector<uint8_t> *types) {
  if (!values || !types) return !values && !types;
  if (values->size() != types->size()) return false;
  for (::flatbuffers::uoffset_t i = 0; i < values->size(); ++i) {
    if (!VerifyActionInput(
        verifier,  values->Get(i), types->GetEnum<ActionInput>(i))) {
      return false;
    }
  }
  return true;
}

}  // namespace fbs
}  // namespace rewind_viewer

#endif  // FLATBUFFERS_GENERATED_ACTIONS_REWIND_VIEWER_FBS_H_
