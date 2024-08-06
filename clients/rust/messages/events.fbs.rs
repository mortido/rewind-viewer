// automatically generated by the FlatBuffers compiler, do not modify


// @generated

use crate::vector2f.fbs::*;
use core::mem;
use core::cmp::Ordering;

extern crate flatbuffers;
use self::flatbuffers::{EndianScalar, Follow};

#[allow(unused_imports, dead_code)]
pub mod rewind_viewer {

  use crate::vector2f.fbs::*;
  use core::mem;
  use core::cmp::Ordering;

  extern crate flatbuffers;
  use self::flatbuffers::{EndianScalar, Follow};
#[allow(unused_imports, dead_code)]
pub mod fbs {

  use crate::vector2f.fbs::*;
  use core::mem;
  use core::cmp::Ordering;

  extern crate flatbuffers;
  use self::flatbuffers::{EndianScalar, Follow};

#[deprecated(since = "2.0.0", note = "Use associated constants instead. This will no longer be generated in 2021.")]
pub const ENUM_MIN_ACTION_VALUE: u8 = 0;
#[deprecated(since = "2.0.0", note = "Use associated constants instead. This will no longer be generated in 2021.")]
pub const ENUM_MAX_ACTION_VALUE: u8 = 4;
#[deprecated(since = "2.0.0", note = "Use associated constants instead. This will no longer be generated in 2021.")]
#[allow(non_camel_case_types)]
pub const ENUM_VALUES_ACTION_VALUE: [ActionValue; 5] = [
  ActionValue::NONE,
  ActionValue::IntValue,
  ActionValue::FloatValue,
  ActionValue::BoolValue,
  ActionValue::StringValue,
];

#[derive(Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash, Default)]
#[repr(transparent)]
pub struct ActionValue(pub u8);
#[allow(non_upper_case_globals)]
impl ActionValue {
  pub const NONE: Self = Self(0);
  pub const IntValue: Self = Self(1);
  pub const FloatValue: Self = Self(2);
  pub const BoolValue: Self = Self(3);
  pub const StringValue: Self = Self(4);

  pub const ENUM_MIN: u8 = 0;
  pub const ENUM_MAX: u8 = 4;
  pub const ENUM_VALUES: &'static [Self] = &[
    Self::NONE,
    Self::IntValue,
    Self::FloatValue,
    Self::BoolValue,
    Self::StringValue,
  ];
  /// Returns the variant's name or "" if unknown.
  pub fn variant_name(self) -> Option<&'static str> {
    match self {
      Self::NONE => Some("NONE"),
      Self::IntValue => Some("IntValue"),
      Self::FloatValue => Some("FloatValue"),
      Self::BoolValue => Some("BoolValue"),
      Self::StringValue => Some("StringValue"),
      _ => None,
    }
  }
}
impl core::fmt::Debug for ActionValue {
  fn fmt(&self, f: &mut core::fmt::Formatter) -> core::fmt::Result {
    if let Some(name) = self.variant_name() {
      f.write_str(name)
    } else {
      f.write_fmt(format_args!("<UNKNOWN {:?}>", self.0))
    }
  }
}
impl<'a> flatbuffers::Follow<'a> for ActionValue {
  type Inner = Self;
  #[inline]
  unsafe fn follow(buf: &'a [u8], loc: usize) -> Self::Inner {
    let b = flatbuffers::read_scalar_at::<u8>(buf, loc);
    Self(b)
  }
}

impl flatbuffers::Push for ActionValue {
    type Output = ActionValue;
    #[inline]
    unsafe fn push(&self, dst: &mut [u8], _written_len: usize) {
        flatbuffers::emplace_scalar::<u8>(dst, self.0);
    }
}

impl flatbuffers::EndianScalar for ActionValue {
  type Scalar = u8;
  #[inline]
  fn to_little_endian(self) -> u8 {
    self.0.to_le()
  }
  #[inline]
  #[allow(clippy::wrong_self_convention)]
  fn from_little_endian(v: u8) -> Self {
    let b = u8::from_le(v);
    Self(b)
  }
}

impl<'a> flatbuffers::Verifiable for ActionValue {
  #[inline]
  fn run_verifier(
    v: &mut flatbuffers::Verifier, pos: usize
  ) -> Result<(), flatbuffers::InvalidFlatbuffer> {
    use self::flatbuffers::Verifiable;
    u8::run_verifier(v, pos)
  }
}

impl flatbuffers::SimpleToVerifyInSlice for ActionValue {}
pub struct ActionValueUnionTableOffset {}

pub enum MousePathOffset {}
#[derive(Copy, Clone, PartialEq)]

pub struct MousePath<'a> {
  pub _tab: flatbuffers::Table<'a>,
}

impl<'a> flatbuffers::Follow<'a> for MousePath<'a> {
  type Inner = MousePath<'a>;
  #[inline]
  unsafe fn follow(buf: &'a [u8], loc: usize) -> Self::Inner {
    Self { _tab: flatbuffers::Table::new(buf, loc) }
  }
}

impl<'a> MousePath<'a> {
  pub const VT_POINTS: flatbuffers::VOffsetT = 4;

  #[inline]
  pub unsafe fn init_from_table(table: flatbuffers::Table<'a>) -> Self {
    MousePath { _tab: table }
  }
  #[allow(unused_mut)]
  pub fn create<'bldr: 'args, 'args: 'mut_bldr, 'mut_bldr, A: flatbuffers::Allocator + 'bldr>(
    _fbb: &'mut_bldr mut flatbuffers::FlatBufferBuilder<'bldr, A>,
    args: &'args MousePathArgs<'args>
  ) -> flatbuffers::WIPOffset<MousePath<'bldr>> {
    let mut builder = MousePathBuilder::new(_fbb);
    if let Some(x) = args.points { builder.add_points(x); }
    builder.finish()
  }


  #[inline]
  pub fn points(&self) -> Option<flatbuffers::Vector<'a, Vector2f>> {
    // Safety:
    // Created from valid Table for this object
    // which contains a valid value in this slot
    unsafe { self._tab.get::<flatbuffers::ForwardsUOffset<flatbuffers::Vector<'a, Vector2f>>>(MousePath::VT_POINTS, None)}
  }
}

