#include <bits/stdc++.h>
#include "Instancia.h"
#include "generador.h"
using namespace std;

vector<ConfigCategoria> configuracionBase = {
    {"Muro", 4, 4},
    {"Techo", 1, 1},
    {"Piso", 1, 1},
    {"Ventana", 1, 4},
    {"Puerta", 1, 2}
};


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

double calcularUmin(const Instancia& instancia) {
    double numerador = 0.0;
    double areaTotal = 0.0;
    for (const Elemento& elemento : instancia.elementos) {
        areaTotal += elemento.area;
        if (elemento.fijo) {
            numerador += elemento.area * elemento.Ufijo;
        }
        else {
            if (elemento.alternativas.empty()) {
                continue;
            }
            double menorU = elemento.alternativas[0].U;

            for (const Alternativa& alternativa :
                 elemento.alternativas) {
                if (alternativa.U < menorU) {
                    menorU = alternativa.U;
                }
            }
            numerador += elemento.area * menorU;
        }
    }
    if (areaTotal == 0.0) {
        return 0.0;
    }
    return numerador / areaTotal;
}

double generarUmax(const Instancia& instancia,mt19937& gen) {
    double Umin = calcularUmin(instancia);
    uniform_real_distribution<double> distMargen(0.05,0.30);
    double margen = distMargen(gen);
    return Umin + margen;
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
        elemento.alternativas.push_back(alternativa);
    }
}

void generarElementoFijo(Elemento& elemento,mt19937& gen) {
    elemento.fijo = true;
    double U = generarU(elemento.categoria,gen);
    double costo = generarCosto(elemento.categoria,U,gen);
    elemento.Ufijo = U;
    elemento.costoFijoM2 = costo;
    elemento.alternativas.clear();
}

void generarElementoOptimizable(Elemento& elemento,const string& tipoInstancia,mt19937& gen) {
    elemento.fijo = false;
    elemento.Ufijo = 0.0;
    elemento.costoFijoM2 = 0.0;
    generarCatalogo(elemento,tipoInstancia,gen);
}

int generarCantidadElementos(const ConfigCategoria& config,mt19937& gen) {
    uniform_int_distribution<int> dist(config.minimoElementos,config.maximoElementos);
    return dist(gen);
}

void generarElementos(Instancia& instancia,mt19937& gen) {
    vector<ConfigCategoria> configuracion = {
        {"Muro", 4, 4},
        {"Techo", 1, 1},
        {"Piso", 1, 1},
        {"Ventana", 1, 4},
        {"Puerta", 1, 2}
    };

    vector<string> categoriasSeleccionadas = elegirCategorias(instancia.tipo, gen);

    for (const ConfigCategoria& config : configuracion) {
        if (
            find(categoriasSeleccionadas.begin(),categoriasSeleccionadas.end(),config.categoria) == categoriasSeleccionadas.end()) {
            continue;
        }

        int cantidad = generarCantidadElementos(config,gen);
        // MUROS
        if (config.categoria == "Muro") {
            vector<string> nombres = {
                "Muro Norte",
                "Muro Sur",
                "Muro Este",
                "Muro Oeste"
            };

            for (int i = 0; i < cantidad; i++) {
                Elemento muro(nombres[i],"Muro",0.0,false);
                instancia.elementos.push_back(muro);
            }
        }

        // TECHO
        else if (config.categoria == "Techo") {
            for (int i = 1; i <= cantidad; i++) {
                Elemento techo("Techo","Techo",0.0,false);
                instancia.elementos.push_back(techo);
            }
        }

        // PISO
        else if (config.categoria == "Piso") {
            for (int i = 1; i <= cantidad; i++) {
                Elemento piso("Piso","Piso",0.0,false);
                instancia.elementos.push_back(piso);
            }
        }

        // VENTANAS
        else if (config.categoria == "Ventana") {

            for (int i = 1; i <= cantidad; i++) {
                Elemento ventana("Ventana " + to_string(i),"Ventana",0.0,false);
                instancia.elementos.push_back(ventana);
            }
        }
        // PUERTAS
        else if (config.categoria == "Puerta") {
            for (int i = 1; i <= cantidad; i++) {
              Elemento puerta("Puerta " + to_string(i),"Puerta",0.0,false);
              instancia.elementos.push_back(puerta);
            }
        }
    }
}

