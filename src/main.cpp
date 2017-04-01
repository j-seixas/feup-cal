#include "utilities.h"
#include "graph.h"

int main() {

	Graph<uint64> graph;
	loadNodes(graph);
	loadEdges(graph);
	return 0;
}