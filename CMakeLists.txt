project(DbPathFinder)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

set(HEADERS
    mainwindow.h
    mywidget.h
)

set(SOURCES
    main.cpp
    mainwindow.cpp
    mywidget.cpp
)

set(FORMS
    mainwindow.ui
)

add_executable(pathfinder ${HEADERS} ${SOURCES} ${FORMS})

target_link_libraries(pathfinder stdc++ Qt5::Core Qt5::Gui Qt5::Widgets)
