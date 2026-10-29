cmake_minimum_required(VERSION 3.15)
project(sistema_turnos LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# 1. Cada entidad como libreria (Opcional cumplido)
add_library(entidades STATIC
    src/Persona.cpp
    src/Medico.cpp
    src/Paciente.cpp
    src/Turno.cpp
)

# 2. Programa principal
add_executable(sistema_turnos
    src/Validaciones.cpp
    src/ClinicaServicio.cpp
    src/InterfazUI.cpp
    src/main.cpp
)

# 3. Vinculacion de la libreria de entidades al ejecutable
target_link_libraries(sistema_turnos PRIVATE entidades)
