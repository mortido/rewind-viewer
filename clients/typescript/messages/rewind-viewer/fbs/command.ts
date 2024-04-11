// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import { Circle, CircleT } from '../../rewind-viewer/fbs/circle.js';
import { EndFrame, EndFrameT } from '../../rewind-viewer/fbs/end-frame.js';
import { LogText, LogTextT } from '../../rewind-viewer/fbs/log-text.js';
import { Options, OptionsT } from '../../rewind-viewer/fbs/options.js';
import { Polyline, PolylineT } from '../../rewind-viewer/fbs/polyline.js';
import { Popup, PopupT } from '../../rewind-viewer/fbs/popup.js';
import { PopupRound, PopupRoundT } from '../../rewind-viewer/fbs/popup-round.js';
import { Rectangle, RectangleT } from '../../rewind-viewer/fbs/rectangle.js';
import { Triangle, TriangleT } from '../../rewind-viewer/fbs/triangle.js';


export enum Command {
  NONE = 0,
  Circle = 1,
  Rectangle = 2,
  Triangle = 3,
  Polyline = 4,
  LogText = 5,
  Popup = 6,
  PopupRound = 7,
  Options = 8,
  EndFrame = 9
}

export function unionToCommand(
  type: Command,
  accessor: (obj:Circle|EndFrame|LogText|Options|Polyline|Popup|PopupRound|Rectangle|Triangle) => Circle|EndFrame|LogText|Options|Polyline|Popup|PopupRound|Rectangle|Triangle|null
): Circle|EndFrame|LogText|Options|Polyline|Popup|PopupRound|Rectangle|Triangle|null {
  switch(Command[type]) {
    case 'NONE': return null; 
    case 'Circle': return accessor(new Circle())! as Circle;
    case 'Rectangle': return accessor(new Rectangle())! as Rectangle;
    case 'Triangle': return accessor(new Triangle())! as Triangle;
    case 'Polyline': return accessor(new Polyline())! as Polyline;
    case 'LogText': return accessor(new LogText())! as LogText;
    case 'Popup': return accessor(new Popup())! as Popup;
    case 'PopupRound': return accessor(new PopupRound())! as PopupRound;
    case 'Options': return accessor(new Options())! as Options;
    case 'EndFrame': return accessor(new EndFrame())! as EndFrame;
    default: return null;
  }
}

export function unionListToCommand(
  type: Command, 
  accessor: (index: number, obj:Circle|EndFrame|LogText|Options|Polyline|Popup|PopupRound|Rectangle|Triangle) => Circle|EndFrame|LogText|Options|Polyline|Popup|PopupRound|Rectangle|Triangle|null, 
  index: number
): Circle|EndFrame|LogText|Options|Polyline|Popup|PopupRound|Rectangle|Triangle|null {
  switch(Command[type]) {
    case 'NONE': return null; 
    case 'Circle': return accessor(index, new Circle())! as Circle;
    case 'Rectangle': return accessor(index, new Rectangle())! as Rectangle;
    case 'Triangle': return accessor(index, new Triangle())! as Triangle;
    case 'Polyline': return accessor(index, new Polyline())! as Polyline;
    case 'LogText': return accessor(index, new LogText())! as LogText;
    case 'Popup': return accessor(index, new Popup())! as Popup;
    case 'PopupRound': return accessor(index, new PopupRound())! as PopupRound;
    case 'Options': return accessor(index, new Options())! as Options;
    case 'EndFrame': return accessor(index, new EndFrame())! as EndFrame;
    default: return null;
  }
}
