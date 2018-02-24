## CT Viewer
This is a University Computer Graphics assignment for viewing 3D volume data.

## Dependencies
* The Qt5 SDK (version 5.8) (https://www.qt.io/download)
* CThead dataset (http://graphics.stanford.edu/data/voldata/)

## Installation
Project is built using CMake (3.8). Two variables must be specified:
* *CMAKE_PREFIX_PATH* must be set to the location of the Qt5 sdk.
* Optionally *CT_DATASET* can be set to the location  of the CThead dataset **or** it can be placed in the project source directory.
* Optionally a *CMAKE_INSTALL_PREFIX* can be given.

Example of how to build from the command line:
```bash
cd project/source/directory
mkdir build && pushd build
cmake -G "Visual Studio 14 2015 Win64" ..
cmake -DCMAKE_INSTALL_PREFIX:PATH=./install .
cmake -DCMAKE_PREFIX_PATH:PATH=C:/Qt/5.8/msvc2015_64/ .
cmake -DCT_DATASET=some/external/path/CThead .
cmake --build . --config release --target install
```

Only Visual Studio 2015 and MinGW have been tested. Visual Studio 2017 has issues compiling with Qt 5.8 at the time of writing this.
