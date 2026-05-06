find_package(Qt6 REQUIRED COMPONENTS Test QuickTest)

qt_add_executable(tst_app_datasource
    domain/tst_app_datasource.cpp
)
target_link_libraries(tst_app_datasource
    PRIVATE Qt6::Test app_domain
)
add_test(NAME tst_app_datasource COMMAND tst_app_datasource)

qt_add_executable(tst_ui_kit
    ui-visual/main.cpp
)
target_link_libraries(tst_ui_kit
    PRIVATE Qt6::QuickTest Qt6::Qml ui_kit_module
)
add_test(NAME tst_ui_kit
         COMMAND tst_ui_kit -input ${CMAKE_CURRENT_SOURCE_DIR}/ui-visual)
set_property(TEST tst_ui_kit APPEND PROPERTY ENVIRONMENT
    QML_IMPORT_PATH=${CMAKE_BINARY_DIR}/qml)