impl flatbuffers::Verifiable for MousePath<'_> {
  #[inline]
  fn run_verifier(
    v: &mut flatbuffers::Verifier, pos: usize
  ) -> Result<(), flatbuffers::InvalidFlatbuffer> {
    use self::flatbuffers::Verifiable;
    v.visit_table(pos)?
     .visit_field::<flatbuffers::ForwardsUOffset<flatbuffers::Vector<'_, Vector2f>>>("points", Self::VT_POINTS, false)?
     .finish();
    Ok(())
  }
}
pub struct MousePathArgs<'a> {
    pub points: Option<flatbuffers::WIPOffset<flatbuffers::Vector<'a, Vector2f>>>,
}
impl<'a> Default for MousePathArgs<'a> {
  #[inline]
  fn default() -> Self {
    MousePathArgs {
      points: None,
    }
  }
}

pub struct MousePathBuilder<'a: 'b, 'b, A: flatbuffers::Allocator + 'a> {
  fbb_: &'b mut flatbuffers::FlatBufferBuilder<'a, A>,
  start_: flatbuffers::WIPOffset<flatbuffers::TableUnfinishedWIPOffset>,
}
impl<'a: 'b, 'b, A: flatbuffers::Allocator + 'a> MousePathBuilder<'a, 'b, A> {
  #[inline]
  pub fn add_points(&mut self, points: flatbuffers::WIPOffset<flatbuffers::Vector<'b , Vector2f>>) {
    self.fbb_.push_slot_always::<flatbuffers::WIPOffset<_>>(MousePath::VT_POINTS, points);
  }
  #[inline]
  pub fn new(_fbb: &'b mut flatbuffers::FlatBufferBuilder<'a, A>) -> MousePathBuilder<'a, 'b, A> {
    let start = _fbb.start_table();
    MousePathBuilder {
      fbb_: _fbb,
      start_: start,
    }
  }
  #[inline]
  pub fn finish(self) -> flatbuffers::WIPOffset<MousePath<'a>> {
    let o = self.fbb_.end_table(self.start_);
    flatbuffers::WIPOffset::new(o.value())
  }
}

impl core::fmt::Debug for MousePath<'_> {
  fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
    let mut ds = f.debug_struct("MousePath");
      ds.field("points", &self.points());
      ds.finish()
  }
}
pub enum KeyEventOffset {}
#[derive(Copy, Clone, PartialEq)]

pub struct KeyEvent<'a> {
  pub _tab: flatbuffers::Table<'a>,
}

impl<'a> flatbuffers::Follow<'a> for KeyEvent<'a> {
  type Inner = KeyEvent<'a>;
  #[inline]
  unsafe fn follow(buf: &'a [u8], loc: usize) -> Self::Inner {
    Self { _tab: flatbuffers::Table::new(buf, loc) }
  }
}

impl<'a> KeyEvent<'a> {
  pub const VT_KEY: flatbuffers::VOffsetT = 4;
  pub const VT_MOUSE_PATHS: flatbuffers::VOffsetT = 6;

  #[inline]
  pub unsafe fn init_from_table(table: flatbuffers::Table<'a>) -> Self {
    KeyEvent { _tab: table }
  }
  #[allow(unused_mut)]
  pub fn create<'bldr: 'args, 'args: 'mut_bldr, 'mut_bldr, A: flatbuffers::Allocator + 'bldr>(
    _fbb: &'mut_bldr mut flatbuffers::FlatBufferBuilder<'bldr, A>,
    args: &'args KeyEventArgs<'args>
  ) -> flatbuffers::WIPOffset<KeyEvent<'bldr>> {
    let mut builder = KeyEventBuilder::new(_fbb);
    if let Some(x) = args.mouse_paths { builder.add_mouse_paths(x); }
    builder.add_key(args.key);
    builder.finish()
  }


  #[inline]
  pub fn key(&self) -> i8 {
    // Safety:
    // Created from valid Table for this object
    // which contains a valid value in this slot
    unsafe { self._tab.get::<i8>(KeyEvent::VT_KEY, Some(0)).unwrap()}
  }
  #[inline]
  pub fn mouse_paths(&self) -> Option<flatbuffers::Vector<'a, flatbuffers::ForwardsUOffset<MousePath<'a>>>> {
    // Safety:
    // Created from valid Table for this object
    // which contains a valid value in this slot
    unsafe { self._tab.get::<flatbuffers::ForwardsUOffset<flatbuffers::Vector<'a, flatbuffers::ForwardsUOffset<MousePath>>>>(KeyEvent::VT_MOUSE_PATHS, None)}
  }
}

impl flatbuffers::Verifiable for KeyEvent<'_> {
  #[inline]
  fn run_verifier(
    v: &mut flatbuffers::Verifier, pos: usize
  ) -> Result<(), flatbuffers::InvalidFlatbuffer> {
    use self::flatbuffers::Verifiable;
    v.visit_table(pos)?
     .visit_field::<i8>("key", Self::VT_KEY, false)?
     .visit_field::<flatbuffers::ForwardsUOffset<flatbuffers::Vector<'_, flatbuffers::ForwardsUOffset<MousePath>>>>("mouse_paths", Self::VT_MOUSE_PATHS, false)?
     .finish();
    Ok(())
  }
}
pub struct KeyEventArgs<'a> {
    pub key: i8,
    pub mouse_paths: Option<flatbuffers::WIPOffset<flatbuffers::Vector<'a, flatbuffers::ForwardsUOffset<MousePath<'a>>>>>,
}
impl<'a> Default for KeyEventArgs<'a> {
  #[inline]
  fn default() -> Self {
    KeyEventArgs {
      key: 0,
      mouse_paths: None,
    }
  }
}

