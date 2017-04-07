#include "../headers/ui.h"
#include <iostream>

void initGraph(Graph<int64> &graph){
	loadNodes(graph);
	loadEdges(graph);
	loadStreets(graph);
}

void initGraphViewer(GraphViewer *gv){
	gv->createWindow(WIDTH, HEIGHT);
	gv->defineVertexColor("white");
	gv->defineEdgeColor("green");
}

void run() {
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
