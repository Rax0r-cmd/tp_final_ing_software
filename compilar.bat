@echo off
echo =======================================================================
echo Compilando Libreria de Entidades y Sistema de Turnos en C++...
echo =======================================================================

if not exist lib mkdir lib

echo [1/3] Compilando entidades (Persona, Medico, Paciente, Turno)...
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/Persona.cpp -o lib/Persona.o
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/Medico.cpp -o lib/Medico.o
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/Paciente.cpp -o lib/Paciente.o
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/Turno.cpp -o lib/Turno.o

echo [2/3] Generando libreria estatica lib/libentidades.a...
ar rcs lib/libentidades.a lib/Persona.o lib/Medico.o lib/Paciente.o lib/Turno.o

echo [3/3] Compilando aplicacion principal y vinculando libreria de entidades...
g++ -std=c++17 -Wall -Wextra -Iinclude ^
    src/Validaciones.cpp ^
    src/ClinicaServicio.cpp ^
    src/InterfazUI.cpp ^
    src/main.cpp ^
    -Llib -lentidades ^
    -o sistema_turnos.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Compilacion exitosa!
    echo Libreria generada: lib/libentidades.a
    echo Ejecutable generado: sistema_turnos.exe
    echo.
    echo Iniciando sistema...
    echo.
    sistema_turnos.exe
) else (
    echo.
    echo Ocurrio un error durante la compilacion.
)
pause
