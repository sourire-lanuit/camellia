cmake_minimum_required(VERSION 3.10)
project(CamelliaCipher CXX)

# Встановлюємо стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Збираємо всі файли ядра алгоритму в одну змінну
set(CAMELLIA_SOURCES
    decrypt_block.cpp
    encrypt_block.cpp
    generate_round_keys.cpp
    sboxes.cpp
)

# Створюємо статичну бібліотеку з основним алгоритмом
add_library(camellia_core STATIC ${CAMELLIA_SOURCES})

# ---------------------------------------------------
# Виконуваний файл 1: Основна програма (CLI)
# ---------------------------------------------------
add_executable(camellia main.cpp)
target_link_libraries(camellia camellia_core)

# ---------------------------------------------------
# Виконуваний файл 2: Юніт-тести
# ---------------------------------------------------
add_executable(camellia_tests tests.cpp)
target_link_libraries(camellia_tests camellia_core)