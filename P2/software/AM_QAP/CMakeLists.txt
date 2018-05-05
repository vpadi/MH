######################################################################################
### 0) Set your application properties
######################################################################################
# check cmake version compatibility
cmake_minimum_required(VERSION 2.8)

set (CMAKE_CXX_STANDARD 11)

# Here define your project name
project(QAP)

# Here define the name and the version of your package
SET(PACKAGE_NAME "QAP" CACHE STRING "QAP" FORCE)
SET(PACKAGE_VERSION "1.0" CACHE STRING "1.0" FORCE)

# set a language for the entire project.
enable_language(CXX)
enable_language(C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/paradiseoFinder/module")

find_package(Paradiseo)

include_directories(${PARADISEO_INCLUDE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

######################################################################################

######################################################################################
### 2) include the sources
######################################################################################
add_executable(QAP_V1 ${CMAKE_SOURCE_DIR}/src/main_Memetico_QAP.cpp)
add_executable(QAP_V2 ${CMAKE_SOURCE_DIR}/src/main_Memetico_QAP_version2.cpp)
add_executable(QAP_V3 ${CMAKE_SOURCE_DIR}/src/main_Memetico_QAP_version3.cpp)

######################################
### include required modules & utilities
#####################################################################################
include(CMakeBackwardCompatibilityCXX)

include(FindDoxygen)

include(CheckLibraryExists)


######################################################################################

#####################################################################################
### Manage the build type
#####################################################################################

# the user should choose the build type on windows environments,excepted under cygwin (default=none)
set(CMAKE_DEFAULT_BUILD_TYPE "Release" CACHE STRING "Variable that stores the default CMake build type" FORCE)

find_program(MEMORYCHECK_COMMAND
    NAMES purify valgrind
    PATHS
    "/usr/local/bin /usr/bin [HKEY_LOCAL_MACHINE\\SOFTWARE\\Rational Software\\Purify\\Setup;InstallFolder]"
    DOC "Path to the memory checking command, used for memory error detection.")


if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DCMAKE_VERBOSE_MAKEFILE=ON)
endif(CMAKE_BUILD_TYPE MATCHES Debug)

add_definitions(-DDEPRECATED_MESSAGES)

######################################################################################
### 3) Link the libraries for your executable
######################################################################################

target_link_libraries(QAP_V1 ${PARADISEO_LIBRARIES})
target_link_libraries(QAP_V2 ${PARADISEO_LIBRARIES})
target_link_libraries(QAP_V3 ${PARADISEO_LIBRARIES})

######################################################################################
