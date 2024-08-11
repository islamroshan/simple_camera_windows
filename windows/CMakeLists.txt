# The Flutter tooling requires that developers have a version of Visual Studio
# installed that includes CMake 3.14 or later. You should not increase this
# version, as doing so will cause the plugin to fail to compile for some
# customers of the plugin.
cmake_minimum_required(VERSION 3.14)

# Project-level configuration.
set(PROJECT_NAME "simple_camera_windows")
project(${PROJECT_NAME} LANGUAGES CXX)

# Explicitly opt in to modern CMake behaviors to avoid warnings with recent
# versions of CMake.
cmake_policy(VERSION 3.14...3.25)

# This value is used when generating builds using this plugin, so it must
# not be changed
set(PLUGIN_NAME "simple_camera_windows_plugin")

# Any new source files that you add to the plugin should be added here.
list(APPEND PLUGIN_SOURCES
  "simple_camera_windows_plugin.cpp"
  "simple_camera_windows_plugin.h"
)

# Define the plugin library target. Its name must not be changed (see comment
# on PLUGIN_NAME above).
add_library(${PLUGIN_NAME} SHARED
  "include/simple_camera_windows/simple_camera_windows_plugin_c_api.h"
  "simple_camera_windows_plugin_c_api.cpp"
  ${PLUGIN_SOURCES}
)


# Apply a standard set of build settings that are configured in the
# application-level CMakeLists.txt. This can be removed for plugins that want
# full control over build settings.
apply_standard_settings(${PLUGIN_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4127 /WX-")

# Symbols are hidden by default to reduce the chance of accidental conflicts
# between plugins. This should not be removed; any symbols that should be
# exported should be explicitly exported with the FLUTTER_PLUGIN_EXPORT macro.
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
 
# Set OpenCV directory path to the copied build folder
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/build")

# Source include directories and library dependencies. Add any plugin-specific
# dependencies here.
target_include_directories(${PLUGIN_NAME} PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${OpenCV_DIR}/include"
)

# Link the appropriate OpenCV library based on the build configuration
target_link_libraries(${PLUGIN_NAME} PRIVATE
	flutter 
	flutter_wrapper_plugin
	"$<IF:$<CONFIG:Debug>,${OpenCV_DIR}/x64/vc17/lib/opencv_core4100d.lib,${OpenCV_DIR}/x64/vc17/lib/opencv_core4100.lib>"
    	"$<IF:$<CONFIG:Debug>,${OpenCV_DIR}/x64/vc17/lib/opencv_highgui4100d.lib,${OpenCV_DIR}/x64/vc17/lib/opencv_highgui4100.lib>"
    	"$<IF:$<CONFIG:Debug>,${OpenCV_DIR}/x64/vc17/lib/opencv_imgcodecs4100d.lib,${OpenCV_DIR}/x64/vc17/lib/opencv_imgcodecs4100.lib>"
    	"$<IF:$<CONFIG:Debug>,${OpenCV_DIR}/x64/vc17/lib/opencv_videoio4100d.lib,${OpenCV_DIR}/x64/vc17/lib/opencv_videoio4100.lib>"
)
 
# List of absolute paths to libraries that should be bundled with the plugin.
# This list could contain prebuilt libraries, or libraries created by an
# external build triggered from this build file.
set(simple_camera_windows_bundled_libraries
  ""
  PARENT_SCOPE
)

# Ensure the Debug and Release directories exist before copying the DLL
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/runner/Debug")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/runner/Release")

# Custom command to copy OpenCV DLL to Debug directory
add_custom_command(TARGET ${PLUGIN_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OpenCV_DIR}/x64/vc17/bin/opencv_core4100d.dll"
        "${CMAKE_BINARY_DIR}/runner/Debug/opencv_core4100d.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OpenCV_DIR}/x64/vc17/bin/opencv_highgui4100d.dll"
        "${CMAKE_BINARY_DIR}/runner/Debug/opencv_highgui4100d.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OpenCV_DIR}/x64/vc17/bin/opencv_imgcodecs4100d.dll"
        "${CMAKE_BINARY_DIR}/runner/Debug/opencv_imgcodecs4100d.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OpenCV_DIR}/x64/vc17/bin/opencv_imgproc4100d.dll"
        "${CMAKE_BINARY_DIR}/runner/Debug/opencv_imgproc4100d.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OpenCV_DIR}/x64/vc17/bin/opencv_videoio4100d.dll"
        "${CMAKE_BINARY_DIR}/runner/Debug/opencv_videoio4100d.dll"
)

# Custom command to copy OpenCV DLL to Release directory
add_custom_command(TARGET ${PLUGIN_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OpenCV_DIR}/x64/vc17/bin/opencv_core4100.dll"
        "${CMAKE_BINARY_DIR}/runner/Release/opencv_core4100.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OpenCV_DIR}/x64/vc17/bin/opencv_highgui4100.dll"
        "${CMAKE_BINARY_DIR}/runner/Release/opencv_highgui4100.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OpenCV_DIR}/x64/vc17/bin/opencv_imgcodecs4100.dll"
        "${CMAKE_BINARY_DIR}/runner/Release/opencv_imgcodecs4100.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OpenCV_DIR}/x64/vc17/bin/opencv_imgproc4100.dll"
        "${CMAKE_BINARY_DIR}/runner/Release/opencv_imgproc4100.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OpenCV_DIR}/x64/vc17/bin/opencv_videoio4100.dll"
        "${CMAKE_BINARY_DIR}/runner/Release/opencv_videoio4100.dll"
)
 
# === Tests ===
# These unit tests can be run from a terminal after building the example, or
# from Visual Studio after opening the generated solution file.

# Only enable test builds when building the example (which sets this variable)
# so that plugin clients aren't building the tests.
if (${include_${PROJECT_NAME}_testss})
set(TEST_RUNNER "${PROJECT_NAME}_test")
enable_testing()

# Add the Google Test dependency.
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.11.0.zip
)
# Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# Disable install commands for gtest so it doesn't end up in the bundle.
set(INSTALL_GTEST OFF CACHE BOOL "Disable installation of googletest" FORCE)
FetchContent_MakeAvailable(googletest)

