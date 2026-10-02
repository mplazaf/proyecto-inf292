#include <bits/stdc++.h>
#include "validacion.h"

using namespace std;


bool validarInstancia(const Instancia& instancia) {

    if (instancia.tipo != "pequena" &&
        instancia.tipo != "mediana" &&
        instancia.tipo != "grande") {

        return false;
    }

// Validar dimensiones
    
    if (instancia.largo <= 0 ||
        instancia.ancho <= 0 ||
        instancia.altura <= 0 ||
        instancia.superficie <= 0 ||
        instancia.volumen <= 0 ||
        instancia.Umax <= 0) {

        return false;
    }
    
    // Validar categorías
    
    set<string> categorias;

    for (const Elemento& elemento : instancia.elementos) {

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

    // Validar cantidad de elementos optimizables

    int cantidadOptimizables = 0;

    for (const Elemento& elemento :
         instancia.elementos) {

        if (!elemento.fijo) {

            cantidadOptimizables++;
        }
    }


    if (instancia.tipo == "pequena") {

        if (cantidadOptimizables < 2 ||
            cantidadOptimizables > 4) {

            return false;
        }
    }

    else if (instancia.tipo == "mediana") {

        if (cantidadOptimizables < 5 ||
            cantidadOptimizables > 8) {

            return false;
        }
    }

    else if (instancia.tipo == "grande") {

        if (cantidadOptimizables < 9 ||
            cantidadOptimizables > 15) {

            return false;
        }
    }

    // Validar los elementos

    for (const Elemento& elemento :
         instancia.elementos) {

        // Elemento fijo

        if (elemento.fijo) {

            if (elemento.Ufijo <= 0 ||
                elemento.costoFijoM2 <= 0) {

                return false;
            }
        }

        // Elemento optimizable

        else {

            int cantidadAlternativas =
                elemento.alternativas.size();

            // Cantidad de alternativas segun tipo

            if (instancia.tipo == "pequena") {

                if (cantidadAlternativas < 5 ||
                    cantidadAlternativas > 10) {

                    return false;
                }
            }

            else if (instancia.tipo == "mediana") {

                if (cantidadAlternativas < 11 ||
                    cantidadAlternativas > 25) {

                    return false;
                }
            }

            else if (instancia.tipo == "grande") {

                if (cantidadAlternativas < 26 ||
                    cantidadAlternativas > 50) {

                    return false;
                }
            }

            // Validar u > 0 y costo

            for (const Alternativa& alternativa :
                 elemento.alternativas) {

                if (alternativa.U <= 0 ||
                    alternativa.costoM2 <= 0) {

                    return false;
                }
            }

            // Validar relacion U - costo
            // menor U -> mayor costo

            for (int i = 0;
                 i < (int)elemento.alternativas.size();
                 i++) {

                for (int j = i + 1;
                     j < (int)elemento.alternativas.size();
                     j++) {

                    const Alternativa& a =
                        elemento.alternativas[i];

                    const Alternativa& b =
                        elemento.alternativas[j];


                    if (a.U < b.U &&
                        a.costoM2 <= b.costoM2) {

                        return false;
                    }


                    if (b.U < a.U &&
                        b.costoM2 <= a.costoM2) {

                        return false;
                    }
                }
            }
        }
    }

    // Validar factibilidad térmica 

    double sumaTermica = 0.0;
    double areaTotal = 0.0;


    for (const Elemento& elemento :
         instancia.elementos) {

        areaTotal +=
            elemento.area;


        // Elemento fijo
        if (elemento.fijo) {

            sumaTermica +=
                elemento.area *
                elemento.Ufijo;
        }


        // Elemento optimizable
        else {

            if (elemento.alternativas.empty()) {

                return false;
            }


            double menorU =
                elemento.alternativas[0].U;


            for (const Alternativa& alternativa :
                 elemento.alternativas) {

                menorU =
                    min(
                        menorU,
                        alternativa.U
                    );
            }


            sumaTermica +=
                elemento.area *
                menorU;
        }
    }


    if (areaTotal <= 0) {

        return false;
    }


    double uMinFactible =
        sumaTermica /
        areaTotal;


    if (uMinFactible >
        instancia.Umax) {

        return false;
    }

    return true;
}
