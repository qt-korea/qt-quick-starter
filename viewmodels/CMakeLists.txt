qt_add_qml_module(app_viewmodels
    URI App.ViewModels
    VERSION 1.0
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

install(TARGETS app_viewmodels
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY "${CMAKE_BINARY_DIR}/qml/App/ViewModels/"
    DESTINATION "${QQS_QML_INSTALL_DIR}/App/ViewModels"
    FILES_MATCHING
        PATTERN "qmldir"
        PATTERN "*.qmltypes"
        PATTERN "*.so"
        PATTERN "*.dylib"
        PATTERN "*.dll"
)
