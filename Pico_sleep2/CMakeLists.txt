# Version minimum de CMake
cmake_minimum_required(VERSION 3.12)

# Importation du SDK
# A faire avant d'appeler project()
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_EXTRAS_PATH}/external/pico_extras_import.cmake)

# Nom du projet
project(base)

# Initialisation SDK
pico_sdk_init()

# sources
set(SOURCE_FILES base.c)

# binaire et sources
add_executable(${CMAKE_PROJECT_NAME}
    ${SOURCE_FILES}
)

# Ajout pico_stdlib pour les fonctions courantes
target_link_libraries(${CMAKE_PROJECT_NAME} pico_stdlib hardware_sleep)

# Création des fichiers dis/bin/hex/uf2
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})

# Ajout cible upload avec picotool
add_custom_target(upload
    COMMAND picotool load -v -x ${CMAKE_PROJECT_NAME}.bin
)

# "make clean" doit aussi supprimer map/dis/bin/hex/uf2
set_directory_properties(PROPERTIES
   ADDITIONAL_MAKE_CLEAN_FILES
   "${CMAKE_PROJECT_NAME}.bin;${CMAKE_PROJECT_NAME}.dis;\
${CMAKE_PROJECT_NAME}.elf.map;${CMAKE_PROJECT_NAME}.hex;\
${CMAKE_PROJECT_NAME}.uf2"
)

