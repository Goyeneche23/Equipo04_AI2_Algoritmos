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
    archivo >> n; // num de colonias

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
    cout << "\n---- [1] Forma opt de cablear (MST - Algoritmo de Prim) ----\n";
    int costoTotal = 0;
    for (int i = 1; i < n; i++) {
        cout << " (" << char('A' + padre[i]) << "," << char('A' + i)
             << ") - " << grafo[i][padre[i]] << " km\n";
        costoTotal += grafo[i][padre[i]];
    }
    cout << "Costo total: " << costoTotal << " km\n";
}

int mejorDistancia = INT_MAX;
vector<int> mejorRuta;

void tspBacktracking(vector<vector<int>> grafo, vector<bool> visitado,
                     vector<int> ruta, int costoActual, int origen) {
    int n = grafo.size();

    // si ya visitamos todas las colonias
    if (ruta.size() == n) {
        // regresar al punto de inicio
        costoActual += grafo[ruta.back()][origen];
        if (costoActual < mejorDistancia) {
            mejorDistancia = costoActual;
            mejorRuta = ruta;
        }
        return;
    }

    // probar ir a cada colonia no visitada
    for (int i = 0; i < n; i++) {
        if (!visitado[i] && grafo[ruta.back()][i] > 0) {
            visitado[i] = true;
            ruta.push_back(i);

            tspBacktracking(grafo, visitado, ruta,
                            costoActual + grafo[ruta[ruta.size() - 2]][i], origen);

            ruta.pop_back();
            visitado[i] = false;
        }
    }
}

void resolverTSP(vector<vector<int>> grafo) {
    int n = grafo.size();
    vector<bool> visitado(n, false);
    vector<int> ruta;

    ruta.push_back(0); // comenzamos en A
    visitado[0] = true;

    mejorDistancia = INT_MAX;
    mejorRuta.clear();

    tspBacktracking(grafo, visitado, ruta, 0, 0);

    cout << "\n---- [2] Ruta mas corta para visitar todas las colonias ----\n";
    cout << "Ruta: ";
    for (int i = 0; i < mejorRuta.size(); i++) {
        cout << char('A' + mejorRuta[i]) << " -> ";
    }
    cout << "A\n";
    cout << "Distancia total: " << mejorDistancia << " km\n";
}


int main() {
    string nombreArchivo = "..\\Equipo04_Entrada.txt";

    vector<vector<int>> grafo = leerMatriz(nombreArchivo);

    mostrarMatriz(grafo);
    primMST(grafo);
    resolverTSP(grafo);

    return 0;
}
