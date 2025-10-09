# Reference: https://doc.qt.io/qt-6/cmake-manual.html

cmake_minimum_required(VERSION 3.16)

# Add folder where are supportive functions
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Include Qt basic functions
include(QtCommon)

# Basic information about project

project(collabvm-qt-client VERSION 1.0)

# Set PROJECT_VERSION_PATCH and PROJECT_VERSION_TWEAK to 0 if not present, needed by add_project_meta
fix_project_version()

set(SOURCE_FILES
    src/main.cpp
    src/mainwindow.cpp
)

add_project_meta(META_FILES_TO_INCLUDE)

set(RESOURCE_FILES collabvm-qt-client.qrc)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

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

target_link_libraries(${PROJECT_NAME} Qt6::Widgets)

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