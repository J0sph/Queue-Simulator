# Definición del compilador y las banderas
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Directorios
COLA_DIR = Colas
GUI_DIR = Interfaz
BUILD_DIR = build

# Compilar y ejecutar los codigos
prueba: $(COLA_DIR)/UsoColas.cpp
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$@ $<
	$(BUILD_DIR)/prueba.exe