pub struct KeyEventBuilder<'a: 'b, 'b, A: flatbuffers::Allocator + 'a> {
  fbb_: &'b mut flatbuffers::FlatBufferBuilder<'a, A>,
  start_: flatbuffers::WIPOffset<flatbuffers::TableUnfinishedWIPOffset>,
}
impl<'a: 'b, 'b, A: flatbuffers::Allocator + 'a> KeyEventBuilder<'a, 'b, A> {
  #[inline]
  pub fn add_key(&mut self, key: i8) {
    self.fbb_.push_slot::<i8>(KeyEvent::VT_KEY, key, 0);
  }
  #[inline]
  pub fn add_mouse_paths(&mut self, mouse_paths: flatbuffers::WIPOffset<flatbuffers::Vector<'b , flatbuffers::ForwardsUOffset<MousePath<'b >>>>) {
    self.fbb_.push_slot_always::<flatbuffers::WIPOffset<_>>(KeyEvent::VT_MOUSE_PATHS, mouse_paths);
  }
  #[inline]
  pub fn new(_fbb: &'b mut flatbuffers::FlatBufferBuilder<'a, A>) -> KeyEventBuilder<'a, 'b, A> {
    let start = _fbb.start_table();
    KeyEventBuilder {
      fbb_: _fbb,
      start_: start,
    }
  }
  #[inline]
  pub fn finish(self) -> flatbuffers::WIPOffset<KeyEvent<'a>> {
    let o = self.fbb_.end_table(self.start_);
    flatbuffers::WIPOffset::new(o.value())
  }
}

impl core::fmt::Debug for KeyEvent<'_> {
  fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
    let mut ds = f.debug_struct("KeyEvent");
      ds.field("key", &self.key());
      ds.field("mouse_paths", &self.mouse_paths());
      ds.finish()
  }
}
pub enum IntValueOffset {}
#[derive(Copy, Clone, PartialEq)]

pub struct IntValue<'a> {
  pub _tab: flatbuffers::Table<'a>,
}

impl<'a> flatbuffers::Follow<'a> for IntValue<'a> {
  type Inner = IntValue<'a>;
  #[inline]
  unsafe fn follow(buf: &'a [u8], loc: usize) -> Self::Inner {
    Self { _tab: flatbuffers::Table::new(buf, loc) }
  }
}

impl<'a> IntValue<'a> {
  pub const VT_VALUE: flatbuffers::VOffsetT = 4;

  #[inline]
  pub unsafe fn init_from_table(table: flatbuffers::Table<'a>) -> Self {
    IntValue { _tab: table }
  }
  #[allow(unused_mut)]
  pub fn create<'bldr: 'args, 'args: 'mut_bldr, 'mut_bldr, A: flatbuffers::Allocator + 'bldr>(
    _fbb: &'mut_bldr mut flatbuffers::FlatBufferBuilder<'bldr, A>,
    args: &'args IntValueArgs
  ) -> flatbuffers::WIPOffset<IntValue<'bldr>> {
    let mut builder = IntValueBuilder::new(_fbb);
    builder.add_value(args.value);
    builder.finish()
  }


  #[inline]
  pub fn value(&self) -> i32 {
    // Safety:
    // Created from valid Table for this object
    // which contains a valid value in this slot
    unsafe { self._tab.get::<i32>(IntValue::VT_VALUE, Some(0)).unwrap()}
  }
}

impl flatbuffers::Verifiable for IntValue<'_> {
  #[inline]
  fn run_verifier(
    v: &mut flatbuffers::Verifier, pos: usize
  ) -> Result<(), flatbuffers::InvalidFlatbuffer> {
    use self::flatbuffers::Verifiable;
    v.visit_table(pos)?
     .visit_field::<i32>("value", Self::VT_VALUE, false)?
     .finish();
    Ok(())
  }
}
pub struct IntValueArgs {
    pub value: i32,
}
impl<'a> Default for IntValueArgs {
  #[inline]
  fn default() -> Self {
    IntValueArgs {
      value: 0,
    }
  }
}

pub struct IntValueBuilder<'a: 'b, 'b, A: flatbuffers::Allocator + 'a> {
  fbb_: &'b mut flatbuffers::FlatBufferBuilder<'a, A>,
  start_: flatbuffers::WIPOffset<flatbuffers::TableUnfinishedWIPOffset>,
}
impl<'a: 'b, 'b, A: flatbuffers::Allocator + 'a> IntValueBuilder<'a, 'b, A> {
  #[inline]
  pub fn add_value(&mut self, value: i32) {
    self.fbb_.push_slot::<i32>(IntValue::VT_VALUE, value, 0);
  }
  #[inline]
  pub fn new(_fbb: &'b mut flatbuffers::FlatBufferBuilder<'a, A>) -> IntValueBuilder<'a, 'b, A> {
    let start = _fbb.start_table();
    IntValueBuilder {
      fbb_: _fbb,
      start_: start,
    }
  }
  #[inline]
  pub fn finish(self) -> flatbuffers::WIPOffset<IntValue<'a>> {
    let o = self.fbb_.end_table(self.start_);
    flatbuffers::WIPOffset::new(o.value())
  }
}

impl core::fmt::Debug for IntValue<'_> {
  fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
    let mut ds = f.debug_struct("IntValue");
      ds.field("value", &self.value());
      ds.finish()
  }
}
pub enum FloatValueOffset {}
#[derive(Copy, Clone, PartialEq)]

pub struct FloatValue<'a> {
  pub _tab: flatbuffers::Table<'a>,
}

impl<'a> flatbuffers::Follow<'a> for FloatValue<'a> {
  type Inner = FloatValue<'a>;
  #[inline]
  unsafe fn follow(buf: &'a [u8], loc: usize) -> Self::Inner {
    Self { _tab: flatbuffers::Table::new(buf, loc) }
  }
}

