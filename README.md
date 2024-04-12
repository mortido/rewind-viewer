# Rewind viewer [mortido remix]

[![MIT License](https://img.shields.io/github/license/mortido/rewind-viewer.svg?style=flat-square)](./LICENSE)
[![C++ standard](https://img.shields.io/badge/C++-17-blue.svg?style=flat-square)](https://isocpp.org/)
[![OpenGL](https://img.shields.io/badge/OpenGL-3.3-green.svg?style=flat-square)](https://www.khronos.org/opengl/)

[//]: # ([![GitHub Releases]&#40;https://img.shields.io/github/release/mortido/rewind-viewer.svg?style=flat-square&#41;]&#40;https://github.com/kswaldemar/rewind-viewer/releases&#41;)

Remix of great tool for visualization for bot competitions.
Original Rewind Viewer writen by Vladimir Kiselev (aka kswaldemar) is located [here](https://github.com/kswaldemar/rewind-viewer).

![](https://user-images.githubusercontent.com/10009947/101282773-06be3080-37e8-11eb-9edd-47e30e58e2a0.png)

## Overview

Key differences with original rewind viewer:
 - binary protocol for faster communication and more primitives per frame
 - updated API (e.g. map size option)
 - bugfixes

Original features:
 - All figures are drawn using your video adapter, so no more problems with slow drawing
 - Rewinding - ability to navigate between game ticks
 - In Pause navigation - zoom and navigate in any game state
 - Handy mouse navigation

Drawbacks:
 - Viewer running as standalone application, it knows nothing about your strategy, so you need manually 
send all data and you can draw only data visible by your strategy
 - In theory, high memory usage, because it needs to store all drawing primitives for rewinding support


## Binaries

There is no binaries for now. You should build rewind-viewer yourself.

[//]: # (It is required to have x86 C++ Redistributable for Visual Studio 2015 installed to run prebuild windows binaries.)


## Build

Clone repository with submodules:
```bash
git clone --recursive https://github.com/mortido/rewind-viewer.git
cd rewind-viewer
```

Linux:

[![Linux Build status](https://github.com/mortido/rewind-viewer/actions/workflows/linux_build.yml/badge.svg)](https://github.com/mortido/rewind-viewer/actions)

```bash
# Install dependencies
sudo apt-get update && sudo apt-get install -y libglu1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev 

mkdir build
cmake -S . -B build -D CMAKE_BUILD_TYPE=Release
cmake --build build --target rewind-viewer -j
```

MacOS:

[![Macos Build status](https://github.com/mortido/rewind-viewer/actions/workflows/macos_build.yml/badge.svg)](https://github.com/mortido/rewind-viewer/actions)

```bash
mkdir build && cd build
cmake --CMAKE_BUILD_TYPE=Release ..
cmake --build .
```

Windows:

[![Windows Build status](https://github.com/mortido/rewind-viewer/actions/workflows/windows_build.yml/badge.svg)](https://github.com/mortido/rewind-viewer/actions)

```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```
*Note*: Compiler with c++17 support needed. 

## Usage

Run Rewind Viewer before starting you code. It will reset itself on new connection, so no restarts required.

See [clients](https://github.com/mortido/rewind-viewer/tree/develop/clients) for more information regardless specific language usage.

## Create client four your language

 - Generate flatbuffers messages for your language. See [Makefile](https://github.com/mortido/rewind-viewer/blob/develop/Makefile) for more info. You will need FlatBuffers compiler (flatc) to be installed.
```bash
# Example of message generation for c++
make cpp_client
```
 - Use message protocol and existent clients as [guidelines](https://github.com/mortido/rewind-viewer/tree/develop/clients)

## License
Project sources distributed under [MIT license](https://github.com/mortido/rewind-viewer/blob/develop/LICENSE), third parties distributed under their own licences

## Credits
Project created with help of many great libraries:
 - [glad](https://github.com/Dav1dde/glad) for loading OpenGL functions
 - [glm](https://glm.g-truc.net/0.9.8/index.html) math library for OpenGL
 - [glfw](http://www.glfw.org/) for creating window and crossplatform context handling
 - [ImGui](https://github.com/ocornut/imgui) for UI widgets inside application
 - [clsocket](https://github.com/DFHack/clsocket) for network interaction
 - [flatbuffers](https://github.com/google/flatbuffers) for binary messages
 - [stb_image](https://github.com/nothings/stb) for images processing
 - [loguru](https://github.com/emilk/loguru) for logging support

Resources: 
 - [fontawesome](http://fontawesome.io/) icon font, embedded inside text in many UI elements
 - Application icon by [Laura Reen](https://www.iconfinder.com/laurareen)  
 
