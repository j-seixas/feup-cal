#include "../headers/ui.h"
#include <iostream>
#include <chrono>
#include <time.h>
#include <stdlib.h>


void initGraph(Graph<long long int> &graph){
	std::chrono::high_resolution_clock::time_point current = std::chrono::high_resolution_clock::now();
	loadNodes(graph);
	loadEdges(graph);
	loadStreets(graph);
	std::chrono::high_resolution_clock::time_point final = std::chrono::high_resolution_clock::now();
	cout << "   Loading Time: " << std::chrono::duration_cast<std::chrono::duration<double>>(final - current).count() << "s\n";
}

void initGraphViewer(GraphViewer *gv){
	gv->createWindow(WIDTH, HEIGHT);
	gv->defineVertexColor("white");
	gv->defineEdgeColor("green");
}

void run() {
	srand(time(0));
	Graph<long long int> graph;
	initGraph(graph);
	GraphViewer *gv = new GraphViewer(WIDTH, HEIGHT, false);
	initGraphViewer(gv);
	do{
		graph.updateGraphViewer(gv);
	} while(menu(graph,gv));
	gv->closeWindow();
	delete gv;
}

int main() {
	run();
	return 0;
}
