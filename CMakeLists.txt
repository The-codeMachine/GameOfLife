cmake_minimum_required(VERSION 3.16)
project(GameOfLife VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set the output directory for built objects
# this makes sure that the dynamic library goes into the build directory automatically (SDL3)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Define the executable and list all .cpp files in src/
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/World.cpp
    src/utils/positionConversions.cpp
    src/rendering/engine.cpp
)

# Add the include/ directory for header lookups
target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)