cmake_minimum_required(VERSION 3.10)
project(DeepfakeDetector-DS-II-i)

# Set OpenCV directory explicitly to your MinGW-compatible build
set(OpenCV_DIR "C:/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/x64/mingw/lib")

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include directories from OpenCV
include_directories(${OpenCV_INCLUDE_DIRS})

# Create an executable
file(GLOB SOURCES "src/*.cpp")
add_executable(DeepfakeDetector-DS-II-i ${SOURCES})


# Link OpenCV libraries
target_link_libraries(DeepfakeDetector-DS-II-i ${OpenCV_LIBS})