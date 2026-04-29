find_package(Qt6 REQUIRED COMPONENTS Test QuickTest)

# ── Domain unit test (no display needed) ─────────────────────────────────────
qt_add_executable(tst_app_datasource
    domain/tst_app_datasource.cpp
)
target_link_libraries(tst_app_datasource
    PRIVATE Qt6::Test app_domain
)
add_test(NAME tst_app_datasource COMMAND tst_app_datasource)

# ── ViewModel unit test (no display needed) ──────────────────────────────────
#
#   Proves the AppServices injection pattern documented in
#   ARCHITECTURE.md → "Wiring domain services": each test constructs only
#   the services it needs, no globals to reset between tests, no display.
#
qt_add_executable(tst_view1_viewmodel
    viewmodels/tst_view1_viewmodel.cpp
)
target_link_libraries(tst_view1_viewmodel
    PRIVATE Qt6::Test app_core app_domain app_viewmodels
)
add_test(NAME tst_view1_viewmodel COMMAND tst_view1_viewmodel)

# ── UI visual test via QtQuickTest ───────────────────────────────────────────
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
