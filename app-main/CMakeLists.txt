qt_add_executable(qt-quick-starter
    main.cpp
)

qt_add_qml_module(qt-quick-starter
    URI App.Integration
    VERSION 1.0

    QML_FILES
        Main.qml

    IMPORTS
        QtQuick
        QtQuick.Controls
        App.UiKit
        App.ViewModels
)

target_link_libraries(qt-quick-starter
    PRIVATE
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
        app_domain
        app_viewmodels
        app_viewmodelsplugin
        ui_kit_module
        ui_kit_moduleplugin
)

set_target_properties(qt-quick-starter PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
