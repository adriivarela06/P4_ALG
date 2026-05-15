.PHONY: all clean

all:
	mkdir -p bin
	g++ -std=c++17 -Wall -Wextra -I./include src/graph_coloring.cpp src/main.cpp -o bin/graph_coloring
	g++ -std=c++17 -Wall -Wextra -I./include src/seleccion_proyectos_backtracking.cpp -o bin/seleccion_proyectos_backtracking
	g++ -std=c++17 -Wall -Wextra -I./include src/seleccion_proyectos_branchandbound.cpp -o bin/seleccion_proyectos_branchandbound

clean:
	rm -rf bin
