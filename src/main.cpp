#include "../headers/ui.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>

void initGraph(Graph<long long int> &graph){
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
	Graph<long long int> graph;
	initGraph(graph);
	GraphViewer *gv = new GraphViewer(WIDTH, HEIGHT, false);
	initGraphViewer(gv);
	do{
		graph.updateGraphViewer(gv);
	} while(menu(graph));
	gv->closeWindow();
	delete gv;
	
}

int main(int argc , char *argv[]) {
	run();
	return 0;
}
