## CThead Viewer
University Computer Graphics assignment for viewing 3D volume data.

## Dependencies
* Qt5
* CThead dataset: http://graphics.stanford.edu/data/voldata/

## Installation
Project is built using CMake (preferably at least version 3.8).
Example of how to build from the command line:

cd CThead
mkdir build
cmake -DQt5_DIR:PATH=<path-to-qt-sdk> -DCMAKE_INSTALL_PREFIX:PATH=<install-path> ..
cmake --build . --config release --target install
