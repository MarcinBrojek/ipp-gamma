cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0037 OLD)
project(Gamma C)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif ()

# Nie chcemy oglądać komunikatów o procentowym postępie kompilacji.
set_property(GLOBAL PROPERTY RULE_MESSAGES OFF)

# Chcemy zobaczyć polecenia wywołane przez make.
set(CMAKE_VERBOSE_MAKEFILE ON)

# Ustawiamy wspólne opcje kompilowania dla wszystkich wariantów projektu.
set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra")
# Domyślne opcje dla wariantów Release i Debug są sensowne.
# Jeśli to konieczne, ustawiamy tu inne.
# set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
# set(CMAKE_C_FLAGS_DEBUG "-g")

# Wskazujemy pliki źródłowe.
set(SOURCE_FILES
    src/gamma_structure.c
    src/gamma_structure.h
    src/gamma_helpers.c
    src/gamma_helpers.h
    src/gamma.c
    src/gamma.h
    src/gamma_command.c
    src/gamma_command.h
    src/gamma_parser.c
    src/gamma_parser.h
    src/gamma_printer.c
    src/gamma_printer.h
    src/gamma_batch.c
    src/gamma_batch.h
    src/gamma_interactive.c
    src/gamma_interactive.h
    src/gamma_main.c)

# Wskazujemy pliki testowe.
set(TEST_SOURCE_FILES
    src/gamma_structure.c
    src/gamma_structure.h
    src/gamma_helpers.c
    src/gamma_helpers.h
    src/gamma.c
    src/gamma.h
    src/gamma_test.c)

# Wskazujemy plik wykonywalny.
add_executable(gamma ${SOURCE_FILES})

# Wskazujemy plik wykonywalny dla testów silnika.
add_executable(test EXCLUDE_FROM_ALL ${TEST_SOURCE_FILES})
set_target_properties(test PROPERTIES OUTPUT_NAME gamma_test)

# Włączenie getline()
add_definitions(-D_GNU_SOURCE)

# Dodajemy obsługę Doxygena: sprawdzamy, czy jest zainstalowany i jeśli tak to:
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # Wskazujemy lokalizacją pliku konfiguracyjnego i podajemy jego docelową lokalizację w folderze, gdzie następuje kompilacja.
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    # Dodajemy cel doc: użycie make doc spowoduje wywołanie doxygena z odpowiednim plikiem konfiguracyjnym w folderze kompilacji.
    # Na wyjście zostanie wypisany odpowiedni komentarz.
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
    )
endif (DOXYGEN_FOUND)
