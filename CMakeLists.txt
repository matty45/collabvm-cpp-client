# Reference: https://doc.qt.io/qt-6/cmake-manual.html

cmake_minimum_required(VERSION 3.16)

# Add folder where are supportive functions
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#Enable cmakes built-in qrc resource embedder.
set(CMAKE_AUTORCC ON)

# Include Qt basic functions
include(QtCommon)

# Basic information about project

project(collabvm-qt-client VERSION 1.0)

# Set PROJECT_VERSION_PATCH and PROJECT_VERSION_TWEAK to 0 if not present, needed by add_project_meta
fix_project_version()

set(SOURCE_FILES
    src/main.cpp
    src/ui/mainwindow.cpp
    src/cvm/ws/ws_manager.cpp
    src/cvm/cvm.cpp
    src/cvm/models/vm_list.cpp
    
)

add_project_meta(META_FILES_TO_INCLUDE)

set(RESOURCE_FILES collabvm-qt-client.qrc)

find_package(Qt6 REQUIRED COMPONENTS Widgets WebSockets Concurrent)

qt_standard_project_setup()

add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE
    ${SOURCE_FILES} ${META_FILES_TO_INCLUDE} ${RESOURCE_FILES}
 )

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

target_precompile_headers(${PROJECT_NAME} INTERFACE QtWidgets.h)

target_precompile_headers(${PROJECT_NAME} PRIVATE   
    src/cvm/models/vm_list.h
    src/cvm/ws/ws_manager.h  
    src/cvm/cvm.h  
    src/guac.h
    src/ui/mainwindow.h
)  

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt::WebSockets Qt6::Concurrent)

install(
    TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION /Applications
)

qt_generate_deploy_app_script(
    TARGET collabvm-qt-client
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})