#include "../headers/utilities.h"
#include "../headers/graph.h"
#include <Windows.h>
#include <iostream>

int main() {

	char dir[256];
	GetCurrentDirectory(256, dir);
	cout << dir << endl;

	Graph<uint64> graph;
	loadNodes(graph);
	loadEdges(graph);
	loadStreets(graph);

	for (uint64 i = 0; i < graph.getVertexSet().size(); i++) {
		Vertex<uint64> *vertex = graph.getVertexSet()[i];
		cout << "Vertex number " << vertex->getID() << " Latitude " << vertex->getLatitude() << " Longitude " << vertex->getLongitude() << endl;
		for (uint64 j = 0; j < vertex->getAdjacent().size(); j++) {
			Edge<uint64> edge = vertex->getAdjacent()[j];
			cout << "\tEdge number " << edge.getID() << " Name " << edge.getName() << " isTwoWays " << edge.getTwoWays() << " Distance " << edge.getWeight() << endl;
		}
		cout << endl;
	}

	graph.showGraph();
	return 0;
}
