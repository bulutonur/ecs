# CMakeList.txt : CMake project for ecs, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


project(test)
include_directories("../ecs")


file(GLOB SRC
     "*.h"
     "*.cpp"
)


# Add source to this project's executable.
add_executable (test ${SRC})
target_link_libraries(test ecslib)

