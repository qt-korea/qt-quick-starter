qt_add_qml_module(app_viewmodels
    URI App.ViewModels
    VERSION 1.0
    NO_PLUGIN
    OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml/App/ViewModels"

    SOURCES
        AppDataViewModel.h
        AppDataViewModel.cpp
)

target_link_libraries(app_viewmodels
    PRIVATE
        Qt6::Core
        Qt6::Qml
        app_domain
)

target_include_directories(app_viewmodels
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
