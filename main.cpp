#include <bits/stdc++.h>
#include "Instancia.h"
#include "generador.h"
#include "validacion.h"

using namespace std;


// ============================================================
// GUARDAR INSTANCIA EN ARCHIVO
// ============================================================

void guardarInstancia(
    const Instancia& casa,
    int numeroInstancia
) {

    string nombreArchivo =
        "instancia_"
        + casa.tipo
        + "_"
        + to_string(numeroInstancia)
        + ".txt";

    ofstream archivo(nombreArchivo);

    if (!archivo.is_open()) {

        cout << "No se pudo crear el archivo "
             << nombreArchivo
             << endl;

        return;
    }

    archivo << "==============================" << endl;
    archivo << "DATOS DE LA INSTANCIA" << endl;
    archivo << "==============================" << endl;

    archivo << "Tipo: "
            << casa.tipo
            << endl;

    archivo << "Semilla: "
            << casa.semilla
            << endl;

    archivo << "\nDIMENSIONES" << endl;

    archivo << "Largo: "
            << casa.largo
            << " m"
            << endl;

    archivo << "Ancho: "
            << casa.ancho
            << " m"
            << endl;

    archivo << "Altura: "
            << casa.altura
            << " m"
            << endl;

    archivo << "Superficie: "
            << casa.superficie
            << " m2"
            << endl;

    archivo << "Volumen: "
            << casa.volumen
            << " m3"
            << endl;

    archivo << "Umax: "
            << casa.Umax
            << endl;

    archivo << "Cantidad total de elementos: "
            << casa.elementos.size()
            << endl;


    // ========================================================
    // ELEMENTOS
    // ========================================================

    archivo << "\n==============================" << endl;
    archivo << "ELEMENTOS" << endl;
    archivo << "==============================" << endl;

    int cantidadFijos = 0;
    int cantidadOptimizables = 0;

    set<string> categorias;


    for (const Elemento& elemento : casa.elementos) {

        categorias.insert(
            elemento.categoria
        );

        archivo << "\n------------------------------" << endl;

        archivo << "Nombre: "
                << elemento.nombre
                << endl;

        archivo << "Categoria: "
                << elemento.categoria
                << endl;

        archivo << "Area: "
                << elemento.area
                << " m2"
                << endl;


        // ELEMENTO FIJO
        if (elemento.fijo) {

            cantidadFijos++;

            archivo << "Tipo: FIJO" << endl;

            archivo << "U fijo: "
                    << elemento.Ufijo
                    << endl;

            archivo << "Costo fijo/m2: $"
                    << elemento.costoFijoM2
                    << endl;
        }


        // ELEMENTO OPTIMIZABLE
        else {

            cantidadOptimizables++;

            archivo << "Tipo: OPTIMIZABLE"
                    << endl;

            archivo << "Cantidad de alternativas: "
                    << elemento.alternativas.size()
                    << endl;

            archivo << "\nCatalogo:" << endl;

            for (const Alternativa& alternativa :
                 elemento.alternativas) {

                archivo
                    << "  "
                    << alternativa.nombre
                    << " | U: "
                    << alternativa.U
                    << " | Costo/m2: $"
                    << alternativa.costoM2
                    << endl;
            }
        }
    }


    // ========================================================
    // RESUMEN
    // ========================================================

    archivo << "\n==============================" << endl;
    archivo << "RESUMEN" << endl;
    archivo << "==============================" << endl;

    archivo << "Categorias: "
            << categorias.size()
            << endl;

    archivo << "Elementos totales: "
            << casa.elementos.size()
            << endl;

    archivo << "Elementos fijos: "
            << cantidadFijos
            << endl;

    archivo << "Elementos optimizables: "
            << cantidadOptimizables
            << endl;

    archivo << "Umax: "
            << casa.Umax
            << endl;

    archivo << "Superficie: "
            << casa.superficie
            << " m2"
            << endl;

    archivo.close();

    cout << "Archivo guardado: "
         << nombreArchivo
         << endl;
}


// ============================================================
// MAIN
// ============================================================

