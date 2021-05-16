# Set CMake minimum version and CMake policy required by UrhoCommon module
cmake_minimum_required (VERSION 3.2.3)
if (COMMAND cmake_policy)
    # Libraries linked via full path no longer produce linker search paths
    cmake_policy (SET CMP0003 NEW)
    # INTERFACE_LINK_LIBRARIES defines the link interface
    cmake_policy (SET CMP0022 NEW)
    # Disallow use of the LOCATION target property - so we set to OLD as we still need it
    cmake_policy (SET CMP0026 OLD)
    # MACOSX_RPATH is enabled by default
    cmake_policy (SET CMP0042 NEW)
    # Honor the visibility properties for SHARED target types only
    cmake_policy (SET CMP0063 OLD)
endif ()
# Define target name
set (TARGET_NAME urho3d-obs-plugin)
# Set project name
project (${TARGET_NAME})

# Set CMake modules search path
#set (URHO3D_HOME /home/scaled/Urho3D-1.7.1)
#set (CMAKE_SOURCE_DIR ${URHO3D_HOME})
#set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

#set (CMAKE_MODULE_PATH ${URHO3D_HOME}/CMake/Modules)

set (URHO3D_HOME /usr/local/share/Urho3D)
set (CMAKE_MODULE_PATH ${URHO3D_HOME}/CMake/Modules)

# Include UrhoCommon.cmake module after setting project name
include (UrhoCommon)
# Define source files
define_source_files ()
#define_resource_dirs (GLOB_PATTERNS ${URHO3D_HOME}/Data ${URHO3D_HOME}/bin/CoreData)
define_resource_dirs (GLOB_PATTERNS ${CMAKE_SOURCE_DIR}/Data ${CMAKE_SOURCE_DIR}/CoreData)
# Setup target with resource copying
#setup_main_executable ()
setup_library (MODULE)

# Link to OpenCV
find_package(LibObs REQUIRED)
target_link_libraries(${TARGET_NAME} pthread ${LIBOBS_LIBRARIES})
