#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <cmath> 
#include <queue>   // [P3] BFS Edmonds–Karp
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
            //si se encuentra un fin se marca como visitada
            visitado[i] = true;
            ruta.push_back(i);
            // llamada recursiva
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
    for (int i = 0; i < (int)mejorRuta.size(); i++) {
        cout << char('A' + mejorRuta[i]) << " -> ";
    }
    cout << "A\n";
    cout << "Distancia total: " << mejorDistancia << " km\n";
}


/* ============================================================
   ===                 [PUNTO 3] Máximo Flujo               ===
   ===   Sección añadida SIN modificar lo anterior          ===
/* ============================================================ */

vector<vector<int>> leerCapacidades(const string& nombreArchivo) {
    ifstream in(nombreArchivo.c_str());
    if (!in.is_open()) {
        cout << "Error: No se pudo abrir el archivo de entrada." << endl;
        exit(1);
    }

    int n;
    if (!(in >> n) || n <= 1) {
        cout << "Error: N invalido." << endl;
        exit(1);
    }

    // consumir primera matriz (distancias)
    int dummy;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (!(in >> dummy)) {
                cout << "Error: matriz de distancias incompleta." << endl;
                exit(1);
            }
        }
    }

    // leer segunda matriz (capacidades)
    vector<vector<int>> cap(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (!(in >> cap[i][j])) {
                cout << "Error: matriz de capacidades incompleta." << endl;
                exit(1);
            }
        }
    }

    return cap;
}

/* Edmonds–Karp: BFS sobre red residual.
 */
int maxFlowEdmondsKarp(vector<vector<int>> cap, int s, int t) {
    int n = (int)cap.size();
    int flujoMax = 0;

    // BFS repetido hasta que no haya camino aumentante
    while (true) {
        vector<int> parent(n, -1);
        parent[s] = s;
        queue<int> q;
        q.push(s);

        while (!q.empty() && parent[t] == -1) {
            int u = q.front(); q.pop();
            for (int v = 0; v < n; ++v) {
                if (parent[v] == -1 && cap[u][v] > 0) {
                    parent[v] = u;
                    q.push(v);
                }
            }
        }

        if (parent[t] == -1) break; // no hay más incrementos

        // Encontrar cuello de botella
        int incremento = numeric_limits<int>::max();
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            incremento = min(incremento, cap[u][v]);
        }

        // Actualizar residual
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            cap[u][v] -= incremento;
            cap[v][u] += incremento;
        }

        flujoMax += incremento;
    }

    return flujoMax;
}


void resolverFlujoMaximo(const vector<vector<int>>& cap) {
    int n = (int)cap.size();
    int s = 0;        // A
    int t = n - 1;    // última colonia
    int valor = maxFlowEdmondsKarp(cap, s, t);

    cout << "\n---- [3] Flujo maximo de informacion ----\n";
    cout << "Origen:  " << char('A' + s) << "\n";
    cout << "Destino: " << char('A' + t) << "\n";
    cout << "Valor de flujo maximo: " << valor << "\n";
}


// Punto 4

struct Punto {
    double x;
    double y;
};

// Devuelve el índice de la central más cercana a una nueva casa
int encontrarCentralMasCercana(const vector<Punto>& centrales, const Punto& nuevaCasa) {
    if (centrales.empty()) {
        cout << "Error: no hay centrales registradas." << endl;
        return -1;
    }

    int indiceMasCercano = 0;
    double distanciaMin = numeric_limits<double>::max();

    for (int i = 0; i < (int)centrales.size(); ++i) {
        double dx = centrales[i].x - nuevaCasa.x;
        double dy = centrales[i].y - nuevaCasa.y;
        double distancia = sqrt(dx * dx + dy * dy); // fórmula euclidiana

        if (distancia < distanciaMin) {
            distanciaMin = distancia;
            indiceMasCercano = i;
        }
    }

    cout << "\n---- [4] Central más cercana ----\n";
    cout << "Nueva casa en (" << nuevaCasa.x << ", " << nuevaCasa.y << ")\n";
    cout << "Central más cercana: #" << indiceMasCercano + 1 
         << " en (" << centrales[indiceMasCercano].x << ", " 
         << centrales[indiceMasCercano].y << ")\n";
    cout << "Distancia: " << distanciaMin << " unidades\n";

    return indiceMasCercano;
}



int main() {
    string nombreArchivo = "..\\Equipo04_Entrada.txt";

    vector<vector<int>> grafo = leerMatriz(nombreArchivo);

    mostrarMatriz(grafo);
    primMST(grafo);
    resolverTSP(grafo);

    vector<vector<int>> capacidades = leerCapacidades(nombreArchivo);
    resolverFlujoMaximo(capacidades);
    /* --------------------------------------------------------------------- */

    vector<Punto> centrales = {
        {2.0, 3.0},
        {5.0, 7.0},
        {9.0, 4.0}
    };

    Punto nuevaCasa = {4.0, 5.0};

    encontrarCentralMasCercana(centrales, nuevaCasa);
    return 0;
}
