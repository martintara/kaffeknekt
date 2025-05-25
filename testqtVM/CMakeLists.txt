cmake_minimum_required(VERSION 3.5)

project(HelloWorldQt)

# Sett C++-standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Finn Qt5 Widgets-pakken
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Opprett eksekverbart mål med kildene våre
add_executable(HelloWorldQt
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

target_link_libraries(HelloWorldQt Qt6::Widgets)
