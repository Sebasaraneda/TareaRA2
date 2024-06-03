# Variables
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Archivos
SERVER = server.cpp
CLIENT = cliente.cpp

# Establecer el prefijo de receta en un espacio en blanco
.RECIPEPREFIX = 

all: server cliente

server: $(SERVER)
	$(CXX) $(CXXFLAGS) -o server $(SERVER) -lpthread

cliente: $(CLIENT)
	$(CXX) $(CXXFLAGS) -o cliente $(CLIENT)

clean:
	rm -f server cliente
