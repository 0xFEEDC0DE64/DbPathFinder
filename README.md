# DbPathFinder
Small application which shows the A* path finding algorithm.

[![Build Status](https://travis-ci.org/0xFEEDC0DE64/DbPathFinder.svg?branch=master)](https://travis-ci.org/0xFEEDC0DE64/DbPathFinder) [![Codacy Badge](https://api.codacy.com/project/badge/Grade/29d40017017840ffaf62bd8d86b9bb0e)](https://www.codacy.com/app/0xFEEDC0DE64/DbPathFinder?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=0xFEEDC0DE64/DbPathFinder&amp;utm_campaign=Badge_Grade)

![The demo showing pathfinding](https://raw.githubusercontent.com/0xFEEDC0DE64/DbPathFinder/master/demo.gif)

## Building from source
This project can only be built as part of the project structure [DbSoftware](https://github.com/0xFEEDC0DE64/DbSoftware)

```Shell
git clone https://github.com/0xFEEDC0DE64/DbSoftware.git
cd DbSoftware
git submodule update --init --recursive DbPathFinder
cd ..
mkdir build_DbSoftware
cd build_DbSoftware
qmake CONFIG+=ccache ../DbSoftware
make -j$(nproc) sub-DbPathFinder
make sub-DbPathFinder-install_subtargets
./bin/pathfinder
```