void asignarFijosYOptimizables(Instancia& instancia,mt19937& gen) {
    int minOptimizables;
    int maxOptimizables;

    if (instancia.tipo == "pequena") {
        minOptimizables = 2;
        maxOptimizables = 4;
    }
    else if (instancia.tipo == "mediana") {
        minOptimizables = 5;
        maxOptimizables = 8;
    }
    else if (instancia.tipo == "grande") {
        minOptimizables = 9;
        maxOptimizables = 15;
    }
    else {
        return;
    }

    int cantidadElementos = instancia.elementos.size();

    // Evitar pedir más optimizables que elementos existentes
    maxOptimizables = min(maxOptimizables, cantidadElementos);

    minOptimizables = min(minOptimizables, maxOptimizables);

    uniform_int_distribution<int> distCantidad(minOptimizables,maxOptimizables);

    int cantidadOptimizables = distCantidad(gen);

    // Primero dejar todos como fijos
    for (Elemento& elemento : instancia.elementos) {
        elemento.fijo = true;
    }
    // Índices 0,1,2,...,n-1
    vector<int> indices(cantidadElementos);

    for (int i = 0; i < cantidadElementos; i++) {
        indices[i] = i;
    }
    // Mezclar los índices
    shuffle(indices.begin(),indices.end(),gen);

    // Los primeros N serán optimizables
    for (int i = 0; i < cantidadOptimizables; i++) {
        instancia.elementos[indices[i]].fijo = false;
    }
}


void asignarAreas(Instancia& instancia,mt19937& gen) {
    // Áreas base de los muros
    double areaMuroNorteSur = instancia.ancho * instancia.altura;
    double areaMuroEsteOeste =instancia.largo * instancia.altura;

    // Para repartir ventanas entre muros
    uniform_real_distribution<double> distPorcentajeVentanas(0.10,0.25);

    double porcentajeVentanas = distPorcentajeVentanas(gen);

    double areaMurosTotal = 2.0 * areaMuroNorteSur + 2.0 * areaMuroEsteOeste;

    double areaVentanasTotal = areaMurosTotal * porcentajeVentanas;

    int cantidadVentanas = 0;

    for (const Elemento& elemento : instancia.elementos) {
        if (elemento.categoria == "Ventana") {
            cantidadVentanas++;
        }
    }
    // Área promedio por ventana
    double areaPorVentana = 0.0;

    if (cantidadVentanas > 0) {
        areaPorVentana = areaVentanasTotal / cantidadVentanas;
    }

    // Puertas: rango de área por puerta
    uniform_real_distribution<double> distAreaPuerta(1.5,3.0);

    for (Elemento& elemento : instancia.elementos) {
        // PISO
        if (elemento.categoria == "Piso") {
            elemento.area = instancia.superficie;
        }
        // TECHO
        else if (elemento.categoria == "Techo") {
            // techo = superficie de planta
            elemento.area = instancia.superficie;
        }

        // MUROS
        else if (elemento.categoria == "Muro") {

            if (elemento.nombre == "Muro Norte" || elemento.nombre == "Muro Sur") {
                elemento.area = areaMuroNorteSur;
            }
            else if (elemento.nombre == "Muro Este" ||elemento.nombre == "Muro Oeste") {
                elemento.area = areaMuroEsteOeste;
            }
        }
        // VENTANAS
        else if (elemento.categoria == "Ventana") {
            elemento.area = areaPorVentana;
        }
        // PUERTAS
        else if (elemento.categoria == "Puerta") {
            elemento.area =
                distAreaPuerta(gen);
        }
    }
}

Instancia generarInstancia(const string& tipo,int semilla) {
    mt19937 gen(semilla);
    Instancia instancia(
        tipo,
        semilla,
        0.0,   // largo
        0.0,   // ancho
        0.0,   // altura
        0.0    // Umax
    );

    generarDimensiones(instancia, gen);
    generarElementos(instancia, gen);
    asignarAreas(instancia, gen);
    asignarFijosYOptimizables(instancia, gen);

    for (Elemento& elemento : instancia.elementos) {
        if (elemento.fijo) {
            generarElementoFijo(elemento,gen);
        } else {
            generarElementoOptimizable(elemento,instancia.tipo,gen);
        }
    }
    instancia.Umax =generarUmax(instancia,gen);
    return instancia;
}