impl<'a> FloatValue<'a> {
  pub const VT_VALUE: flatbuffers::VOffsetT = 4;

  #[inline]
  pub unsafe fn init_from_table(table: flatbuffers::Table<'a>) -> Self {
    FloatValue { _tab: table }
  }
  #[allow(unused_mut)]
  pub fn create<'bldr: 'args, 'args: 'mut_bldr, 'mut_bldr, A: flatbuffers::Allocator + 'bldr>(
    _fbb: &'mut_bldr mut flatbuffers::FlatBufferBuilder<'bldr, A>,
    args: &'args FloatValueArgs
  ) -> flatbuffers::WIPOffset<FloatValue<'bldr>> {
    let mut builder = FloatValueBuilder::new(_fbb);
    builder.add_value(args.value);
    builder.finish()
  }


  #[inline]
  pub fn value(&self) -> f32 {
    // Safety:
    // Created from valid Table for this object
    // which contains a valid value in this slot
    unsafe { self._tab.get::<f32>(FloatValue::VT_VALUE, Some(0.0)).unwrap()}
  }
}

impl flatbuffers::Verifiable for FloatValue<'_> {
  #[inline]
  fn run_verifier(
    v: &mut flatbuffers::Verifier, pos: usize
  ) -> Result<(), flatbuffers::InvalidFlatbuffer> {
    use self::flatbuffers::Verifiable;
    v.visit_table(pos)?
     .visit_field::<f32>("value", Self::VT_VALUE, false)?
     .finish();
    Ok(())
  }
}
pub struct FloatValueArgs {
    pub value: f32,
}
impl<'a> Default for FloatValueArgs {
  #[inline]
  fn default() -> Self {
    FloatValueArgs {
      value: 0.0,
    }
  }
}

pub struct FloatValueBuilder<'a: 'b, 'b, A: flatbuffers::Allocator + 'a> {
  fbb_: &'b mut flatbuffers::FlatBufferBuilder<'a, A>,
  start_: flatbuffers::WIPOffset<flatbuffers::TableUnfinishedWIPOffset>,
}
impl<'a: 'b, 'b, A: flatbuffers::Allocator + 'a> FloatValueBuilder<'a, 'b, A> {
  #[inline]
  pub fn add_value(&mut self, value: f32) {
    self.fbb_.push_slot::<f32>(FloatValue::VT_VALUE, value, 0.0);
  }
  #[inline]
  pub fn new(_fbb: &'b mut flatbuffers::FlatBufferBuilder<'a, A>) -> FloatValueBuilder<'a, 'b, A> {
    let start = _fbb.start_table();
    FloatValueBuilder {
      fbb_: _fbb,
      start_: start,
    }
  }
  #[inline]
  pub fn finish(self) -> flatbuffers::WIPOffset<FloatValue<'a>> {
    let o = self.fbb_.end_table(self.start_);
    flatbuffers::WIPOffset::new(o.value())
  }
}

impl core::fmt::Debug for FloatValue<'_> {
  fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
    let mut ds = f.debug_struct("FloatValue");
      ds.field("value", &self.value());
      ds.finish()
  }
}
pub enum BoolValueOffset {}
#[derive(Copy, Clone, PartialEq)]

pub struct BoolValue<'a> {
  pub _tab: flatbuffers::Table<'a>,
}

impl<'a> flatbuffers::Follow<'a> for BoolValue<'a> {
  type Inner = BoolValue<'a>;
  #[inline]
  unsafe fn follow(buf: &'a [u8], loc: usize) -> Self::Inner {
    Self { _tab: flatbuffers::Table::new(buf, loc) }
  }
}

impl<'a> BoolValue<'a> {
  pub const VT_VALUE: flatbuffers::VOffsetT = 4;

  #[inline]
  pub unsafe fn init_from_table(table: flatbuffers::Table<'a>) -> Self {
    BoolValue { _tab: table }
  }
  #[allow(unused_mut)]
  pub fn create<'bldr: 'args, 'args: 'mut_bldr, 'mut_bldr, A: flatbuffers::Allocator + 'bldr>(
    _fbb: &'mut_bldr mut flatbuffers::FlatBufferBuilder<'bldr, A>,
    args: &'args BoolValueArgs
  ) -> flatbuffers::WIPOffset<BoolValue<'bldr>> {
    let mut builder = BoolValueBuilder::new(_fbb);
    builder.add_value(args.value);
    builder.finish()
  }


  #[inline]
  pub fn value(&self) -> bool {
    // Safety:
    // Created from valid Table for this object
    // which contains a valid value in this slot
    unsafe { self._tab.get::<bool>(BoolValue::VT_VALUE, Some(false)).unwrap()}
  }
}

impl flatbuffers::Verifiable for BoolValue<'_> {
  #[inline]
  fn run_verifier(
    v: &mut flatbuffers::Verifier, pos: usize
  ) -> Result<(), flatbuffers::InvalidFlatbuffer> {
    use self::flatbuffers::Verifiable;
    v.visit_table(pos)?
     .visit_field::<bool>("value", Self::VT_VALUE, false)?
     .finish();
    Ok(())
  }
}
pub struct BoolValueArgs {
    pub value: bool,
}
impl<'a> Default for BoolValueArgs {
  #[inline]
  fn default() -> Self {
    BoolValueArgs {
      value: false,
    }
  }
}

