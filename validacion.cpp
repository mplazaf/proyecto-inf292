#include <bits/stdc++.h>
#include "validacion.h"

using namespace std;


bool validarInstancia(const Instancia& instancia) {

    // ========================================================
    // VALIDAR TIPO DE INSTANCIA
    // ========================================================

    if (instancia.tipo != "pequena" &&
        instancia.tipo != "mediana" &&
        instancia.tipo != "grande") {

        return false;
    }


    // ========================================================
    // VALIDAR DIMENSIONES GENERALES
    // ========================================================

    if (instancia.largo <= 0 ||
        instancia.ancho <= 0 ||
        instancia.altura <= 0 ||
        instancia.superficie <= 0 ||
        instancia.volumen <= 0 ||
        instancia.Umax <= 0) {

        return false;
    }


    // ========================================================
    // VALIDAR CATEGORIAS Y AREAS
    // ========================================================

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


    // ========================================================
    // VALIDAR CANTIDAD DE ELEMENTOS OPTIMIZABLES
    // ========================================================

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


    // ========================================================
    // VALIDAR ELEMENTOS
    // ========================================================

    for (const Elemento& elemento :
         instancia.elementos) {


        // ----------------------------------------------------
        // ELEMENTO FIJO
        // ----------------------------------------------------

        if (elemento.fijo) {

            if (elemento.Ufijo <= 0 ||
                elemento.costoFijoM2 <= 0) {

                return false;
            }
        }


        // ----------------------------------------------------
        // ELEMENTO OPTIMIZABLE
        // ----------------------------------------------------

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


            // ------------------------------------------------
            // VALIDAR POSITIVIDAD DE U Y COSTO
            // ------------------------------------------------

            for (const Alternativa& alternativa :
                 elemento.alternativas) {

                if (alternativa.U <= 0 ||
                    alternativa.costoM2 <= 0) {

                    return false;
                }
            }


            // ------------------------------------------------
            // VALIDAR RELACION U - COSTO
            //
            // menor U -> mayor costo
            // ------------------------------------------------

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


    // ========================================================
    // VALIDAR FACTIBILIDAD TERMICA
    // ========================================================

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


    // ========================================================
    // TODAS LAS VALIDACIONES FUERON SUPERADAS
    // ========================================================

    return true;
}