int main() {

    vector<string> tipos = {
        "pequena",
        "mediana",
        "grande"
    };

    int semillaBase = 100;
    int numeroInstancia = 1;

    int cantidadValidas = 0;
    int cantidadInvalidas = 0;


    for (const string& tipo : tipos) {

        for (int i = 0; i < 5; i++) {

            int semilla =
                semillaBase
                + numeroInstancia;


            // ====================================================
            // GENERAR INSTANCIA
            // ====================================================

            Instancia casa =
                generarInstancia(
                    tipo,
                    semilla
                );


            cout << "\n\n========================================" << endl;
            cout << "INSTANCIA " << numeroInstancia << endl;
            cout << "========================================" << endl;

            cout << "Tipo: "
                 << casa.tipo
                 << endl;

            cout << "Semilla: "
                 << casa.semilla
                 << endl;


            // ====================================================
            // VALIDACION
            // ====================================================

            if (!validarInstancia(casa)) {

                cout << "Estado: INSTANCIA INVALIDA" << endl;

                cantidadInvalidas++;

                numeroInstancia++;

                continue;
            }


            cout << "Estado: INSTANCIA VALIDA" << endl;

            cantidadValidas++;


            // ====================================================
            // DATOS GENERALES
            // ====================================================

            cout << "\n==============================" << endl;
            cout << "DATOS DE LA INSTANCIA" << endl;
            cout << "==============================" << endl;

            cout << "Tipo: "
                 << casa.tipo
                 << endl;

            cout << "Semilla: "
                 << casa.semilla
                 << endl;


            cout << "\nDIMENSIONES" << endl;

            cout << "Largo: "
                 << casa.largo
                 << " m"
                 << endl;

            cout << "Ancho: "
                 << casa.ancho
                 << " m"
                 << endl;

            cout << "Altura: "
                 << casa.altura
                 << " m"
                 << endl;

            cout << "Superficie: "
                 << casa.superficie
                 << " m2"
                 << endl;

            cout << "Volumen: "
                 << casa.volumen
                 << " m3"
                 << endl;

            cout << "\nUmax: "
                 << casa.Umax
                 << endl;

            cout << "\nCantidad total de elementos: "
                 << casa.elementos.size()
                 << endl;


            // ====================================================
            // ELEMENTOS
            // ====================================================

            cout << "\n==============================" << endl;
            cout << "ELEMENTOS" << endl;
            cout << "==============================" << endl;

            int cantidadFijos = 0;
            int cantidadOptimizables = 0;

            set<string> categorias;


            for (const Elemento& elemento :
                 casa.elementos) {

                categorias.insert(
                    elemento.categoria
                );

                cout << "\n------------------------------" << endl;

                cout << "Nombre: "
                     << elemento.nombre
                     << endl;

                cout << "Categoria: "
                     << elemento.categoria
                     << endl;

                cout << "Area: "
                     << elemento.area
                     << " m2"
                     << endl;


                // ELEMENTO FIJO
                if (elemento.fijo) {

                    cantidadFijos++;

                    cout << "Tipo: FIJO" << endl;

                    cout << "U fijo: "
                         << elemento.Ufijo
                         << endl;

                    cout << "Costo fijo/m2: $"
                         << elemento.costoFijoM2
                         << endl;
                }


                // ELEMENTO OPTIMIZABLE
                else {

                    cantidadOptimizables++;

                    cout << "Tipo: OPTIMIZABLE" << endl;

                    cout << "Cantidad de alternativas: "
                         << elemento.alternativas.size()
                         << endl;

                    cout << "\nCatalogo:" << endl;


                    for (const Alternativa& alternativa :
                         elemento.alternativas) {

                        cout << "  "
                             << alternativa.nombre
                             << " | U: "
                             << alternativa.U
                             << " | Costo/m2: $"
                             << alternativa.costoM2
                             << endl;
                    }
                }
            }


            // ====================================================
            // RESUMEN
            // ====================================================

            cout << "\n==============================" << endl;
            cout << "RESUMEN" << endl;
            cout << "==============================" << endl;

            cout << "Categorias: "
                 << categorias.size()
                 << endl;

            cout << "Elementos totales: "
                 << casa.elementos.size()
                 << endl;

            cout << "Elementos fijos: "
                 << cantidadFijos
                 << endl;

            cout << "Elementos optimizables: "
                 << cantidadOptimizables
                 << endl;

            cout << "Umax: "
                 << casa.Umax
                 << endl;

            cout << "Superficie: "
                 << casa.superficie
                 << " m2"
                 << endl;


            // ====================================================
            // GUARDAR INSTANCIA
            // ====================================================

            guardarInstancia(
                casa,
                numeroInstancia
            );


            numeroInstancia++;
        }
    }


    // ============================================================
    // RESUMEN FINAL
    // ============================================================

    cout << "\n\n========================================" << endl;
    cout << "RESUMEN FINAL" << endl;
    cout << "========================================" << endl;

    cout << "Instancias validas: "
         << cantidadValidas
         << endl;

    cout << "Instancias invalidas: "
         << cantidadInvalidas
         << endl;


    if (cantidadValidas == 15) {

        cout << "Se generaron correctamente las 15 instancias."
             << endl;
    }

    else {

        cout << "No se lograron generar 15 instancias validas."
             << endl;
    }


    return 0;
}
