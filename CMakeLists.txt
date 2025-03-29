cmake_minimum_required(VERSION 3.21)
project(nxdk_nv2a_register_finder)

set(
        CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/"
)

set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "Generate verbose Makefile")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(_CMAKE_PROCESSING_LANGUAGE "CXX")

include(CheckSymbolExists)
include(FindPkgConfig)

# Determine if this build is for the Xbox or the host system.
if (CMAKE_TOOLCHAIN_FILE MATCHES "toolchain-nxdk.cmake")
    set(IS_TARGET_BUILD ON)
else ()
    set(IS_TARGET_BUILD OFF)
endif ()

include(XBEUtils REQUIRED)

find_package(NXDK REQUIRED)
find_package(Threads REQUIRED)

add_subdirectory(third_party)

macro(set_opt_compile_and_link_options TARGET_NAME)
    target_compile_options(
            "${TARGET_NAME}"
            PRIVATE
            -O3
            -fno-strict-aliasing
            -Wall
            $<$<COMPILE_LANGUAGE:CXX>:-Wno-builtin-macro-redefined>   # Suppress warning from NXDK undef of __STDC_NO_THREADS__
            -D_USE_MATH_DEFINES
    )
    target_link_options(
            "${TARGET_NAME}"
            PRIVATE
            "/debug:none"
    )
endmacro()

macro(set_compile_and_link_options TARGET_NAME)
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        target_compile_options(
                "${TARGET_NAME}"
                PRIVATE
                -g
                -gdwarf-4
                -O0
                -Wall
                -fstandalone-debug
                $<$<COMPILE_LANGUAGE:CXX>:-Wno-builtin-macro-redefined>   # Suppress warning from NXDK undef of __STDC_NO_THREADS__
                -D_USE_MATH_DEFINES
        )
        target_link_options(
                "${TARGET_NAME}"
                PRIVATE
                "/debug:full"
        )
    else ()
        set_opt_compile_and_link_options("${TARGET_NAME}")
    endif ()
endmacro()

add_executable(
        nxdk_nv2a_register_finder
        src/debug_output.cpp
        src/debug_output.h
        src/main.cpp
        src/pbkit_ext.cpp
        src/pbkit_ext.h
        src/pgraph_diff_token.cpp
        src/pgraph_diff_token.h
        src/tests/set_light_control.cpp
        src/tests/set_light_control.h
        src/tests/set_light_enable.cpp
        src/tests/set_light_enable.h
        src/tests/set_specular_enable.cpp
        src/tests/set_specular_enable.h
        src/tests/set_specular_params.cpp
        src/tests/set_specular_params.h
        src/tests/test_case.cpp
        src/tests/test_case.h
)

# Pull debug info out of the binary into a host-side linked binary.
split_debug(nxdk_nv2a_register_finder)

set(EXECUTABLE_BINARY "${CMAKE_CURRENT_BINARY_DIR}/nxdk_nv2a_register_finder.exe")
set_compile_and_link_options(nxdk_nv2a_register_finder)
target_include_directories(
        nxdk_nv2a_register_finder
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_SOURCE_DIR}/third_party"
        "${CMAKE_CURRENT_BINARY_DIR}"
)

target_link_libraries(
        nxdk_nv2a_register_finder
        PRIVATE
        NXDK::NXDK
        NXDK::NXDK_CXX
        printf
)

add_xbe(
        xbe_file "${EXECUTABLE_BINARY}"
        TITLE "PGRAPH Tests"
        RESOURCE_ROOTS
        "${CMAKE_SOURCE_DIR}/resources"
        RESOURCE_DIRS
        "${CMAKE_SOURCE_DIR}/resources"
)
add_xiso(nxdk_nv2a_register_finder_xiso xbe_file)


# Deployment via xbdm_gdb_bridge - https://github.com/abaire/xbdm_gdb_bridge
#set(XBDM_GDB_BRIDGE $ENV{XBDM_GDB_BRIDGE})
#set(XBOX $ENV{XBOX})
#if (DEFINED ENV{REMOTE_PATH})
#    set(REMOTE_PATH $ENV{REMOTE_PATH})
#else ()
#    set(REMOTE_PATH "e:\\\\cmake_pgraph")
#endif ()
#
#add_custom_target(
#        deploy_with_xbdm
#        COMMAND "${XBDM_GDB_BRIDGE}" "${XBOX}" -- %syncdir "${XBE_STAGING_DIR}" "${REMOTE_PATH}" -d
#        DEPENDS "${XBE_OUTPUT_PATH}"
#)
