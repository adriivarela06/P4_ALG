#include <stack>
#include <vector>
#include <iostream>

using namespace std;

struct Proyectos{
    vector<pair<int, int>> conjunto;
};

struct Solucion{
    vector<bool> seleccionados;
    int coste;
    int beneficio;
    int limite;
    int contador_podas;
    int contador_soluciones;
    int size;

    Solucion(int n, Proyectos &p){
        limite = n;
        size = p.conjunto.size();
        seleccionados.resize(size, false);
        coste = 0;
        beneficio = 0;
        contador_podas = 0;
        contador_soluciones = 0;
    }

    bool funcFactibilidad(int coste_p, int indice){
        return (coste + coste_p <= limite);
    }
};

void backtracking(Solucion &S, Proyectos &P, int indice){
    if (indice == S.size){
        S.contador_soluciones++;
        cout << "Subconjunto " << S.contador_soluciones << ": { ";
        for (int i = 0; i < S.size; i++) {
            if (S.seleccionados[i]) {
                cout << (char)('A' + i) << " "; 
            }
        }
        cout << "}" << endl << "Beneficio Total: " << S.beneficio << endl << "Coste: " << S.coste << endl;
        return;
    }

    int coste_p = P.conjunto[indice].first;
    int beneficio_p = P.conjunto[indice].second;

    if (S.funcFactibilidad(coste_p, indice)){
        S.seleccionados[indice] = true;
        S.coste += coste_p;
        S.beneficio += beneficio_p;
        
        backtracking(S, P, indice + 1); //exploramos la siguiente rama que seguiria a nuestro proyecto factible
        S.seleccionados[indice] = false; // ahora quitamos estos datos para explorar otra rama distinta
        S.coste -= coste_p;
        S.beneficio -= beneficio_p;
    }
    else{
        S.contador_podas++;
    }
    S.seleccionados[indice] = false;
    backtracking(S, P, indice + 1);
}

int main() {
    Proyectos p;
    p.conjunto = {{2, 4}, {3, 5}, {5, 6}};
    
    int presupuesto = 8;
    Solucion S(presupuesto, p);
    backtracking(S, p, 0);
    cout << "Número soluciones posibles: " << S.contador_soluciones << endl;
    cout << "Número ramas podadas: " << S.contador_podas << endl;

    return 0;
}