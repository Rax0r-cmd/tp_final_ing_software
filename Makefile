CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRCS = src/Persona.cpp \
       src/Medico.cpp \
       src/Paciente.cpp \
       src/Turno.cpp \
       src/Validaciones.cpp \
       src/ClinicaServicio.cpp \
       src/InterfazUI.cpp \
       src/main.cpp

TARGET = sistema_turnos.exe

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o

