// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace rewind_viewer.fbs
{

public enum ActionInput : byte
{
  NONE = 0,
  BoolInput = 1,
  ButtonInput = 2,
  FloatInput = 3,
  IntInput = 4,
  SelectInput = 5,
  StringInput = 6,
};



static public class ActionInputVerify
{
  static public bool Verify(Google.FlatBuffers.Verifier verifier, byte typeId, uint tablePos)
  {
    bool result = true;
    switch((ActionInput)typeId)
    {
      case ActionInput.BoolInput:
        result = rewind_viewer.fbs.BoolInputVerify.Verify(verifier, tablePos);
        break;
      case ActionInput.ButtonInput:
        result = rewind_viewer.fbs.ButtonInputVerify.Verify(verifier, tablePos);
        break;
      case ActionInput.FloatInput:
        result = rewind_viewer.fbs.FloatInputVerify.Verify(verifier, tablePos);
        break;
      case ActionInput.IntInput:
        result = rewind_viewer.fbs.IntInputVerify.Verify(verifier, tablePos);
        break;
      case ActionInput.SelectInput:
        result = rewind_viewer.fbs.SelectInputVerify.Verify(verifier, tablePos);
        break;
      case ActionInput.StringInput:
        result = rewind_viewer.fbs.StringInputVerify.Verify(verifier, tablePos);
        break;
      default: result = true;
        break;
    }
    return result;
  }
}


}
