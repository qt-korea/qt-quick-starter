qt_add_qml_module(ui_kit_module
    URI App.UiKit
    VERSION 1.0
    OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml/App/UiKit"

    QML_FILES
        AppShell.qml

        # Components (reusable UI primitives)
        components/AppButton.qml

        # Forms (pure layout)
        forms/View1Form.ui.qml
        forms/View2Form.ui.qml
        forms/View3Form.ui.qml

        # Views (ViewModel binding + navigation)
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
