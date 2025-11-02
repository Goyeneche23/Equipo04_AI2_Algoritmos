#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
using namespace std;


// Leer la matriz desde un archivo de texto
vector<vector<int>> leerMatriz(string nombreArchivo) {
    ifstream archivo(nombreArchivo.c_str());
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo de entrada." << endl;
        exit(1);
    }

    int n;
    archivo >> n; // número de colonias

    vector<vector<int>> matriz(n, vector<int>(n));

    // Leer matriz n x n
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            archivo >> matriz[i][j];
        }
    }

    archivo.close();
    return matriz;
}

// ---------------Mostrar la matriz en consola---------------------
void mostrarMatriz(vector<vector<int>> matriz) {
    int n = matriz.size();
    cout << "\nMatriz de adyacencia (" << n << "x" << n << "):" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << matriz[i][j] << "\t";
        }
        cout << endl;
    }
}

// ------------------------------Algoritmo de PRIM MST--------------------------
void primMST(vector<vector<int>> grafo) {
    int n = grafo.size();

    // arreglos auxiliares
    vector<int> clave(n, INT_MAX);  // guarda el peso minimo hacia un nodo
    vector<bool> incluido(n, false); // indica si ya se incluyo en el MST
    vector<int> padre(n, -1);        // guarda el nodo de donde viene la conexin

    clave[0] = 0; // empezamos desde la colonia A

    // Se repite N-1 veces porque un MST tiene N-1 aristas
    for (int c = 0; c < n - 1; c++) {
        int min = INT_MAX;
        int u = -1;

        // buscar el nodo no incluido con menor clave
        for (int v = 0; v < n; v++) {
            if (!incluido[v] && clave[v] < min) {
                min = clave[v];
                u = v;
            }
        }

        // marcar el nodo elegido como incluido
        incluido[u] = true;

        // actualizar los valores de los vecinos del nodo elegido
        for (int v = 0; v < n; v++) {
            if (grafo[u][v] != 0 && !incluido[v] && grafo[u][v] < clave[v]) {
                padre[v] = u;
                clave[v] = grafo[u][v];
            }
        }
    }

    // Mostrar resultado
    cout << "\n---- [1] Forma óptima de cablear (MST - Algoritmo de Prim) ----\n";
    int costoTotal = 0;
    for (int i = 1; i < n; i++) {
        cout << " (" << char('A' + padre[i]) << "," << char('A' + i)
             << ") - " << grafo[i][padre[i]] << " km\n";
        costoTotal += grafo[i][padre[i]];
    }
    cout << "Costo total: " << costoTotal << " km\n";
}


int main() {
    string nombreArchivo = "..\\Equipo04_Entrada.txt";

    vector<vector<int>> grafo = leerMatriz(nombreArchivo);

    mostrarMatriz(grafo);
    primMST(grafo);

    return 0;
}