pub struct BoolValueBuilder<'a: 'b, 'b, A: flatbuffers::Allocator + 'a> {
  fbb_: &'b mut flatbuffers::FlatBufferBuilder<'a, A>,
  start_: flatbuffers::WIPOffset<flatbuffers::TableUnfinishedWIPOffset>,
}
impl<'a: 'b, 'b, A: flatbuffers::Allocator + 'a> BoolValueBuilder<'a, 'b, A> {
  #[inline]
  pub fn add_value(&mut self, value: bool) {
    self.fbb_.push_slot::<bool>(BoolValue::VT_VALUE, value, false);
  }
  #[inline]
  pub fn new(_fbb: &'b mut flatbuffers::FlatBufferBuilder<'a, A>) -> BoolValueBuilder<'a, 'b, A> {
    let start = _fbb.start_table();
    BoolValueBuilder {
      fbb_: _fbb,
      start_: start,
    }
  }
  #[inline]
  pub fn finish(self) -> flatbuffers::WIPOffset<BoolValue<'a>> {
    let o = self.fbb_.end_table(self.start_);
    flatbuffers::WIPOffset::new(o.value())
  }
}

impl core::fmt::Debug for BoolValue<'_> {
  fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
    let mut ds = f.debug_struct("BoolValue");
      ds.field("value", &self.value());
      ds.finish()
  }
}
pub enum StringValueOffset {}
#[derive(Copy, Clone, PartialEq)]

pub struct StringValue<'a> {
  pub _tab: flatbuffers::Table<'a>,
}

impl<'a> flatbuffers::Follow<'a> for StringValue<'a> {
  type Inner = StringValue<'a>;
  #[inline]
  unsafe fn follow(buf: &'a [u8], loc: usize) -> Self::Inner {
    Self { _tab: flatbuffers::Table::new(buf, loc) }
  }
}

impl<'a> StringValue<'a> {
  pub const VT_VALUE: flatbuffers::VOffsetT = 4;

  #[inline]
  pub unsafe fn init_from_table(table: flatbuffers::Table<'a>) -> Self {
    StringValue { _tab: table }
  }
  #[allow(unused_mut)]
  pub fn create<'bldr: 'args, 'args: 'mut_bldr, 'mut_bldr, A: flatbuffers::Allocator + 'bldr>(
    _fbb: &'mut_bldr mut flatbuffers::FlatBufferBuilder<'bldr, A>,
    args: &'args StringValueArgs<'args>
  ) -> flatbuffers::WIPOffset<StringValue<'bldr>> {
    let mut builder = StringValueBuilder::new(_fbb);
    if let Some(x) = args.value { builder.add_value(x); }
    builder.finish()
  }


  #[inline]
  pub fn value(&self) -> Option<&'a str> {
    // Safety:
    // Created from valid Table for this object
    // which contains a valid value in this slot
    unsafe { self._tab.get::<flatbuffers::ForwardsUOffset<&str>>(StringValue::VT_VALUE, None)}
  }
}

impl flatbuffers::Verifiable for StringValue<'_> {
  #[inline]
  fn run_verifier(
    v: &mut flatbuffers::Verifier, pos: usize
  ) -> Result<(), flatbuffers::InvalidFlatbuffer> {
    use self::flatbuffers::Verifiable;
    v.visit_table(pos)?
     .visit_field::<flatbuffers::ForwardsUOffset<&str>>("value", Self::VT_VALUE, false)?
     .finish();
    Ok(())
  }
}
pub struct StringValueArgs<'a> {
    pub value: Option<flatbuffers::WIPOffset<&'a str>>,
}
impl<'a> Default for StringValueArgs<'a> {
  #[inline]
  fn default() -> Self {
    StringValueArgs {
      value: None,
    }
  }
}

pub struct StringValueBuilder<'a: 'b, 'b, A: flatbuffers::Allocator + 'a> {
  fbb_: &'b mut flatbuffers::FlatBufferBuilder<'a, A>,
  start_: flatbuffers::WIPOffset<flatbuffers::TableUnfinishedWIPOffset>,
}
impl<'a: 'b, 'b, A: flatbuffers::Allocator + 'a> StringValueBuilder<'a, 'b, A> {
  #[inline]
  pub fn add_value(&mut self, value: flatbuffers::WIPOffset<&'b  str>) {
    self.fbb_.push_slot_always::<flatbuffers::WIPOffset<_>>(StringValue::VT_VALUE, value);
  }
  #[inline]
  pub fn new(_fbb: &'b mut flatbuffers::FlatBufferBuilder<'a, A>) -> StringValueBuilder<'a, 'b, A> {
    let start = _fbb.start_table();
    StringValueBuilder {
      fbb_: _fbb,
      start_: start,
    }
  }
  #[inline]
  pub fn finish(self) -> flatbuffers::WIPOffset<StringValue<'a>> {
    let o = self.fbb_.end_table(self.start_);
    flatbuffers::WIPOffset::new(o.value())
  }
}

impl core::fmt::Debug for StringValue<'_> {
  fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
    let mut ds = f.debug_struct("StringValue");
      ds.field("value", &self.value());
      ds.finish()
  }
}
pub enum ActionEventOffset {}
#[derive(Copy, Clone, PartialEq)]

pub struct ActionEvent<'a> {
  pub _tab: flatbuffers::Table<'a>,
}

impl<'a> flatbuffers::Follow<'a> for ActionEvent<'a> {
  type Inner = ActionEvent<'a>;
  #[inline]
  unsafe fn follow(buf: &'a [u8], loc: usize) -> Self::Inner {
    Self { _tab: flatbuffers::Table::new(buf, loc) }
  }
}

impl<'a> ActionEvent<'a> {
  pub const VT_NAME: flatbuffers::VOffsetT = 4;
  pub const VT_VALUE_TYPE: flatbuffers::VOffsetT = 6;
  pub const VT_VALUE: flatbuffers::VOffsetT = 8;

