cmake_minimum_required(VERSION "3.12") # Minimum cmake version to support C++20
set(CMAKE_CXX_STANDARD 20) # Target C++ standard.
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Enable the target standard.

project("RedWolfSandbox" VERSION "0.5.0" DESCRIPTION "Sandbox application that uses the RedWolf library.")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${RW_BIN_DIR}/${PROJECT_NAME}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${RW_BIN_DIR}/${PROJECT_NAME}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${RW_BIN_DIR}/${PROJECT_NAME}")
set(CMAKE_INSTALL_PREFIX "${RW_BIN_DIR}/${PROJECT_NAME}/${CMAKE_BUILD_TYPE}")

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

include_directories(
    ${RW_ROOT_DIR}
    "${RW_LIB_DIR}/include"
)

add_executable("${PROJECT_NAME}"
    "main.cpp"
    "MainApplication.cpp"
    "MainWindow.cpp"
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)

target_link_libraries("${PROJECT_NAME}" 
                      "${RW_LIB_DIR}/${RW_LIB_TARGET_NAME}.lib"
)

# Copy all files required by the users in the output folder.
if (WIN32)
    file(TO_NATIVE_PATH "${RW_BIN_DIR}/${PROJECT_NAME}" NATIVE_OUTPUT_DIR)

    # Config files
    file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}" NATIVE_SOURCE_DIR)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND xcopy "${NATIVE_SOURCE_DIR}\\config" "${NATIVE_OUTPUT_DIR}\\config" /f /i /s /y
    )

    # RedWolf dll
    file(TO_NATIVE_PATH "${RW_LIB_DIR}" NATIVE_RW_LIB_DIR)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND xcopy "${NATIVE_RW_LIB_DIR}\\${RW_LIB_TARGET_NAME}.dll" "${NATIVE_OUTPUT_DIR}" /f /i /s /y
    )

    # Redwolf pdb
    if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND xcopy "${NATIVE_RW_LIB_DIR}\\${RW_LIB_TARGET_NAME}.pdb" "${NATIVE_OUTPUT_DIR}" /f /i /s /y
        )
    endif ()
endif (WIN32)