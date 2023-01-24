# ================================================================================================ #
# EXTERNAL LIBS
# ================================================================================================ #

# EXTERNAL MODULES (submodules)
set(EXTERNAL_INCLUDES
    PUBLIC ${CMAKE_SOURCE_DIR}/external/nlohmann/json/single_include
    )

set(EXTERNAL_LIBS
    PRIVATE nlohmann_json::nlohmann_json
    )


# ================================================================================================ #
# SRC LIBS
# ================================================================================================ #

# SRC/LIBS MODULES
set(SRC_INCLUDES
    PUBLIC ${CMAKE_SOURCE_DIR}/src/include
    )

set(SRC_LIBS
    PRIVATE src
    )


# ================================================================================================ #
# SRC MAIN
# ================================================================================================ #
configure_file(optUsage.def.in ${CMAKE_CURRENT_SOURCE_DIR}/optUsage.def)

add_executable(main_exec main.cpp)

target_include_directories(main_exec
    ${EXTERNAL_INCLUDES}
    ${SRC_INCLUDES}
    )

target_link_libraries(main_exec
    ${EXTERNAL_LIBS}
    #${SRC_LIBS}
    )

