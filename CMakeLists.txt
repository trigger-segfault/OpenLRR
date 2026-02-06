cmake_minimum_required(VERSION 4.0)
project(OpenLRR)

if (NOT CMAKE_SYSTEM_NAME MATCHES "Windows")
    message(FATAL_ERROR "Can't build on ${CMAKE_SYSTEM_NAME}, only Windows is supported.")
endif ()

if (NOT (CMAKE_SYSTEM_PROCESSOR MATCHES "i586.*|i686.*|i386.*|x86.*|amd64.*|AMD64.*|x86_64.*" AND "${CMAKE_SIZEOF_VOID_P}" EQUAL "4"))
    message(FATAL_ERROR "Can only build on 32-bit x86, not ${CMAKE_SYSTEM_PROCESSOR}.")
endif ()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(lib/d3drm)
add_subdirectory(src/openlrr)
add_subdirectory(src/openlrr-injector)
add_subdirectory(src/openlrr-makeexe)

option(LEGORR_DIR "Path to the LegoRR game directory")

if (LEGORR_DIR)
    foreach (target openlrr openlrr-injector openlrr-makeexe)
        set_target_properties(${target} PROPERTIES
                # For single-target generators
                RUNTIME_OUTPUT_DIRECTORY "${LEGORR_DIR}"
                LIBRARY_OUTPUT_DIRECTORY "${LEGORR_DIR}"

                # For multi-target generators
                RUNTIME_OUTPUT_DIRECTORY_RELEASE "${LEGORR_DIR}"
                LIBRARY_OUTPUT_DIRECTORY_RELEASE "${LEGORR_DIR}"
                RUNTIME_OUTPUT_DIRECTORY_DEBUG "${LEGORR_DIR}"
                LIBRARY_OUTPUT_DIRECTORY_DEBUG "${LEGORR_DIR}"
        )

        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            target_link_options(${target} PRIVATE -static-libgcc -static-libstdc++)
        endif()
    endforeach ()
endif ()
