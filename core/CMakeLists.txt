# =============================================================================
#  core/CMakeLists.txt
#
#  The reusable infrastructure of this template:
#    - ViewModelBase   (abstract base class for all page ViewModels)
#    - AppServices     (struct of pointers to long-lived domain services)
#    - ViewNavigator   (QML_SINGLETON — route table + ViewModel factory)
#
#  Treat this folder as a small in-tree library. Other folders depend on
#  it; you should not need to edit it as your project grows.
#
#  This is a C++ QML module because ViewNavigator is registered as a
#  QML_SINGLETON under the URI "App.Core". The other C++ types are also
#  registered (QML_ELEMENT / QML_UNCREATABLE) so QML knows their property
#  metadata even though it cannot construct them directly.
# =============================================================================

qt_add_qml_module(app_core
    URI App.Core
    VERSION 1.0
    OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml/App/Core"

    SOURCES
        ViewModelBase.h
        ViewModelBase.cpp

        AppServices.h

        ViewNavigator.h
        ViewNavigator.cpp
)

target_link_libraries(app_core
    PRIVATE
        Qt6::Core
        Qt6::Qml
)

target_include_directories(app_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
