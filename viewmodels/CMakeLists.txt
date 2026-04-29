qt_add_qml_module(app_viewmodels
    URI App.ViewModels
    VERSION 1.0
    OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml/App/ViewModels"

    SOURCES
        View1ViewModel.h
        View1ViewModel.cpp

        View2ViewModel.h
        View2ViewModel.cpp

        View3ViewModel.h
        View3ViewModel.cpp
)

target_link_libraries(app_viewmodels
    PRIVATE
        Qt6::Core
        Qt6::Qml
        app_domain
        app_core
)

target_include_directories(app_viewmodels
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