  #[inline]
  pub unsafe fn init_from_table(table: flatbuffers::Table<'a>) -> Self {
    ActionEvent { _tab: table }
  }
  #[allow(unused_mut)]
  pub fn create<'bldr: 'args, 'args: 'mut_bldr, 'mut_bldr, A: flatbuffers::Allocator + 'bldr>(
    _fbb: &'mut_bldr mut flatbuffers::FlatBufferBuilder<'bldr, A>,
    args: &'args ActionEventArgs<'args>
  ) -> flatbuffers::WIPOffset<ActionEvent<'bldr>> {
    let mut builder = ActionEventBuilder::new(_fbb);
    if let Some(x) = args.value { builder.add_value(x); }
    if let Some(x) = args.name { builder.add_name(x); }
    builder.add_value_type(args.value_type);
    builder.finish()
  }


  #[inline]
  pub fn name(&self) -> Option<&'a str> {
    // Safety:
    // Created from valid Table for this object
    // which contains a valid value in this slot
    unsafe { self._tab.get::<flatbuffers::ForwardsUOffset<&str>>(ActionEvent::VT_NAME, None)}
  }
  #[inline]
  pub fn value_type(&self) -> ActionValue {
    // Safety:
    // Created from valid Table for this object
    // which contains a valid value in this slot
    unsafe { self._tab.get::<ActionValue>(ActionEvent::VT_VALUE_TYPE, Some(ActionValue::NONE)).unwrap()}
  }
  #[inline]
  pub fn value(&self) -> Option<flatbuffers::Table<'a>> {
    // Safety:
    // Created from valid Table for this object
    // which contains a valid value in this slot
    unsafe { self._tab.get::<flatbuffers::ForwardsUOffset<flatbuffers::Table<'a>>>(ActionEvent::VT_VALUE, None)}
  }
  #[inline]
  #[allow(non_snake_case)]
  pub fn value_as_int_value(&self) -> Option<IntValue<'a>> {
    if self.value_type() == ActionValue::IntValue {
      self.value().map(|t| {
       // Safety:
       // Created from a valid Table for this object
       // Which contains a valid union in this slot
       unsafe { IntValue::init_from_table(t) }
     })
    } else {
      None
    }
  }

  #[inline]
  #[allow(non_snake_case)]
  pub fn value_as_float_value(&self) -> Option<FloatValue<'a>> {
    if self.value_type() == ActionValue::FloatValue {
      self.value().map(|t| {
       // Safety:
       // Created from a valid Table for this object
       // Which contains a valid union in this slot
       unsafe { FloatValue::init_from_table(t) }
     })
    } else {
      None
    }
  }

  #[inline]
  #[allow(non_snake_case)]
  pub fn value_as_bool_value(&self) -> Option<BoolValue<'a>> {
    if self.value_type() == ActionValue::BoolValue {
      self.value().map(|t| {
       // Safety:
       // Created from a valid Table for this object
       // Which contains a valid union in this slot
       unsafe { BoolValue::init_from_table(t) }
     })
    } else {
      None
    }
  }

  #[inline]
  #[allow(non_snake_case)]
  pub fn value_as_string_value(&self) -> Option<StringValue<'a>> {
    if self.value_type() == ActionValue::StringValue {
      self.value().map(|t| {
       // Safety:
       // Created from a valid Table for this object
       // Which contains a valid union in this slot
       unsafe { StringValue::init_from_table(t) }
     })
    } else {
      None
    }
  }

}

impl flatbuffers::Verifiable for ActionEvent<'_> {
  #[inline]
  fn run_verifier(
    v: &mut flatbuffers::Verifier, pos: usize
  ) -> Result<(), flatbuffers::InvalidFlatbuffer> {
    use self::flatbuffers::Verifiable;
    v.visit_table(pos)?
     .visit_field::<flatbuffers::ForwardsUOffset<&str>>("name", Self::VT_NAME, false)?
     .visit_union::<ActionValue, _>("value_type", Self::VT_VALUE_TYPE, "value", Self::VT_VALUE, false, |key, v, pos| {
        match key {
          ActionValue::IntValue => v.verify_union_variant::<flatbuffers::ForwardsUOffset<IntValue>>("ActionValue::IntValue", pos),
          ActionValue::FloatValue => v.verify_union_variant::<flatbuffers::ForwardsUOffset<FloatValue>>("ActionValue::FloatValue", pos),
          ActionValue::BoolValue => v.verify_union_variant::<flatbuffers::ForwardsUOffset<BoolValue>>("ActionValue::BoolValue", pos),
          ActionValue::StringValue => v.verify_union_variant::<flatbuffers::ForwardsUOffset<StringValue>>("ActionValue::StringValue", pos),
          _ => Ok(()),
        }
     })?
     .finish();
    Ok(())
  }
}
pub struct ActionEventArgs<'a> {
    pub name: Option<flatbuffers::WIPOffset<&'a str>>,
    pub value_type: ActionValue,
    pub value: Option<flatbuffers::WIPOffset<flatbuffers::UnionWIPOffset>>,
}
impl<'a> Default for ActionEventArgs<'a> {
  #[inline]
  fn default() -> Self {
    ActionEventArgs {
      name: None,
      value_type: ActionValue::NONE,
      value: None,
    }
  }
}

pub struct ActionEventBuilder<'a: 'b, 'b, A: flatbuffers::Allocator + 'a> {
  fbb_: &'b mut flatbuffers::FlatBufferBuilder<'a, A>,
  start_: flatbuffers::WIPOffset<flatbuffers::TableUnfinishedWIPOffset>,
}
impl<'a: 'b, 'b, A: flatbuffers::Allocator + 'a> ActionEventBuilder<'a, 'b, A> {
  #[inline]
  pub fn add_name(&mut self, name: flatbuffers::WIPOffset<&'b  str>) {
    self.fbb_.push_slot_always::<flatbuffers::WIPOffset<_>>(ActionEvent::VT_NAME, name);
  }
  #[inline]
  pub fn add_value_type(&mut self, value_type: ActionValue) {
    self.fbb_.push_slot::<ActionValue>(ActionEvent::VT_VALUE_TYPE, value_type, ActionValue::NONE);
  }
  #[inline]
  pub fn add_value(&mut self, value: flatbuffers::WIPOffset<flatbuffers::UnionWIPOffset>) {
    self.fbb_.push_slot_always::<flatbuffers::WIPOffset<_>>(ActionEvent::VT_VALUE, value);
  }
  #[inline]
  pub fn new(_fbb: &'b mut flatbuffers::FlatBufferBuilder<'a, A>) -> ActionEventBuilder<'a, 'b, A> {
    let start = _fbb.start_table();
    ActionEventBuilder {
      fbb_: _fbb,
      start_: start,
    }
  }
  #[inline]
  pub fn finish(self) -> flatbuffers::WIPOffset<ActionEvent<'a>> {
    let o = self.fbb_.end_table(self.start_);
    flatbuffers::WIPOffset::new(o.value())
  }
}

