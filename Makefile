# Definición del compilador y las banderas
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Directorios
COLA_DIR = Colas
GUI_DIR = Interfaz
ESTRUCT_DIR = Estructuras
BUILD_DIR = build

# Crear el directorio build si no existe
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)  

# Compilar y ejecutar los codigos
prueba: $(COLA_DIR)/UsoColas.cpp
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$@ $<
	$(BUILD_DIR)/prueba.exe

main: $(ESTRUCT_DIR)/main.cpp $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$@ $<  
	@echo "Ejecutando $@..."
	@$(BUILD_DIR)/$@



