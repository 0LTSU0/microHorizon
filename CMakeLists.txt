cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 20)

project(microHorizon)

#None of this shit works, would be nice to figure out eventually
#option(BUILD_BOOST_FROM_SOURCE "Build Boost from source TODO: THIS NEEDS TO BE ON ATM" ON)
#set(BOOST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/boost-1.86.0")
#message(${BOOST_SOURCE_DIR})
#include(ExternalProject)
#if (WIN32)
#    # For Windows, use `bootstrap.bat` and `b2.exe`
#    ExternalProject_Add(
#        Boost
#        SOURCE_DIR ${BOOST_SOURCE_DIR}
#        CONFIGURE_COMMAND ${BOOST_SOURCE_DIR}/bootstrap.bat
#        BUILD_COMMAND ${BOOST_SOURCE_DIR}/b2.exe --prefix=${CMAKE_BINARY_DIR}/boost_install
#        INSTALL_COMMAND ${BOOST_SOURCE_DIR}/b2.exe install --prefix=${CMAKE_BINARY_DIR}/boost_install
#	)
#else()
#    # For Unix-like systems, use `bootstrap.sh` and `b2`
#    ExternalProject_Add(
#        Boost
#        SOURCE_DIR ${BOOST_SOURCE_DIR}
#        CONFIGURE_COMMAND ${BOOST_SOURCE_DIR}/bootstrap.sh --prefix=${CMAKE_BINARY_DIR}/boost_install
#        BUILD_COMMAND ${BOOST_SOURCE_DIR}/b2
#        INSTALL_COMMAND ${BOOST_SOURCE_DIR}/b2 install --prefix=${CMAKE_BINARY_DIR}/boost_install
#	)
#endif()
#set(Boost_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/boost_install/include)
#set(Boost_LIBRARY_DIRS ${CMAKE_BINARY_DIR}/boost_install/lib)
#add_dependencies(microHorizon Boost)

#If we're on linux, check if arm to enable some compiler options to prevent compiler from crashing on raspiZero
if (NOT WIN32)
	EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
	message( STATUS "Architecture: ${ARCHITECTURE}" )
	if (${ARCHITECTURE} STREQUAL "aarch64" OR ${ARCHITECTURE} STREQUAL "arm")
		set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Default build type form ARM: Release" FORCE)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1 -g -fno-inline -fno-ipa-cp-clone")
	endif()
endif()

#Note to self, to build/install from source on windows.
#1. Run bootstrap.sh or bootstrap.bat in boost src dir
#2. Run b2.exe or b2 created by step 1
#3. Run b2.exe/b2 install --prefix=<pathto/install/dir> (cant be "install" btw because there is a file called INSTALL)
#Or for linux just do: sudo apt install libboost-all-dev (probably works if version in apt happens to be somewhere around 1.8x)
if (WIN32)
set(BOOST_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/boost-1.86.0/installation" CACHE PATH "Path to Boost install dir")
message(STATUS "Trying to find boost installation from: " ${BOOST_INSTALL_DIR})
set(BOOST_ROOT ${BOOST_INSTALL_DIR})
#else()
#set(BOOST_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/boost-1.86.0/installation_linux64" CACHE PATH "Path to Boost install dir")
#message(STATUS "Trying to find boost installation from: " ${BOOST_INSTALL_DIR})
#set(BOOST_ROOT ${BOOST_INSTALL_DIR})
endif()
find_package(Boost REQUIRED COMPONENTS chrono thread)
if(NOT Boost_FOUND)
	message(FATAL_ERROR "Boost libraries not found. Please install them, I can't get them to be built as part of the project :(")
else()
	message("boost vars" ${Boost_LIBRARIES} ${Boost_INCLUDE_DIRS})
endif()

add_subdirectory(ThirdParty)
add_subdirectory(src)
