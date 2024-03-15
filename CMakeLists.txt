# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable multiprocessor builds
if(MSVC)
 target_compile_options(<target> PRIVATE "/MP")
endif()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("technical-assessment")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")

	# MyToolchain.cmake
	include("../vcpkg/scripts/buildsystems/vcpkg.cmake")
	set(VCPKG_TARGET_TRIPLET x64-my-custom-windows-triplet)

	# Boost
	set(BOOST_ROOT "vcpkg_installed/x64-windows")

endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")


endif() 

# Boost
set(Boost_USE_STATIC_LIBS       OFF)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and 
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs 
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

set(Boost_INCLUDE_DIR "${BOOST_ROOT}/include")
set(Boost_LIBRARY_DIR "${BOOST_ROOT}/lib")

include_directories("${Boost_INCLUDE_DIR}")

find_package(Boost REQUIRED COMPONENTS 
	system
	filesystem
	serialization ) 

# Include sub-projects.
add_subdirectory ("Tech")

