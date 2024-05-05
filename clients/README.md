# Clients

 - C++17
 - C++17 cmake project
 - C# **[Not Implemented]**
 - Go **[Not Implemented]**
 - Java **[Not Implemented]**
 - Kotlin **[Not Implemented]**
 - Python3 **[Not Implemented]**
 - rust **[Not Implemented]**
 - TtypeScript **[Not Implemented]**

Original clients [implementation](https://github.com/kswaldemar/rewind-viewer/tree/develop/clients) can be used as reference.

# Message protocol

Rewind viewer uses FlatBuffers for control message serialization. See [message schema](https://github.com/mortido/rewind-viewer/blob/develop/fbs/rewind_message.fbs) for more details.

Viewer accepts the following types of commands:
- ColorCircle
- Rectangle
- Triangle
- Polyline
- LogText
- Popup
- PopupRound
- Options
- EndFrame

### Vector2f
Structure for 2D coordinates and sizes
```
x:float;
y:float;
```

### Color
Color value is encoded as uint32 in form `ARGB`
But zero alpha component means fully opaque object instead of invisible one.
```
value:uint;
fill:bool;   # used by some primitives to fill them with specified color
```
Example:
```
0xff0000    # red
0xffff0000  # exactly the same as above, but aplha set to 255 manually (fully opaque)
0x7700ff00  # transparent green
0x0000ff    # blue
0x000033    # dark blue
```

### ColorCircle
```
color:Color;        # Color (fill supported)
center:Vector2f;    # Center position
radius:float;       # Radius
```

### Rectangle
```
color:Color;        # Color (fill supported)
position:Vector2f;  # Position (least x and y coordinates)
size:Vector2f;      # Size (width, height)  
```
:information_source: If you mix up `tl` and `br` positions they will be normalized

### Ttriangle
```
color:Color;        # Color (fill supported)
points:[Vector2f];  # Exactly 3 points
```

### Polyline
```
color:Color;        # Color (fill not supported)
points:[Vector2f];  # Arbitrary points count (atleast 2 points)
```

### LogText
Messages will be shown in `Frame message` window inside viewer.
You can send several messages during one frame. Newline added automatically after each message
Escape characters also allowed, e.g. you can send `multilined\nstring`
```
text:string
```

### Popup
Shows popup window with message when cursor overlaps with given rectangular area.
```
text:string;             # Text to be displayed (escape characters as \n are allowed)
area_position:Vector2f;  # Position (least x and y coordinates)
area_size:Vector2f;      # Size of area (width, heigh)
```

### PopupRound:
Shows popup window with message when cursor overlaps with given circular area.
```
text:string;             # Text to be displayed (escape characters as \n are allowed)
area_center:Vector2f     # Center position
area_radius:float;       # Area radius
```

### Options
```
map:Map;
layer:Layer;

Layer {
  id:uint;                   # Set active layer (0-9) for primitives
  use_permanent_frame:bool;  # Whenever to draw primitives to permanent layer
}

Map {
  width:float;     # Map canvas width
  height:float;    # Map canvas height
  x_grid:uint;     # Map x cells count (used to draw grid)
  y_grid:uint;     # Map y cells count (used to draw grid)
}
```

### End
Don't have any data. Indicates and of frame. All objects will be drawn after it (if immediate mode is disabled).
