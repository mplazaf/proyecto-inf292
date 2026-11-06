#include <bits/stdc++.h>
#include "validacion.h"

using namespace std;


bool validarInstancia(const Instancia& instancia) {
    if (instancia.tipo != "pequena" &&
        instancia.tipo != "mediana" &&
        instancia.tipo != "grande") {
        return false;
    }

    // Validar dimensiones y rangos
    if (instancia.largo < 8.0 ||
        instancia.largo > 15.0 ||
        instancia.ancho < 6.0 ||
        instancia.ancho > 12.0 ||
        instancia.altura < 2.3 ||
        instancia.altura > 2.8 ||
        instancia.superficie <= 0 ||
        instancia.volumen <= 0 ||
        instancia.Umax <= 0) {
        return false;
    }

    // Validar coherencia geometrica
    const double tolerancia = 1e-6;
    if (std::abs(instancia.superficie -
                 instancia.largo * instancia.ancho) > tolerancia ||
        std::abs(instancia.volumen -
                 instancia.superficie * instancia.altura) > tolerancia) {

        return false;
    }

    // Validar categorías
    set<string> categorias;
    for (const Elemento& elemento : instancia.elementos) {
        categorias.insert(elemento.categoria);
        if (elemento.area <= 0) {
            return false;
        }
    }
    int cantidadCategorias = categorias.size();
    if (instancia.tipo == "pequena") {
        if (cantidadCategorias < 3 || cantidadCategorias > 4) {
            return false;
        }
    }

    else if (instancia.tipo == "mediana" || instancia.tipo == "grande") {
        if (cantidadCategorias != 5) {
            return false;
        }
    }

    // Validar cantidad de elementos optimizables
    int cantidadOptimizables = 0;
    for (const Elemento& elemento : instancia.elementos) {
        if (!elemento.fijo) {
            cantidadOptimizables++;
        }
    }

    if (instancia.tipo == "pequena") {
        if (cantidadOptimizables < 2 || cantidadOptimizables > 4) {
            return false;
        }
    }
    else if (instancia.tipo == "mediana") {
        if (cantidadOptimizables < 5 || cantidadOptimizables > 8) {
            return false;
        }
    }
    else if (instancia.tipo == "grande") {
        if (cantidadOptimizables < 9 || cantidadOptimizables > 15) {
            return false;
        }
    }


    // Validar rangos de transmitancia segun categoria
    auto transmitanciaValida = [](const string& categoria, double U) {
        if (!isfinite(U)) return false;
        if (categoria == "Muro") return U >= 0.3 && U <= 1.5;
        if (categoria == "Techo") return U >= 0.2 && U <= 1.2;
        if (categoria == "Piso") return U >= 0.3 && U <= 1.4;
        if (categoria == "Ventana") return U >= 1.0 && U <= 5.5;
        if (categoria == "Puerta") return U >= 1.0 && U <= 3.5;
        return false;
    };

    // Validar costo segun categoria y transmitancia
    auto costoValido = [](const string& categoria, double U, double costo) {
        if (!isfinite(U) || !isfinite(costo) || U <= 0 || costo <= 0) {
            return false;
        }
        double base = 0.0;
        double factor = 0.0;
        if (categoria == "Muro") {
            base = 10000;
            factor = 12000;
        }
        else if (categoria == "Techo") {
            base = 12000;
            factor = 10000;
        }
        else if (categoria == "Piso") {
            base = 9000;
            factor = 11000;
        }
        else if (categoria == "Ventana") {
            base = 50000;
            factor = 70000;
        }
        else if (categoria == "Puerta") {
            base = 30000;
            factor = 40000;
        }
        else {
            return false;
        }
        double costoEsperado = base + factor / U;
        return abs(costo - costoEsperado) <= 1e-8 * max(1.0, abs(costoEsperado));
    };

    // Validar los elementos
    for (const Elemento& elemento : instancia.elementos) {
        if (elemento.fijo) {
            if (!transmitanciaValida(elemento.categoria, elemento.Ufijo) || !costoValido(elemento.categoria, elemento.Ufijo, elemento.costoFijoM2)) {
                return false;
            }
        }
        else {
            int cantidadAlternativas = elemento.alternativas.size();
            if (instancia.tipo == "pequena") {
                if (cantidadAlternativas < 5 || cantidadAlternativas > 10) {
                    return false;
                }
            }
            else if (instancia.tipo == "mediana") {
                if (cantidadAlternativas < 11 || cantidadAlternativas > 25) {
                    return false;
                }
            }
            else if (instancia.tipo == "grande") {
                if (cantidadAlternativas < 26 || cantidadAlternativas > 50) {
                    return false;
                }
            }
            // Validar u > 0 y costo
            for (const Alternativa& alternativa : elemento.alternativas) {
                if (!transmitanciaValida(elemento.categoria, alternativa.U) || !costoValido(elemento.categoria, alternativa.U, alternativa.costoM2)) {
                    return false;
                }
            }
            // Validar relacion U - costo
            for (int i = 0; i < (int)elemento.alternativas.size(); i++) {
                for (int j = i + 1; j < (int)elemento.alternativas.size(); j++) {
                    const Alternativa& a = elemento.alternativas[i];
                    const Alternativa& b = elemento.alternativas[j];
                    if (a.U < b.U && a.costoM2 <= b.costoM2) {
                        return false;
                    }
                    if (b.U < a.U && b.costoM2 <= a.costoM2) {
                        return false;
                    }
                }
            }
        }
    }

    // Validar factibilidad térmica
    double sumaTermica = 0.0;
    double areaTotal = 0.0;
    for (const Elemento& elemento : instancia.elementos) {
        areaTotal += elemento.area;
        if (elemento.fijo) {
            sumaTermica += elemento.area * elemento.Ufijo;
        }
        else {
            if (elemento.alternativas.empty()) {
                return false;
            }
            double menorU = elemento.alternativas[0].U;
            for (const Alternativa& alternativa : elemento.alternativas) {
                menorU = min(menorU, alternativa.U);
            }
            sumaTermica += elemento.area * menorU;
        }
    }
    if (areaTotal <= 0) {
        return false;
    }
    double uMinFactible = sumaTermica / areaTotal;
    if (uMinFactible > instancia.Umax) {
        return false;
    }
    return true;
}