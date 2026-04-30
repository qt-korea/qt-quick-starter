qt_add_qml_module(ui_kit_module
    URI App.UiKit
    VERSION 1.0
    OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml/App/UiKit"

    QML_FILES
        AppShell.qml

        components/AppButton.qml

        forms/View1Form.ui.qml
        forms/View2Form.ui.qml
        forms/View3Form.ui.qml

        views/View1.qml
        views/View2.qml
        views/View3.qml

        previews/AppPreview.qml

    IMPORTS
        QtQuick
        QtQuick.Controls
        QtQuick.Layouts
)

target_link_libraries(ui_kit_module
    PRIVATE
        Qt6::Quick
        Qt6::QuickControls2
)

install(DIRECTORY "${CMAKE_BINARY_DIR}/qml/App/UiKit/"
    DESTINATION "${QQS_QML_INSTALL_DIR}/App/UiKit"
    FILES_MATCHING
        PATTERN "qmldir"
        PATTERN "*.qmltypes"
        PATTERN "*.so"
        PATTERN "*.dylib"
        PATTERN "*.dll"
)
