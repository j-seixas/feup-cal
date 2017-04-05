#include "../headers/ui.h"
#include <iostream>

void initGraph(Graph<int64> &graph){
	loadNodes(graph);
	loadEdges(graph);
	loadStreets(graph);
}

void initGraphViewer(GraphViewer *gv){
	gv->createWindow(1000, 800);
	gv->defineVertexColor("white");
	gv->defineEdgeColor("green");
}

void run() {
	Graph<int64> graph;
	initGraph(graph);
	GraphViewer *gv = new GraphViewer(1000, 800, true);
	initGraphViewer(gv);
	do{
		graph.updateGraphViewer(gv);
	} while(menu(graph));
	gv->closeWindow();
	delete gv;

	/*for (uint64 i = 0; i < graph.getVertexSet().size(); i++) {
	 Vertex<int64> *vertex = graph.getVertexSet()[i];
	 cout << "Vertex number " << vertex->getID() << " Latitude " << vertex->getLatitude() << " Longitude " << vertex->getLongitude() << endl;
	 for (uint64 j = 0; j < vertex->getAdjacent().size(); j++) {
	 Edge<int64> edge = vertex->getAdjacent()[j];
	 cout << "\tEdge number " << edge.getID() << " Name " << edge.getName() << " isTwoWays " << edge.getTwoWays() << " Distance " << edge.getWeight() << endl;
	 }
	 cout << endl;
	 }*/
}

int main() {
	run();
	return 0;
}
