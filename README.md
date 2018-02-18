## CT Viewer
This is a University Computer Graphics assignment for viewing 3D volume data.

## Dependencies
* The Qt5 SDK (version 5.8) (https://www.qt.io/download)
* CThead dataset (http://graphics.stanford.edu/data/voldata/)

## Installation
Project is built using CMake (3.8). Two variables must be specified:
* *CMAKE_PREFIX_PATH* must be set to the location of the Qt5 sdk.
* *CT_DATASET* must be set to the location  of the CThead dataset **or** it can be placed in the project source directory.
* Optionally a *CMAKE_INSTALL_PREFIX* can be given.

Example of how to build from the command line:
```bash
cd CThead
mkdir build && pushd build
cmake -DCMAKE_PREFIX_PATH:PATH=<path-to-qt-sdk> -DCMAKE_INSTALL_PREFIX:PATH=<install-path> -G "generator"..
cmake --build . --config release --target install
```
Only Visual Studio 2015 and MinGW have been tested. Visual Studio 2017 has issues compiling with Qt 5.8 at the time of writing this.
