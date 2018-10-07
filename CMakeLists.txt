project(DbPathFinder)

set(HEADERS
    mainwindow.h mywidget.h
)

set(SOURCES
    main.cpp
    mainwindow.cpp
    mywidget.cpp
)

add_executable(pathfinder ${HEADERS} ${SOURCES})

target_link_libraries(pathfinder Qt5::Core Qt5::Gui Qt5::Widgets)
