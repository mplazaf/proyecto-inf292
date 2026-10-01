all:
	g++ Alternativa.h Elemento.h generador.cpp generador.h Instancia.h validacion.cpp main.cpp -o gen
clear:
	rm gen
