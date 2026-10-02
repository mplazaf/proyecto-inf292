#ifndef GENERADOR_H
#define GENERADOR_H
#include <random>
#include <string>
#include <vector>
#include "Instancia.h"
using namespace std;

struct ConfigCategoria {
    string categoria;
    int minimoElementos;
    int maximoElementos;
};


Instancia generarInstancia(const string& tipo,int semilla);

void generarDimensiones(Instancia& instancia,mt19937& gen);

double calcularUmin(const Instancia& instancia);

double generarUmax(const Instancia& instancia,mt19937& gen);

vector<string> elegirCategorias(const string& tipo,mt19937& gen);

void generarElementos(Instancia& instancia,mt19937& gen);

void asignarFijosYOptimizables(Instancia& instancia,mt19937& gen);

int generarCantidadAlternativas(const string& tipo,mt19937& gen);

vector<Alternativa> generarCatalogo(const string& categoria,const string& tipoInstancia,mt19937& gen); 

double generarU(const string& categoria,mt19937& gen);

double generarCosto(const string& categoria,double U,mt19937& gen);

void generarElementoFijo(Elemento& elemento, mt19937& gen);

int generarCantidadElementos(const ConfigCategoria& config, mt19937& gen);

void generarElementoOptimizable(Elemento& elemento,const vector<Alternativa>& catalogo);

void asignarAreas(Instancia& instancia,mt19937& gen);

#endif
