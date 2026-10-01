#include <bits/stdc++.h>
#include "Instancia.h"
#include "generador.h"
using namespace std;

void generarDimensiones(Instancia &instancia, mt19937 &gen){
    uniform_real_distribution<double> largoDist(8.0, 15.0);
    uniform_real_distribution<double> anchoDist(6.0, 12.0);
    uniform_real_distribution<double> alturaDist(2.3, 2.8);

    instancia.largo = largoDist(gen);
    instancia.ancho = anchoDist(gen);
    instancia.altura = alturaDist(gen);
    instancia.superficie =instancia.largo * instancia.ancho;
    instancia.volumen = instancia.superficie * instancia.altura;
}

double generarUmax(mt19937& gen) {
    uniform_real_distribution<double> dist(0.5, 1.2);
    return dist(gen);
}

vector<string> elegirCategorias(const string& tipo,mt19937& gen) {
    vector<string> categorias = {"Muro","Techo","Piso","Ventana","Puerta"};
    if (tipo == "pequena") {
        shuffle(categorias.begin(), categorias.end(), gen);
        uniform_int_distribution<int> distCantidad(3, 4);
        int cantidad = distCantidad(gen);
        categorias.resize(cantidad);
    }
    return categorias;
}

int generarCantidadAlternativas(const string& tipo,mt19937& gen) {
    if (tipo == "pequena") {
        uniform_int_distribution<int> dist(5, 10);
        return dist(gen);
    }
    else if (tipo == "mediana") {
        uniform_int_distribution<int> dist(11, 25);
        return dist(gen);
    }
    else if (tipo == "grande") {
        uniform_int_distribution<int> dist(26, 50);
        return dist(gen);
    }
    return 0;
}
//LOS RANGOS PUEDEN SER CAMBIADOS
double generarU(const string& categoria, mt19937& gen){
    if (categoria == "Muro") {
        uniform_real_distribution<double> dist(0.3, 1.5);
        return dist(gen);
    }
    else if (categoria == "Techo") {
        uniform_real_distribution<double> dist(0.2, 1.2);
        return dist(gen);
    }
    else if (categoria == "Piso") {
        uniform_real_distribution<double> dist(0.3, 1.4);
        return dist(gen);
    }
    else if (categoria == "Ventana") {
        uniform_real_distribution<double> dist(1.0, 5.5);
        return dist(gen);
    }
    else if (categoria == "Puerta") {
        uniform_real_distribution<double> dist(1.0, 3.5);
        return dist(gen);
    }
    return 0.0;
}
// LOS PRECIOS PUEDEN SER CAMBIADOS
double generarCosto(const string& categoria,double U,mt19937& gen){
    double costoBase;
    double factor;
    if (categoria == "Muro") {
        costoBase = 10000;
        factor = 12000;
    }
    else if (categoria == "Techo") {
        costoBase = 12000;
        factor = 10000;
    }
    else if (categoria == "Piso") {
        costoBase = 9000;
        factor = 11000;
    }
    else if (categoria == "Ventana") {
        costoBase = 50000;
        factor = 70000;
    }
    else if (categoria == "Puerta") {
        costoBase = 30000;
        factor = 40000;
    }
    else {
        return 0.0;
    }
    uniform_real_distribution<double> ruido(0.95, 1.05);
    double costo = costoBase + factor / U;
    return costo * ruido(gen);
}

void generarCatalogo(Elemento& elemento,const string& tipoInstancia,mt19937& gen) {

    int cantidad = generarCantidadAlternativas(tipoInstancia,gen);

    for (int i = 0; i < cantidad; i++) {
        double U =generarU(elemento.categoria,gen);
        double costo = generarCosto(elemento.categoria,U,gen);
        Alternativa alternativa("A" + to_string(i + 1),U,costo);
        elemento.alternativas.push_back(alternativa
        );
    }
}