impl core::fmt::Debug for ActionEvent<'_> {
  fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
    let mut ds = f.debug_struct("ActionEvent");
      ds.field("name", &self.name());
      ds.field("value_type", &self.value_type());
      match self.value_type() {
        ActionValue::IntValue => {
          if let Some(x) = self.value_as_int_value() {
            ds.field("value", &x)
          } else {
            ds.field("value", &"InvalidFlatbuffer: Union discriminant does not match value.")
          }
        },
        ActionValue::FloatValue => {
          if let Some(x) = self.value_as_float_value() {
            ds.field("value", &x)
          } else {
            ds.field("value", &"InvalidFlatbuffer: Union discriminant does not match value.")
          }
        },
        ActionValue::BoolValue => {
          if let Some(x) = self.value_as_bool_value() {
            ds.field("value", &x)
          } else {
            ds.field("value", &"InvalidFlatbuffer: Union discriminant does not match value.")
          }
        },
        ActionValue::StringValue => {
          if let Some(x) = self.value_as_string_value() {
            ds.field("value", &x)
          } else {
            ds.field("value", &"InvalidFlatbuffer: Union discriminant does not match value.")
          }
        },
        _ => {
          let x: Option<()> = None;
          ds.field("value", &x)
        },
      };
      ds.finish()
  }
}
pub enum RewindEventListOffset {}
#[derive(Copy, Clone, PartialEq)]

pub struct RewindEventList<'a> {
  pub _tab: flatbuffers::Table<'a>,
}

impl<'a> flatbuffers::Follow<'a> for RewindEventList<'a> {
  type Inner = RewindEventList<'a>;
  #[inline]
  unsafe fn follow(buf: &'a [u8], loc: usize) -> Self::Inner {
    Self { _tab: flatbuffers::Table::new(buf, loc) }
  }
}

impl<'a> RewindEventList<'a> {
  pub const VT_KEY_EVENTS: flatbuffers::VOffsetT = 4;
  pub const VT_ACTION_EVENTS: flatbuffers::VOffsetT = 6;

  #[inline]
  pub unsafe fn init_from_table(table: flatbuffers::Table<'a>) -> Self {
    RewindEventList { _tab: table }
  }
  #[allow(unused_mut)]
  pub fn create<'bldr: 'args, 'args: 'mut_bldr, 'mut_bldr, A: flatbuffers::Allocator + 'bldr>(
    _fbb: &'mut_bldr mut flatbuffers::FlatBufferBuilder<'bldr, A>,
    args: &'args RewindEventListArgs<'args>
  ) -> flatbuffers::WIPOffset<RewindEventList<'bldr>> {
    let mut builder = RewindEventListBuilder::new(_fbb);
    if let Some(x) = args.action_events { builder.add_action_events(x); }
    if let Some(x) = args.key_events { builder.add_key_events(x); }
    builder.finish()
  }


  #[inline]
  pub fn key_events(&self) -> Option<flatbuffers::Vector<'a, flatbuffers::ForwardsUOffset<KeyEvent<'a>>>> {
    // Safety:
    // Created from valid Table for this object
    // which contains a valid value in this slot
    unsafe { self._tab.get::<flatbuffers::ForwardsUOffset<flatbuffers::Vector<'a, flatbuffers::ForwardsUOffset<KeyEvent>>>>(RewindEventList::VT_KEY_EVENTS, None)}
  }
  #[inline]
  pub fn action_events(&self) -> Option<flatbuffers::Vector<'a, flatbuffers::ForwardsUOffset<ActionEvent<'a>>>> {
    // Safety:
    // Created from valid Table for this object
    // which contains a valid value in this slot
    unsafe { self._tab.get::<flatbuffers::ForwardsUOffset<flatbuffers::Vector<'a, flatbuffers::ForwardsUOffset<ActionEvent>>>>(RewindEventList::VT_ACTION_EVENTS, None)}
  }
}

impl flatbuffers::Verifiable for RewindEventList<'_> {
  #[inline]
  fn run_verifier(
    v: &mut flatbuffers::Verifier, pos: usize
  ) -> Result<(), flatbuffers::InvalidFlatbuffer> {
    use self::flatbuffers::Verifiable;
    v.visit_table(pos)?
     .visit_field::<flatbuffers::ForwardsUOffset<flatbuffers::Vector<'_, flatbuffers::ForwardsUOffset<KeyEvent>>>>("key_events", Self::VT_KEY_EVENTS, false)?
     .visit_field::<flatbuffers::ForwardsUOffset<flatbuffers::Vector<'_, flatbuffers::ForwardsUOffset<ActionEvent>>>>("action_events", Self::VT_ACTION_EVENTS, false)?
     .finish();
    Ok(())
  }
}
pub struct RewindEventListArgs<'a> {
    pub key_events: Option<flatbuffers::WIPOffset<flatbuffers::Vector<'a, flatbuffers::ForwardsUOffset<KeyEvent<'a>>>>>,
    pub action_events: Option<flatbuffers::WIPOffset<flatbuffers::Vector<'a, flatbuffers::ForwardsUOffset<ActionEvent<'a>>>>>,
}
impl<'a> Default for RewindEventListArgs<'a> {
  #[inline]
  fn default() -> Self {
    RewindEventListArgs {
      key_events: None,
      action_events: None,
    }
  }
}

