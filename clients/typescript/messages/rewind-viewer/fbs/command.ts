// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import { Arc, ArcT } from '../../rewind-viewer/fbs/arc.js';
import { CameraView, CameraViewT } from '../../rewind-viewer/fbs/camera-view.js';
import { Circle, CircleT } from '../../rewind-viewer/fbs/circle.js';
import { CircleSegment, CircleSegmentT } from '../../rewind-viewer/fbs/circle-segment.js';
import { EndFrame, EndFrameT } from '../../rewind-viewer/fbs/end-frame.js';
import { LogText, LogTextT } from '../../rewind-viewer/fbs/log-text.js';
import { Options, OptionsT } from '../../rewind-viewer/fbs/options.js';
import { Polyline, PolylineT } from '../../rewind-viewer/fbs/polyline.js';
import { Popup, PopupT } from '../../rewind-viewer/fbs/popup.js';
import { PopupRound, PopupRoundT } from '../../rewind-viewer/fbs/popup-round.js';
import { Rectangle, RectangleT } from '../../rewind-viewer/fbs/rectangle.js';
import { Tiles, TilesT } from '../../rewind-viewer/fbs/tiles.js';
import { Triangle, TriangleT } from '../../rewind-viewer/fbs/triangle.js';


export enum Command {
  NONE = 0,
  Arc = 1,
  CameraView = 2,
  Circle = 3,
  CircleSegment = 4,
  LogText = 5,
  Options = 6,
  Polyline = 7,
  Popup = 8,
  PopupRound = 9,
  Rectangle = 10,
  Tiles = 11,
  Triangle = 12,
  EndFrame = 13
}

export function unionToCommand(
  type: Command,
  accessor: (obj:Arc|CameraView|Circle|CircleSegment|EndFrame|LogText|Options|Polyline|Popup|PopupRound|Rectangle|Tiles|Triangle) => Arc|CameraView|Circle|CircleSegment|EndFrame|LogText|Options|Polyline|Popup|PopupRound|Rectangle|Tiles|Triangle|null
): Arc|CameraView|Circle|CircleSegment|EndFrame|LogText|Options|Polyline|Popup|PopupRound|Rectangle|Tiles|Triangle|null {
  switch(Command[type]) {
    case 'NONE': return null; 
    case 'Arc': return accessor(new Arc())! as Arc;
    case 'CameraView': return accessor(new CameraView())! as CameraView;
    case 'Circle': return accessor(new Circle())! as Circle;
    case 'CircleSegment': return accessor(new CircleSegment())! as CircleSegment;
    case 'LogText': return accessor(new LogText())! as LogText;
    case 'Options': return accessor(new Options())! as Options;
    case 'Polyline': return accessor(new Polyline())! as Polyline;
    case 'Popup': return accessor(new Popup())! as Popup;
    case 'PopupRound': return accessor(new PopupRound())! as PopupRound;
    case 'Rectangle': return accessor(new Rectangle())! as Rectangle;
    case 'Tiles': return accessor(new Tiles())! as Tiles;
    case 'Triangle': return accessor(new Triangle())! as Triangle;
    case 'EndFrame': return accessor(new EndFrame())! as EndFrame;
    default: return null;
  }
}

export function unionListToCommand(
  type: Command, 
  accessor: (index: number, obj:Arc|CameraView|Circle|CircleSegment|EndFrame|LogText|Options|Polyline|Popup|PopupRound|Rectangle|Tiles|Triangle) => Arc|CameraView|Circle|CircleSegment|EndFrame|LogText|Options|Polyline|Popup|PopupRound|Rectangle|Tiles|Triangle|null, 
  index: number
): Arc|CameraView|Circle|CircleSegment|EndFrame|LogText|Options|Polyline|Popup|PopupRound|Rectangle|Tiles|Triangle|null {
  switch(Command[type]) {
    case 'NONE': return null; 
    case 'Arc': return accessor(index, new Arc())! as Arc;
    case 'CameraView': return accessor(index, new CameraView())! as CameraView;
    case 'Circle': return accessor(index, new Circle())! as Circle;
    case 'CircleSegment': return accessor(index, new CircleSegment())! as CircleSegment;
    case 'LogText': return accessor(index, new LogText())! as LogText;
    case 'Options': return accessor(index, new Options())! as Options;
    case 'Polyline': return accessor(index, new Polyline())! as Polyline;
    case 'Popup': return accessor(index, new Popup())! as Popup;
    case 'PopupRound': return accessor(index, new PopupRound())! as PopupRound;
    case 'Rectangle': return accessor(index, new Rectangle())! as Rectangle;
    case 'Tiles': return accessor(index, new Tiles())! as Tiles;
    case 'Triangle': return accessor(index, new Triangle())! as Triangle;
    case 'EndFrame': return accessor(index, new EndFrame())! as EndFrame;
    default: return null;
  }
}
