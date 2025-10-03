cmake_minimum_required(VERSION 3.12)
project(collabvm-cpp-client)
set(CMAKE_CXX_STANDARD 26)

# add dependencies
include(cmake/CPM.cmake)
set(CPM_USE_LOCAL_PACKAGES ON)
set(CPM_SOURCE_CACHE ${CMAKE_CURRENT_LIST_DIR}/CPMCache)

include(cmake/cpm/zlib.cmake)
include(cmake/cpm/mbedtls.cmake)
include(cmake/cpm/ixwebsocket.cmake)

#Get helloimgui
CPMAddPackage("gh:pthom/imgui_bundle@1.92.4")

# Disable stuff we will never use.
set(IMGUI_BUNDLE_DISABLE_IMGUI_NODE_EDITOR ON CACHE BOOL "" FORCE)
set(IMGUI_BUNDLE_DISABLE_IMGUI_TEX_INSPECT ON CACHE BOOL "" FORCE)
set(IMGUI_BUNDLE_DISABLE_IMGUIZMO ON CACHE BOOL "" FORCE)
set(IMGUI_BUNDLE_DISABLE_IMMVISION ON CACHE BOOL "" FORCE)

#Cannot disable implot as for some reason it still tries to get linked and causes a linker error due to missing implot.lib
#set(IMGUI_BUNDLE_DISABLE_IMPLOT ON CACHE BOOL "" FORCE)
#set(IMGUI_BUNDLE_DISABLE_IMPLOT3D ON CACHE BOOL "" FORCE)

set(IMGUI_BUNDLE_DISABLE_NANOVG ON CACHE BOOL "" FORCE)


# Build your app
# ==============
# imgui_bundle_add_app is a helper function, similar to cmake's "add_executable"
# Usage:
# imgui_bundle_add_app(app_name file1.cpp file2.cpp ...)
#
# Features:
# * It will automatically link the target to the required libraries (hello_imgui, OpenGl, glad, etc)
# * It will embed the assets (for desktop, mobile, and emscripten apps)
# * It will perform additional customization (app icon and name on mobile platforms, etc)
#
# Now you can build your app with
#     mkdir build && cd build && cmake .. && cmake --build .

# By default, we install in a portable way (i.e. assets and executable are in the same folder)
imgui_bundle_add_app(collabvm-cpp-client main.cpp client.cpp)

target_compile_definitions(ixwebsocket PRIVATE IXWEBSOCKET_USE_MBED_TLS_MIN_VERSION_3)
target_link_libraries(collabvm-cpp-client PRIVATE ixwebsocket)
