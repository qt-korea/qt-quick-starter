qt_add_library(app_domain STATIC
    AppDataSource.h
    AppDataSource.cpp
)

target_include_directories(app_domain
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(app_domain
    PUBLIC
        Qt6::Core
)
