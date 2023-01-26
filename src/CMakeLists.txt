# ================================================================================================ #
# INCLUDES
# ================================================================================================ #

# EXTERNAL MODULES (submodules)
set(EXTERNAL_INCLUDES
    PUBLIC ${CMAKE_SOURCE_DIR}/external/nlohmann/json/single_include
    )

# SRC MODULES
set(SRC_INCLUDES
    PUBLIC ${CMAKE_SOURCE_DIR}/src/include
    )


# ================================================================================================ #
# CONFIGURE FILES
# ================================================================================================ #

# SRC MODULES
configure_file(include/config/optUsage.def.in
               ${CMAKE_CURRENT_SOURCE_DIR}/include/config/optUsage.def)


# ================================================================================================ #
# LIBS (EXTERNAL LIBS & SRC LIBS)
# ================================================================================================ #

# EXTERNAL MODULES (submodules)
set(EXTERNAL_LIBS
    PUBLIC nlohmann_json::nlohmann_json
    )

# SRC MODULES
find_package (Threads REQUIRED)

add_subdirectory(filesystem)
add_subdirectory(threadpool)

add_library(SRC_LIB STATIC ${SRC_FILES})

target_link_libraries(SRC_LIB
                      PUBLIC Threads::Threads)

target_include_directories(SRC_LIB
                           PUBLIC ${SRC_INCLUDES}
                           PUBLIC ${EXTERNAL_INCLUDES})

set(SRC_LIBS
    PUBLIC SRC_LIB
    )


# ================================================================================================ #
# MAIN
# ================================================================================================ #
add_executable(main_exec main.cpp)

target_include_directories(main_exec
    ${EXTERNAL_INCLUDES}
    ${SRC_INCLUDES}
    )

target_link_libraries(main_exec
    ${EXTERNAL_LIBS}
    ${SRC_LIBS}
    )
