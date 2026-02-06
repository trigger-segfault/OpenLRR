project(d3drm)

include(CheckIncludeFile)

check_include_file(d3drm.h HAVE_D3DRM_H)

##
# Iff we don't have a system d3drm, use our vendored one.
# When other things link to "d3drm", it'll pick up this instead.
##
if(NOT HAVE_D3DRM_H)
        message(STATUS "No local d3drm.h found, using vendored version.")
        add_library(d3drm INTERFACE)
        target_include_directories(d3drm INTERFACE "${CMAKE_CURRENT_LIST_DIR}/inc")
        target_link_libraries(d3drm INTERFACE "${CMAKE_CURRENT_LIST_DIR}/d3drm.lib")
endif()
