#include <bits/stdc++.h>
#include "Instancia.h"
#include "generador.h"
#include "validacion.h"

using namespace std;

// ======================================================
// Guarda una instancia en formato .dzn para MiniZinc
// ======================================================

void guardarInstanciaDZN(
    const Instancia& casa,
    int numeroInstancia
) {

    // Guardamos los elementos optimizables
    vector<const Elemento*> optimizables;
    double areaTotal = 0.0;
    double aporteUFijo = 0.0;
    double costoFijo = 0.0;
    int maxAlt = 0;

    // --------------------------------------------------
    // Recorrer todos los elementos
    // --------------------------------------------------

    for (const Elemento& elemento : casa.elementos) {
        areaTotal += elemento.area;
        // Elementos fijos
        if (elemento.fijo) {
            aporteUFijo += elemento.area * elemento.Ufijo;
            costoFijo += elemento.area * elemento.costoFijoM2;
        }

        // Elementos optimizables
        else {
            optimizables.push_back(&elemento);
            maxAlt = max(
                maxAlt, (int) elemento.alternativas.size()
            );
        }
    }

    // --------------------------------------------------
    // Nombre del archivo
    // --------------------------------------------------

    string nombreArchivo = "minizinc/datos/" + casa.tipo + "_" + to_string(numeroInstancia) + ".dzn";
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "No se pudo crear " << nombreArchivo << endl;  
        return;
    }

    // Para conservar precision numerica
    archivo << setprecision(15);

    // --------------------------------------------------
    // Datos generales
    // --------------------------------------------------

    archivo << "nOpt = " << optimizables.size() << ";\n\n";
    archivo << "maxAlt = " << maxAlt << ";\n\n";

    // --------------------------------------------------
    // Cantidad de alternativas
    // --------------------------------------------------

    archivo << "nAlt = [";
    for (int i = 0; i < optimizables.size(); i++) {
        if (i > 0)
            archivo << ", ";
        archivo << optimizables[i]->alternativas.size();
    }
    archivo << "];\n\n";

    // --------------------------------------------------
    // Nombres de los elementos
    // --------------------------------------------------

    archivo << "nombreElemento = [";
    for (int i = 0; i < optimizables.size(); i++) {
        if (i > 0)
            archivo << ", ";
        archivo << "\"" << optimizables[i]-> nombre << "\"";
    }
    archivo << "];\n\n";

    // --------------------------------------------------
    // Areas
    // --------------------------------------------------

    archivo << "area = [";
    for (int i = 0; i < optimizables.size(); i++) {
        if (i > 0)
            archivo << ", ";
        archivo << optimizables[i]->area;
    }
    archivo << "];\n\n";

    // --------------------------------------------------
    // Matriz U
    // --------------------------------------------------

    archivo << "U = [|\n";
    for (int e = 0; e < optimizables.size(); e++) {
        archivo << "    ";
        for (int a = 0; a < maxAlt; a++) {
            if (a > 0)
                archivo << ", ";
            if (a < optimizables[e]->alternativas.size()) {
                archivo << optimizables[e] ->alternativas[a].U;
            }
            else {
                archivo << "0.0";
            }
        }
        archivo << " |";
        if (e < optimizables.size() - 1)
            archivo << "\n";
    }
    archivo << "\n|];\n\n";

    // --------------------------------------------------
    // Matriz de costos
    // --------------------------------------------------

    archivo << "costo = [|\n";
    for (int e = 0; e < optimizables.size(); e++) {
        archivo << "    ";
        for (int a = 0; a < maxAlt; a++) {
            if (a > 0)
                archivo << ", ";
            if (a < optimizables[e]->alternativas.size()) {
                archivo << optimizables[e] ->alternativas[a].costoM2;
            }

            else {
                archivo << "0.0";
            }
        }

        archivo << " |";
        if (e < optimizables.size() - 1)
            archivo << "\n";
    }
    archivo << "\n|];\n\n";

    // --------------------------------------------------
    // Parametros globales
    // --------------------------------------------------

    archivo << "Umax = " << casa.Umax << ";\n\n";
    archivo << "areaTotal = " << areaTotal << ";\n\n";
    archivo << "aporteUFijo = " << aporteUFijo << ";\n\n";
    archivo << "costoFijo = " << costoFijo << ";\n";
    archivo.close();
    cout << "DZN generado: " << nombreArchivo << endl;
}

// ======================================================
// MAIN
// ======================================================

int main() {
    vector<string> tipos = {
        "pequena",
        "mediana",
        "grande"
    };

    int semilla = 101;
    for (const string& tipo : tipos) {
        int validasTipo = 0;
        while (validasTipo < 5) {
            Instancia casa = generarInstancia(tipo, semilla);
            if (!validarInstancia(casa)) {
                semilla++;
                continue;
            }
            validasTipo++;
            guardarInstanciaDZN(casa, validasTipo);
            semilla++;
        }
    }

    cout << "\n==============================" << endl;
    cout << "15 archivos DZN generados" << endl;
    cout << "==============================" << endl;
    return 0;
}