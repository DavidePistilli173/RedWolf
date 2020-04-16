cmake_minimum_required(VERSION "3.8")

project("RedWolfEngine" VERSION "0.0.0" DESCRIPTION "")

set(REDWOLF_SOURCE_DIR "${PROJECT_SOURCE_DIR}/RedWolf")
set(REDWOLFEDITOR_SOURCE_DIR "${PROJECT_SOURCE_DIR}/RedWolfEditor")
set(REDWOLFTESTS_SOURCE_DIR "${PROJECT_SOURCE_DIR}/RedWolfTests")
set(VENDOR_DIR "${PROJECT_SOURCE_DIR}/vendor")
set(OBJ_DIR "${PROJECT_SOURCE_DIR}/obj")
set(BIN_DIR "${PROJECT_SOURCE_DIR}/bin")

enable_testing()

add_subdirectory("${REDWOLF_SOURCE_DIR}")
add_subdirectory("${REDWOLFEDITOR_SOURCE_DIR}")
add_subdirectory("${REDWOLFTESTS_SOURCE_DIR}")

# Copy DLLs to executable folders.
if (WIN32)
    # RedWolfEditor
    file(COPY "${BIN_DIR}/RedWolf/RedWolf.dll" DESTINATION "${BIN_DIR}/RedWolfEditor/")
    file(COPY "${VENDOR_DIR}/lib/SDL2.0.9/SDL2.dll" DESTINATION "${BIN_DIR}/RedWolfEditor/")
    # RedWolfTests
    file(COPY "${BIN_DIR}/RedWolf/RedWolf.dll" DESTINATION "${BIN_DIR}/RedWolfTests/")
    file(COPY "${VENDOR_DIR}/lib/SDL2.0.9/SDL2.dll" DESTINATION "${BIN_DIR}/RedWolfTests/")
endif (WIN32)