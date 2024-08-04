// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import { BoolInput } from '../../rewind-viewer/fbs/bool-input.js';
import { ButtonInput } from '../../rewind-viewer/fbs/button-input.js';
import { FloatInput } from '../../rewind-viewer/fbs/float-input.js';
import { IntInput } from '../../rewind-viewer/fbs/int-input.js';
import { SelectInput } from '../../rewind-viewer/fbs/select-input.js';
import { StringInput } from '../../rewind-viewer/fbs/string-input.js';


export enum ActionInput {
  NONE = 0,
  BoolInput = 1,
  ButtonInput = 2,
  FloatInput = 3,
  IntInput = 4,
  SelectInput = 5,
  StringInput = 6
}

export function unionToActionInput(
  type: ActionInput,
  accessor: (obj:BoolInput|ButtonInput|FloatInput|IntInput|SelectInput|StringInput) => BoolInput|ButtonInput|FloatInput|IntInput|SelectInput|StringInput|null
): BoolInput|ButtonInput|FloatInput|IntInput|SelectInput|StringInput|null {
  switch(ActionInput[type]) {
    case 'NONE': return null; 
    case 'BoolInput': return accessor(new BoolInput())! as BoolInput;
    case 'ButtonInput': return accessor(new ButtonInput())! as ButtonInput;
    case 'FloatInput': return accessor(new FloatInput())! as FloatInput;
    case 'IntInput': return accessor(new IntInput())! as IntInput;
    case 'SelectInput': return accessor(new SelectInput())! as SelectInput;
    case 'StringInput': return accessor(new StringInput())! as StringInput;
    default: return null;
  }
}

export function unionListToActionInput(
  type: ActionInput, 
  accessor: (index: number, obj:BoolInput|ButtonInput|FloatInput|IntInput|SelectInput|StringInput) => BoolInput|ButtonInput|FloatInput|IntInput|SelectInput|StringInput|null, 
  index: number
): BoolInput|ButtonInput|FloatInput|IntInput|SelectInput|StringInput|null {
  switch(ActionInput[type]) {
    case 'NONE': return null; 
    case 'BoolInput': return accessor(index, new BoolInput())! as BoolInput;
    case 'ButtonInput': return accessor(index, new ButtonInput())! as ButtonInput;
    case 'FloatInput': return accessor(index, new FloatInput())! as FloatInput;
    case 'IntInput': return accessor(index, new IntInput())! as IntInput;
    case 'SelectInput': return accessor(index, new SelectInput())! as SelectInput;
    case 'StringInput': return accessor(index, new StringInput())! as StringInput;
    default: return null;
  }
}
