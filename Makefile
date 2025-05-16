# Nombre del compilador de C++
CXX = g++

# Flags para el compilador de C++:
# -std=c++17: Especifica el estándar de C++ (puedes cambiarlo a c++11, c++14, c++20, etc.)
# -Wall: Activa la mayoría de las advertencias del compilador.
# -Wextra: Activa advertencias adicionales.
# -g: Genera información de depuración (para usar con gdb).
# -O2: Nivel de optimización (puedes usar -O0 para sin optimización, -O3 para más optimización).
# -MMD -MP: Genera archivos de dependencia (.d) para rastrear cambios en los encabezados.
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O2 -MMD -MP

# Flags para el enlazador (linker)
# LDFLAGS se usa para enlazar bibliotecas, por ejemplo: -lm para la biblioteca matemática.
LDFLAGS =

# Nombre del ejecutable final
TARGET = B_Galopante

# Archivos fuente (.cpp)
# En este caso, solo uno. Si tuvieras más, los listarías aquí: SRC = main.cpp utils.cpp etc.
SOURCES = B_Galopante.cpp

# Archivos objeto (.o) - se generan a partir de los archivos fuente
# Esto toma la lista en SOURCES y reemplaza la extensión .cpp por .o
OBJECTS = $(SOURCES:.cpp=.o)

# Archivos de dependencia (.d) - generados por las flags -MMD -MP
# Estos archivos ayudan a make a saber cuándo recompilar si un encabezado cambia.
DEPS = $(OBJECTS:.o=.d)

# Target por defecto: se ejecuta cuando solo escribes "make" en la terminal.
# Depende del ejecutable final.
all: $(TARGET)

# Regla para crear el ejecutable final a partir de los archivos objeto.
# $(OBJECTS) son los prerrequisitos.
# $@ es una variable automática que representa el nombre del target (en este caso, $(TARGET)).
# $^ es una variable automática que representa todos los prerrequisitos.
$(TARGET): $(OBJECTS)
	@echo "Enlazando para crear $(TARGET)..."
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Ejecutable $(TARGET) creado."

# Regla genérica para compilar archivos .cpp en archivos .o.
# %.o: Un target que coincide con cualquier archivo .o.
# %.cpp: Un prerrequisito que coincide con un archivo .cpp con el mismo nombre base.
# $< es una variable automática que representa el primer prerrequisito (el archivo .cpp).
%.o: %.cpp
	@echo "Compilando $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Incluye los archivos de dependencia generados.
# El '-' al inicio significa que make no se quejará ni se detendrá si los archivos .d no existen
# (lo cual es normal la primera vez que compilas o después de un 'make clean').
-include $(DEPS)

# Target para limpiar los archivos generados (ejecutable, objetos, dependencias).
clean:
	@echo "Limpiando archivos generados..."
	rm -f $(TARGET) $(OBJECTS) $(DEPS)
	@echo "Limpieza completa."

# Declara targets que no representan archivos reales.
# Esto evita conflictos si tienes un archivo llamado "all" o "clean" en tu directorio.
.PHONY: all clean