# The plugin's C API is not very useful for unit testing, so build the sources
# directly into the test binary rather than using the DLL.
add_executable(${TEST_RUNNER}
  test/simple_camera_windows_plugin_test.cpp
  ${PLUGIN_SOURCES}
)
apply_standard_settings(${TEST_RUNNER})
# Set OpenCV directory path to the copied build folder
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/build")
target_include_directories(${TEST_RUNNER} PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${OpenCV_DIR}/include"
)
target_link_libraries(${TEST_RUNNER} PRIVATE flutter_wrapper_plugin)
target_link_libraries(${TEST_RUNNER} PRIVATE gtest_main gmock)

# Source include directories and library dependencies. Add any plugin-specific
# dependencies here.
target_link_libraries(${TEST_RUNNER} PRIVATE
    "$<IF:$<CONFIG:Debug>,${OpenCV_DIR}/x64/vc17/lib/opencv_core4100d.lib,${OpenCV_DIR}/x64/vc17/lib/opencv_core4100.lib>"
    "$<IF:$<CONFIG:Debug>,${OpenCV_DIR}/x64/vc17/lib/opencv_highgui4100d.lib,${OpenCV_DIR}/x64/vc17/lib/opencv_highgui4100.lib>"
    "$<IF:$<CONFIG:Debug>,${OpenCV_DIR}/x64/vc17/lib/opencv_imgcodecs4100d.lib,${OpenCV_DIR}/x64/vc17/lib/opencv_imgcodecs4100.lib>"
    "$<IF:$<CONFIG:Debug>,${OpenCV_DIR}/x64/vc17/lib/opencv_videoio4100d.lib,${OpenCV_DIR}/x64/vc17/lib/opencv_videoio4100.lib>"
)
message(STATUS "Flutter Library Path: ${FLUTTER_LIBRARY}")

if(EXISTS "${FLUTTER_LIBRARY}")
    add_custom_command(TARGET ${TEST_RUNNER} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${FLUTTER_LIBRARY}" $<TARGET_FILE_DIR:${TEST_RUNNER}>
    )
    message(STATUS "Flutter library copied successfully.")
else()
    message(FATAL_ERROR "Flutter library not found at ${FLUTTER_LIBRARY}")
endif()

# Enable automatic test discovery.
include(GoogleTest)
gtest_discover_tests(${TEST_RUNNER})
endif()
