@echo off
echo ========================================================
echo Compilando Sistema de Gestion de Turnos en C++...
echo ========================================================

g++ -std=c++17 -Wall -Wextra -Iinclude ^
    src/Persona.cpp ^
    src/Medico.cpp ^
    src/Paciente.cpp ^
    src/Turno.cpp ^
    src/Validaciones.cpp ^
    src/ClinicaServicio.cpp ^
    src/InterfazUI.cpp ^
    src/main.cpp ^
    -o sistema_turnos.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Compilacion exitosa! Ejecutable generado: sistema_turnos.exe
    echo Ejecutando el programa...
    echo.
    sistema_turnos.exe
) else (
    echo.
    echo Ocurrio un error durante la compilacion.
)
pause

