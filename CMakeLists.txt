# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.28)

# CXX COMPILER 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

# Enable multiprocessor builds
if(MSVC)
 target_compile_options(<target> PRIVATE "/MP")
endif()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("technical-assessment" VERSION 0.1.0)

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

#set(Boost_INCLUDE_DIR "${BOOST_ROOT}/include")
#set(Boost_LIBRARY_DIR "${BOOST_ROOT}/lib")

include_directories("${Boost_INCLUDE_DIR}")

find_package(Boost REQUIRED COMPONENTS 
	system
	filesystem
	serialization 
	unit_test_framework ) 

# Include Unit Tests
add_executable(Tests.IntervalMap "Tech/test/IntervalMapTest.cpp")

add_executable(Tests.Arithmetics "Tech/test/ArithmeticsTest.cpp")

target_link_libraries(Tests.IntervalMap Boost::unit_test_framework)
target_link_libraries(Tests.Arithmetics Boost::unit_test_framework)

enable_testing()

add_test(NAME Tests.IntervalMap COMMAND Tests.IntervalMap)
add_test(NAME Tests.Arithmetics COMMAND Tests.Arithmetics)

# Include sub-projects.
add_subdirectory ("Tech")

