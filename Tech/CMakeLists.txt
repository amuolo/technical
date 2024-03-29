# CMakeList.txt : CMake project for Tech, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Tech "Tech.cpp" "../vcpkg.json" "../vcpkg-configuration.json"      
"utils/AlgorithmComplexity.hpp" 
"utils/Arithmetics.hpp" 
"utils/Calculator.hpp"
"utils/Concepts.hpp"
"utils/InputOutput.hpp"  
"utils/IntervalMap.hpp" 
"utils/RandomMachine.hpp"  
"utils/Resources.hpp"  
"utils/StandardLibs.hpp"  
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Tech PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