pub struct RewindEventListBuilder<'a: 'b, 'b, A: flatbuffers::Allocator + 'a> {
  fbb_: &'b mut flatbuffers::FlatBufferBuilder<'a, A>,
  start_: flatbuffers::WIPOffset<flatbuffers::TableUnfinishedWIPOffset>,
}
impl<'a: 'b, 'b, A: flatbuffers::Allocator + 'a> RewindEventListBuilder<'a, 'b, A> {
  #[inline]
  pub fn add_key_events(&mut self, key_events: flatbuffers::WIPOffset<flatbuffers::Vector<'b , flatbuffers::ForwardsUOffset<KeyEvent<'b >>>>) {
    self.fbb_.push_slot_always::<flatbuffers::WIPOffset<_>>(RewindEventList::VT_KEY_EVENTS, key_events);
  }
  #[inline]
  pub fn add_action_events(&mut self, action_events: flatbuffers::WIPOffset<flatbuffers::Vector<'b , flatbuffers::ForwardsUOffset<ActionEvent<'b >>>>) {
    self.fbb_.push_slot_always::<flatbuffers::WIPOffset<_>>(RewindEventList::VT_ACTION_EVENTS, action_events);
  }
  #[inline]
  pub fn new(_fbb: &'b mut flatbuffers::FlatBufferBuilder<'a, A>) -> RewindEventListBuilder<'a, 'b, A> {
    let start = _fbb.start_table();
    RewindEventListBuilder {
      fbb_: _fbb,
      start_: start,
    }
  }
  #[inline]
  pub fn finish(self) -> flatbuffers::WIPOffset<RewindEventList<'a>> {
    let o = self.fbb_.end_table(self.start_);
    flatbuffers::WIPOffset::new(o.value())
  }
}

impl core::fmt::Debug for RewindEventList<'_> {
  fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
    let mut ds = f.debug_struct("RewindEventList");
      ds.field("key_events", &self.key_events());
      ds.field("action_events", &self.action_events());
      ds.finish()
  }
}
#[inline]
/// Verifies that a buffer of bytes contains a `RewindEventList`
/// and returns it.
/// Note that verification is still experimental and may not
/// catch every error, or be maximally performant. For the
/// previous, unchecked, behavior use
/// `root_as_rewind_event_list_unchecked`.
pub fn root_as_rewind_event_list(buf: &[u8]) -> Result<RewindEventList, flatbuffers::InvalidFlatbuffer> {
  flatbuffers::root::<RewindEventList>(buf)
}
#[inline]
/// Verifies that a buffer of bytes contains a size prefixed
/// `RewindEventList` and returns it.
/// Note that verification is still experimental and may not
/// catch every error, or be maximally performant. For the
/// previous, unchecked, behavior use
/// `size_prefixed_root_as_rewind_event_list_unchecked`.
pub fn size_prefixed_root_as_rewind_event_list(buf: &[u8]) -> Result<RewindEventList, flatbuffers::InvalidFlatbuffer> {
  flatbuffers::size_prefixed_root::<RewindEventList>(buf)
}
#[inline]
/// Verifies, with the given options, that a buffer of bytes
/// contains a `RewindEventList` and returns it.
/// Note that verification is still experimental and may not
/// catch every error, or be maximally performant. For the
/// previous, unchecked, behavior use
/// `root_as_rewind_event_list_unchecked`.
pub fn root_as_rewind_event_list_with_opts<'b, 'o>(
  opts: &'o flatbuffers::VerifierOptions,
  buf: &'b [u8],
) -> Result<RewindEventList<'b>, flatbuffers::InvalidFlatbuffer> {
  flatbuffers::root_with_opts::<RewindEventList<'b>>(opts, buf)
}
#[inline]
/// Verifies, with the given verifier options, that a buffer of
/// bytes contains a size prefixed `RewindEventList` and returns
/// it. Note that verification is still experimental and may not
/// catch every error, or be maximally performant. For the
/// previous, unchecked, behavior use
/// `root_as_rewind_event_list_unchecked`.
pub fn size_prefixed_root_as_rewind_event_list_with_opts<'b, 'o>(
  opts: &'o flatbuffers::VerifierOptions,
  buf: &'b [u8],
) -> Result<RewindEventList<'b>, flatbuffers::InvalidFlatbuffer> {
  flatbuffers::size_prefixed_root_with_opts::<RewindEventList<'b>>(opts, buf)
}
#[inline]
/// Assumes, without verification, that a buffer of bytes contains a RewindEventList and returns it.
/// # Safety
/// Callers must trust the given bytes do indeed contain a valid `RewindEventList`.
pub unsafe fn root_as_rewind_event_list_unchecked(buf: &[u8]) -> RewindEventList {
  flatbuffers::root_unchecked::<RewindEventList>(buf)
}
#[inline]
/// Assumes, without verification, that a buffer of bytes contains a size prefixed RewindEventList and returns it.
/// # Safety
/// Callers must trust the given bytes do indeed contain a valid size prefixed `RewindEventList`.
pub unsafe fn size_prefixed_root_as_rewind_event_list_unchecked(buf: &[u8]) -> RewindEventList {
  flatbuffers::size_prefixed_root_unchecked::<RewindEventList>(buf)
}
#[inline]
pub fn finish_rewind_event_list_buffer<'a, 'b, A: flatbuffers::Allocator + 'a>(
    fbb: &'b mut flatbuffers::FlatBufferBuilder<'a, A>,
    root: flatbuffers::WIPOffset<RewindEventList<'a>>) {
  fbb.finish(root, None);
}

#[inline]
pub fn finish_size_prefixed_rewind_event_list_buffer<'a, 'b, A: flatbuffers::Allocator + 'a>(fbb: &'b mut flatbuffers::FlatBufferBuilder<'a, A>, root: flatbuffers::WIPOffset<RewindEventList<'a>>) {
  fbb.finish_size_prefixed(root, None);
}
}  // pub mod fbs
}  // pub mod rewind_viewer

