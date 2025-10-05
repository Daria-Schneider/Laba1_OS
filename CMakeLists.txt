cmake_minimum_required(VERSION 3.10)
project(Lab1 VERSION 1.0)

# Установка путей
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Директории для заголовков
include_directories(include)

# Общая библиотека
add_library(crossplatform STATIC 
    src/crossplatform.c 
    src/stringutils.c
)

# Исполняемые файлы
add_executable(parent src/parent.c)
add_executable(child src/child.c)

# Линковка
target_link_libraries(parent crossplatform)
target_link_libraries(child crossplatform)

# Для Windows
if(WIN32)
    target_link_libraries(parent kernel32)
    target_link_libraries(child kernel32)
endif()

# Установка выходных директорий
set_target_properties(parent child crossplatform
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)