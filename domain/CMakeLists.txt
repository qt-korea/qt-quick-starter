qt_add_library(app_domain STATIC
    AppDataSource.h
    AppDataSource.cpp
)

set_target_properties(app_domain PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(app_domain
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(app_domain
    PUBLIC
        Qt6::Core
)
