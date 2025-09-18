cmake_minimum_required (VERSION 4.0)

project ("IndProgTask1")

find_program(CLANG_FORMAT "clang-format")

if (CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCE_FILES
        "*.cpp" "*.hpp" "*.c" "*.h"
    )

    add_custom_target(
        format
        COMMAND ${CLANG_FORMAT}
        -i
        -style=file
        ${ALL_SOURCE_FILES}
        COMMENT "Formatting all source files with clang-format..."
    )
else()
    message(WARNING "clang-format not found. The 'format' target will not be available.")
endif()

find_program(CLANG_TIDY "clang-tidy")

if (CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY
        ${CLANG_TIDY}
        -config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
        -extra-arg=-Wno-unknown-warning-option
    )
else()
    message(WARNING "clang-tidy not found. Static analysis will not be performed.")
endif()

include(${CMAKE_BINARY_DIR}/generators/conan_toolchain.cmake)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_EXPORT_COMPILE_COMMANDS YES)

find_package(Doxygen REQUIRED)

set (DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
set (DOXYGEN_EXTRACT_ALL YES)

doxygen_add_docs(docs
    ${CMAKE_SOURCE_DIR}/libcompressor/include
    ${CMAKE_SOURCE_DIR}/libcompressor/src
    ${CMAKE_SOURCE_DIR}/compressor/src
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen"
    DOXYFILE ${CMAKE_SOURCE_DIR}/Doxyfile
)

enable_testing()

add_subdirectory(compressor)

add_subdirectory(libcompressor)