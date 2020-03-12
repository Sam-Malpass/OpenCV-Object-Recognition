# CMakeList.txt : CMake project for OpenCV-Object-Recognition, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories("Source Files", "Header Files")

# Add source to this project's executable.
add_executable (Run "Source Files/OpenCV-Object-Recognition.cpp" "Source Files/Preprocessor.cpp" "Source Files/FileHandler.cpp" "Header Files/OpenCV-Object-Recognition.h" "Header Files/Structures.h" "Header Files/ConfusionMatrix.h" "Header Files/Preprocessor.h" "Header Files/FileHandler.h")

# TODO: Add tests and install targets if needed.
find_package(OpenCV REQUIRED)
 
target_link_libraries(Run ${OpenCV_LIBS})