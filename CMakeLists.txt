cmake_minimum_required(VERSION 3.10)
project(DeepfakeDetector-DS-II-i)

# Set OpenCV directory explicitly to your MinGW-compatible build
set(OpenCV_DIR "C:/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/x64/mingw/lib")

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include)
include_directories(utils)

# Define source files
set(SOURCES
    src/main.cpp
    src/hash_generator.cpp
    src/hash_database.cpp
    src/image_processor.cpp
    src/detector.cpp
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create an executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link OpenCV libraries
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

# Print OpenCV version information during configuration (helpful for debugging)
message(STATUS "OpenCV Version: ${OpenCV_VERSION}")
message(STATUS "OpenCV Libraries: ${OpenCV_LIBS}")
message(STATUS "OpenCV Include Dirs: ${OpenCV_INCLUDE_DIRS}")