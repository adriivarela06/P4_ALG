#include "graph_coloring.h"

#include "graph_coloring.h"

#include <iostream>
#include <string>
#include <chrono> // Añadido para medir el tiempo

using namespace std;
using namespace std::chrono; // Añadido para acortar las llamadas a chrono

/*
Instrucciones de compilación: g++ -std=c++17 main.cpp graph_coloring.cpp -o graph_coloring
*/


/**
 * @brief Función principal que carga un grafo, realiza coloreo voraz y ejecuta algoritmos de backtracking y branch-and-bound.
 * @param argc Número de argumentos de línea de comandos.
 * @param argv Array de argumentos de línea de comandos (espera ruta de archivo de grafo y opcionalmente colores máximos).
 * @return 0 en éxito, 1 en error.
 */
int main(int argc, char* argv[]) {
   if (argc < 2) {
      cout << "Usage: " << argv[0] << " <graph_file> [max_colors_for_backtracking]" << endl;
      cout << "Example: " << argv[0] << " data/map_coloring_example.txt 3" << endl;
      return 0;
   }

   string file_path = argv[1];
   Graph graph = parse_graph_file(file_path);

   if (graph.num_vertices <= 0) {
      cout << "Failed to load graph. Please check the file format." << endl;
      return 1;
   }

   int edge_count = 0;
   for (const auto& neighbors : graph.adj) {
      edge_count += static_cast<int>(neighbors.size());
   }
   edge_count /= 2;

   cout << "Loaded graph with " << graph.num_vertices << " vertices and " << edge_count << " edges." << endl;

   vector<int> greedy_colors = greedy_coloring(graph);
   int greedy_used = count_used_colors(greedy_colors);
   cout << "Greedy coloring uses " << greedy_used << " colors." << endl;

   if (argc >= 3) {
      int max_colors = stoi(argv[2]);
        
      // Medición de tiempo para Backtracking
      auto start_bt = high_resolution_clock::now();
      ColoringResult backtracking_result = backtracking_color_with_k(graph, max_colors);
      auto stop_bt = high_resolution_clock::now();
      auto duration_bt = duration_cast<microseconds>(stop_bt - start_bt);
        
      print_coloring_solution(backtracking_result, "Backtracking with max K=" + to_string(max_colors));
      cout << "-> Tiempo de ejecución (Backtracking): " << duration_bt.count() << " microsegundos.\n" << endl;
   }

   // Medición de tiempo para Branch and Bound
   auto start_bnb = high_resolution_clock::now();
   ColoringResult bnb_result = branch_and_bound_min_colors(graph);
   auto stop_bnb = high_resolution_clock::now();
   auto duration_bnb = duration_cast<microseconds>(stop_bnb - start_bnb);
    
   print_coloring_solution(bnb_result, "Branch and Bound Optimal Coloring");
   cout << "-> Tiempo de ejecución (Branch and Bound): " << duration_bnb.count() << " microsegundos." << endl;

   return 0;
}
