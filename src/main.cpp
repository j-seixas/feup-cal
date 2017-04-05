#include "../headers/utilities.h"
#include "../headers/ui.h"
#include "../graph_viewer/graphviewer.h"
#include <iostream>

void run() {
	Graph<int64> graph;
	loadNodes(graph);
	loadEdges(graph);
	loadStreets(graph);
	GraphViewer *gv = graph.showGraph();
	menu(graph);
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
