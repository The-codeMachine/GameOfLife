cmake_minimum_required(VERSION 3.16)
project(GameOfLife VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the executable and list all .cpp files in src/
add_executable(${PROJECT_NAME}
    src/main.cpp
)

# Add the include/ directory for header lookups
target_include_directories(${PROJECT_NAME} PRIVATE include)
