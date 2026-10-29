CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
AR = ar rcs

LIB_DIR = lib
LIB_TARGET = $(LIB_DIR)/libentidades.a
ENTIDADES_SRCS = src/Persona.cpp src/Medico.cpp src/Paciente.cpp src/Turno.cpp
ENTIDADES_OBJS = $(LIB_DIR)/Persona.o $(LIB_DIR)/Medico.o $(LIB_DIR)/Paciente.o $(LIB_DIR)/Turno.o

MAIN_SRCS = src/Validaciones.cpp src/ClinicaServicio.cpp src/InterfazUI.cpp src/main.cpp
TARGET = sistema_turnos.exe

all: $(TARGET)

$(LIB_DIR):
	mkdir $(LIB_DIR)

$(LIB_DIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIB_TARGET): $(ENTIDADES_OBJS)
	$(AR) $@ $^

$(TARGET): $(LIB_TARGET) $(MAIN_SRCS)
	$(CXX) $(CXXFLAGS) $(MAIN_SRCS) -L$(LIB_DIR) -lentidades -o $(TARGET)

clean:
	rm -f $(LIB_DIR)/*.o $(LIB_TARGET) $(TARGET)
