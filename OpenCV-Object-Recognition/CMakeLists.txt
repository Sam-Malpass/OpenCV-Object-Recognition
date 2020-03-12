# CMakeList.txt : CMake project for OpenCV-Object-Recognition, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories("Source Files", "Header Files")

# Add source to this project's executable.
add_executable (Run "Source Files/OpenCV-Object-Recognition.cpp" "Header Files/OpenCV-Object-Recognition.h")

# TODO: Add tests and install targets if needed.
