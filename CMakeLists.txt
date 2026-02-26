cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto y versión
project(Actia VERSION 1.0 LANGUAGES CXX)

# Forzar el estándar C++ 17 o 20 (recomendado para RHEL 10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Carpeta donde se guardarán los archivos compilados (opcional)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_options(-g)

# Buscar archivos fuente automáticamente (o lístalos manualmente)
file(GLOB SOURCES 
    "src/rng/*.cpp"
    "src/filter/*.cpp"
    "src/logger/*.cpp"
    "src/*.cpp" 
    "main.cpp"
)

# Crear el ejecutable
add_executable(${PROJECT_NAME} ${SOURCES})

# Directorios de inclusión (headers)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Ejemplo: Si necesitas añadir librerías externas (ej. Threads)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

# Mensaje de confirmación al configurar
message(STATUS "Proyecto configurado para: ${PROJECT_NAME}")
