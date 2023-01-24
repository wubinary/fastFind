cmake_minimum_required(VERSION 3.16.3)

# INIT PROJ AND GET PROJ VERSION
project(QUICKFIND)

file(STRINGS "${PROJECT_SOURCE_DIR}/VERSION" PROJECT_VERSION)

string(REGEX MATCH "([0-9]*).[0-9]*.[0-9]*" _ ${PROJECT_VERSION})
set(PROJECT_VERSION_MAJOR ${CMAKE_MATCH_1})

string(REGEX MATCH "[0-9]*.([0-9]*).[0-9]*" _ ${PROJECT_VERSION})
set(PROJECT_VERSION_MINOR ${CMAKE_MATCH_1})

string(REGEX MATCH "[0-9]*.[0-9]*.([0-9]*)" _ ${PROJECT_VERSION})
set(PROJECT_VERSION_PATCH ${CMAKE_MATCH_1})

# CXX VERSION
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

# DOWNLOAD ALL SUBMODULES
find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if (GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, "
                                "please check submodule")
        endif()
    endif()
endif()

# CHECK ALL SUBMODULES
# TODO:

# ADD SUBMODULES DIRS
add_subdirectory(external/nlohmann/json)

# BUILD FLAGS
if (BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG=TRUE)
endif()

# SRC CODES
add_subdirectory(src)
