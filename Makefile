CPPFLAGS=-std=c++11

all: prueba

prueba : main.o MSvector.o Objeto.o Vector.o String.o Pivote.o Bloque.o Nodo.o Cargador.o Consultor.o Lector.o
	g++-5 $(CPPFLAGS) main.o Objeto.o MSvector.o Vector.o String.o Pivote.o Bloque.o Nodo.o Cargador.o Consultor.o Lector.o -o prueba

main.o: main.cpp
	g++-5 $(CPPFLAGS) -c main.cpp

MSvector.o: MSvector.cpp
	g++-5 $(CPPFLAGS) -c MSvector.cpp

Objeto.o: Objeto.cpp
	g++-5 $(CPPFLAGS) -c Objeto.cpp

Vector.o: Vector.cpp
	g++-5 $(CPPFLAGS) -c Vector.cpp

String.o: String.cpp
	g++-5 $(CPPFLAGS) -c String.cpp

Pivote.o: Pivote.cpp
	g++-5 $(CPPFLAGS) -c Pivote.cpp

Bloque.o: Bloque.cpp
	g++-5 $(CPPFLAGS) -c Bloque.cpp

Nodo.o: Nodo.cpp
	g++-5 $(CPPFLAGS) -c Nodo.cpp

Cargador.o: Cargador.cpp
	g++-5 $(CPPFLAGS) -c Cargador.cpp

Consultor.o: Consultor.cpp
	g++-5 $(CPPFLAGS) -c Consultor.cpp

Lector.o: Lector.cpp
	g++-5 $(CPPFLAGS) -c Lector.cpp

clean:
	rm -rf *o prueba

