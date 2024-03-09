# CMakeList.txt : CMake project for Tech, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Tech "Tech.cpp" "Tech.h"  "test/IntervalMapTest.h" "test/IntervalMapTest.cpp" "test/Tests.h" "utils/IntervalMap.h" "utils/Utils.h" "test/Utils.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Tech PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
