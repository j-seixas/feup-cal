#include "../headers/ui.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>

void initGraph(Graph<int64> &graph){
	loadNodes(graph);
	loadEdges(graph);
	loadStreets(graph);
	graph.initDestinations();
}

void initGraphViewer(GraphViewer *gv){
	gv->createWindow(WIDTH, HEIGHT);
	gv->defineVertexColor("white");
	gv->defineEdgeColor("green");
}

void run() {
	srand(time(0));
	Graph<int64> graph;
	initGraph(graph);
	GraphViewer *gv = new GraphViewer(WIDTH, HEIGHT, false);
	initGraphViewer(gv);
	do{
		graph.updateGraphViewer(gv);
	} while(menu(graph));
	gv->closeWindow();
	delete gv;
}

int main() {
	run();
	return 0;
}
