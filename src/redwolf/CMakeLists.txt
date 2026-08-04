add_library(redwolf STATIC)

target_sources(redwolf PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/boot/entry_point.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/logger.cpp)

target_include_directories(redwolf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
