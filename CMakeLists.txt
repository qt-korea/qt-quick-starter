cmake_minimum_required(VERSION 3.21)

project(QtQuickStarter
    VERSION 0.1.0
    DESCRIPTION "Qt Quick Starter Application"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    QuickControls2
)

qt_standard_project_setup(REQUIRES 6.5)

option(BUILD_TESTING "Build unit and UI tests" ON)

add_subdirectory(domain)
add_subdirectory(viewmodels)
add_subdirectory(ui-kit)
add_subdirectory(app-main)

set(QML_IMPORT_PATH
    "${CMAKE_BINARY_DIR}/qml"
    "${CMAKE_BINARY_DIR}/app-main"
    CACHE STRING "Qt Creator QML import paths" FORCE
)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

install(TARGETS qt-quick-starter
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION bin
)
