# CMakeList.txt : CMake project for ecs, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

file(GLOB SRC
     "**/*.h"
     "**/*.cpp"
)

project (ecslib)

# Add source to this project's executable.
add_library (ecslib STATIC ${SRC})

# TODO: Add tests and install targets if needed.
