#include <bits/stdc++.h>
#include "Instancia.h"

using namespace std;

bool validarInstancia(const Instancia& instancia) {

    // Dimensiones validas
    if (instancia.largo <= 0 ||
        instancia.ancho <= 0 ||
        instancia.altura <= 0 ||
        instancia.superficie <= 0 ||
        instancia.volumen <= 0 ||
        instancia.Umax <= 0) {
        return false;
    }

    // Categorias
    set<string> categorias;

    for (const Elemento& elemento :
         instancia.elementos) {

        categorias.insert(
            elemento.categoria
        );

        if (elemento.area <= 0) {
            return false;
        }
    }

    int cantidadCategorias =
        categorias.size();

    if (instancia.tipo == "pequena") {

        if (cantidadCategorias < 3 ||
            cantidadCategorias > 4) {
            return false;
        }
    }

    else if (instancia.tipo == "mediana" ||
             instancia.tipo == "grande") {

        if (cantidadCategorias != 5) {
            return false;
        }
    }

    // Cantidad de optimizables
    int optimizables = 0;

    for (const Elemento& elemento :
         instancia.elementos) {

        if (!elemento.fijo) {
            optimizables++;
        }
    }

    if (instancia.tipo == "pequena") {

        if (optimizables < 2 ||
            optimizables > 4) {
            return false;
        }
    }

    else if (instancia.tipo == "mediana") {

        if (optimizables < 5 ||
            optimizables > 8) {
            return false;
        }
    }

    else if (instancia.tipo == "grande") {

        if (optimizables < 9 ||
            optimizables > 15) {
            return false;
        }
    }

    // Validar elementos
    for (const Elemento& elemento :
         instancia.elementos) {

        if (elemento.fijo) {

            if (elemento.Ufijo <= 0 ||
                elemento.costoFijoM2 <= 0) {
                return false;
            }
        }

        else {

            int cantidadAlt =
                elemento.alternativas.size();

            if (instancia.tipo == "pequena") {

                if (cantidadAlt < 5 ||
                    cantidadAlt > 10) {
                    return false;
                }
            }

            else if (instancia.tipo == "mediana") {

                if (cantidadAlt < 11 ||
                    cantidadAlt > 25) {
                    return false;
                }
            }

            else if (instancia.tipo == "grande") {

                if (cantidadAlt < 26 ||
                    cantidadAlt > 50) {
                    return false;
                }
            }

            // Positividad
            for (const Alternativa& alt :
                 elemento.alternativas) {

                if (alt.U <= 0 ||
                    alt.costo <= 0) {
                    return false;
                }
            }

            // Relacion U-costo
            for (int i = 0;
                 i < elemento.alternativas.size();
                 i++) {

                for (int j = i + 1;
                     j < elemento.alternativas.size();
                     j++) {

                    const Alternativa& a =
                        elemento.alternativas[i];

                    const Alternativa& b =
                        elemento.alternativas[j];

                    if (a.U < b.U &&
                        a.costo <= b.costo) {
                        return false;
                    }

                    if (b.U < a.U &&
                        b.costo <= a.costo) {
                        return false;
                    }
                }
            }
        }
    }

    // Factibilidad termica
    double sumaTermica = 0.0;
    double areaTotal = 0.0;

    for (const Elemento& elemento :
         instancia.elementos) {

        areaTotal += elemento.area;

        if (elemento.fijo) {

            sumaTermica +=
                elemento.area *
                elemento.Ufijo;
        }

        else {

            double menorU =
                elemento.alternativas[0].U;

            for (const Alternativa& alt :
                 elemento.alternativas) {

                menorU =
                    min(
                        menorU,
                        alt.U
                    );
            }

            sumaTermica +=
                elemento.area *
                menorU;
        }
    }

    double uMinFactible =
        sumaTermica / areaTotal;

    if (uMinFactible >
        instancia.Umax) {
        return false;
    }

    return true;
}
