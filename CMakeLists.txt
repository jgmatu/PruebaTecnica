cmake_minimum_required(VERSION 3.10)
project(Actia VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Define the Library (Your logic/modules)
# List your sources but EXCLUDE main.cpp
file(GLOB LIB_SOURCES 
    "src/rng/*.cpp"
    "src/filter/*.cpp"
    "src/logger/*.cpp"
)

add_library(ActiaLib STATIC ${LIB_SOURCES})

# 2. Set Header Scopes
target_include_directories(ActiaLib 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 3. Define the Executable (The "User" of the library)
add_executable(ActiaApp src/main.cpp)

# 4. Link the Library to the App
# ActiaApp will inherit PUBLIC headers (include/), but NOT PRIVATE headers (src/)
target_link_libraries(ActiaApp PRIVATE ActiaLib)

# Optional: Threads
find_package(Threads REQUIRED)
target_link_libraries(ActiaLib PRIVATE Threads::Threads)
