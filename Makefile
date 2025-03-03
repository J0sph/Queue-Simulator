# Definición del compilador y las banderas
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./Estructuras/
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system 

# Directorios
ESTRUCT_DIR = Estructuras
BUILD_DIR = build

# Crear el directorio build si no existe
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)  

# Regla para compilar y ejecutar 'main'
main: main.cpp $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$@ $< $(LDFLAGS)
	@echo "Ejecutando $@..."
	@$(BUILD_DIR)/$@


# Regla para compilar y ejecutar 'pruebaQueue'
pruebaQueue: $(ESTRUCT_DIR)/pruebaQueue.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$@ $^  
	@echo "Ejecutando $@..."
	@$(BUILD_DIR)/$@

# Regla para compilar y ejecutar 'pruebaVIPQueue'
pruebaVIPQueue: $(ESTRUCT_DIR)/pruebaVIPQueue.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$@ $^  
	@echo "Ejecutando $@..."
	@$(BUILD_DIR)/$@