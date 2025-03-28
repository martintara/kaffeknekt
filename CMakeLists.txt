cmake_minimum_required(VERSION 3.19)

project(TestGraf LANGUAGES CXX)

# Find required Qt6 modules
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

# Set up standard Qt project structure
qt_standard_project_setup()

# Define the executable and its source files
qt_add_executable(TestGraf
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    graphview.cpp
    graphview.h
)

# Link Qt libraries
target_link_libraries(TestGraf
    PRIVATE
        Qt::Core
        Qt::Widgets
)

# Install settings
include(GNUInstallDirs)

install(TARGETS TestGraf
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Generate deployment script
qt_generate_deploy_app_script(
    TARGET TestGraf
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

# Install the deploy script
install(SCRIPT ${deploy_script})
