#include <queue>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

struct Proyectos{
    vector<pair<int, int>> conjunto;
    
};

struct Solucion{
    int nodosgenerados;
    int nodospodados;
    int beneficioOptimo;
    vector<bool> seleccionados;

    Solucion(){
        nodosgenerados=0;
        nodospodados=0;
        seleccionados.assign(0, false);
    }
};

bool compararProyectos(pair<int,int> p1, pair<int,int> p2){
        double r1 = (double)p1.first / p1.second;
        double r2 = (double)p2.first / p2.second;
        if (r1 == r2) return p2.second < p2.second; 
        return r1 > r2;
}

struct Nodo{
    int nivel;
    int beneficio;
    int coste;
    int limite;
    float cota;
    vector<bool> camino; 

    bool operator<(const Nodo& otro) const {
        return cota < otro.cota;
    }
};

float calcularCota(Nodo nodo, int n, int limite, Proyectos &P) {
    // Si el coste actual ya supera el límite, la cota es 0 (rama inviable)
    if (nodo.coste >= limite) return 0;

    float beneficio_cota = (float)nodo.beneficio;
    int coste_actual = nodo.coste;
    
    // Empezamos a mirar desde el SIGUIENTE proyecto al nivel actual
    int j = nodo.nivel + 1;

    // 1. Metemos todos los proyectos enteros que quepan
    while (j < n && (coste_actual + P.conjunto[j].first <= limite)) {
        coste_actual += P.conjunto[j].second;
        beneficio_cota += P.conjunto[j].first;
        j++;
    }

    if (j < n) {
        int espacio_libre = limite - coste_actual;
        float eficiencia = (float)P.conjunto[j].first / P.conjunto[j].second;
        beneficio_cota += espacio_libre * eficiencia;
    }

    return beneficio_cota;
}

void BranchandBound(Proyectos &P, int presupuesto, Solucion &s) {
    int n = P.conjunto.size();
    sort(P.conjunto.begin(), P.conjunto.end(), compararProyectos); 

    priority_queue<Nodo> Q;
    
    Nodo u;
    u.nivel = -1; u.beneficio = 0; u.coste = 0;
    u.camino.assign(n, false); 
    u.cota = calcularCota(u, n, presupuesto, P);
    Q.push(u);
    s.nodosgenerados++;

    while (!Q.empty()) {
        u = Q.top();
        Q.pop();

        if (u.cota <= s.beneficioOptimo) {
            s.nodospodados++;
            continue;
        }

        Nodo v;
        v.nivel = u.nivel + 1;
        if (v.nivel >= n) continue;

        v.beneficio = u.beneficio + P.conjunto[v.nivel].first;
        v.coste = u.coste + P.conjunto[v.nivel].second;
        v.camino = u.camino;   
        v.camino[v.nivel] = true; 

        if (v.coste <= presupuesto) {
            if (v.beneficio > s.beneficioOptimo) {
                s.beneficioOptimo = v.beneficio;
                s.seleccionados = v.camino;
            }
            v.cota = calcularCota(v, n, presupuesto, P);
            if (v.cota > s.beneficioOptimo) Q.push(v);
            else s.nodospodados++;
        } else {
            s.nodospodados++;
        }
        s.nodosgenerados++;

        v.beneficio = u.beneficio;
        v.coste = u.coste;
        v.camino = u.camino;
        v.camino[v.nivel] = 
        v.cota = calcularCota(v, n, presupuesto, P);

        if (v.cota > s.beneficioOptimo) {
            Q.push(v);
        } else {
            s.nodospodados++;
        }
        s.nodosgenerados++;
    }
}

int main() {
    Proyectos p;
    Solucion s;
    p.conjunto = {{4, 2}, {5, 3}, {6, 5}};
    int limite = 8;
    BranchandBound(p, limite,s);
    cout << "Beneficio Optimo: " << s.beneficioOptimo << endl;
    cout << "Nodos generados: " << s.nodosgenerados << endl;
    cout << "Nodos podados: " << s.nodospodados << endl;
    cout << "Proyectos seleccionados (segun orden de eficiencia): " << endl;
    for(int i = 0; i < s.seleccionados.size(); i++) {
        cout << "P" << i << ": " << (s.seleccionados[i] ? "SI" : "NO") << " ";
    }
    return 0;
}