#include "../headers/utilities.h"
#include <iostream>

void initializeTestGraph(Graph<int64> &graph);
void testAstar(Graph<int64> &graph);

int main() {

	Graph<int64> graph;
	//initializeTestGraph(graph);
	//testAstar(graph);
	
	loadNodes(graph);
	loadEdges(graph);
	loadStreets(graph);

	for (uint64 i = 0; i < graph.getVertexSet().size(); i++) {
		Vertex<int64> *vertex = graph.getVertexSet()[i];
		//cout << "Vertex number " << vertex->getID() << " Latitude " << vertex->getLatitude() << " Longitude " << vertex->getLongitude() << endl;
		for (uint64 j = 0; j < vertex->getAdjacent().size(); j++) {
			Edge<int64> edge = vertex->getAdjacent()[j];
			//cout << "\tEdge number " << edge.getID() << " Name " << edge.getName() << " isTwoWays " << edge.getTwoWays() << " Distance " << edge.getWeight() << endl;
		}
		//cout << endl;
	}
	
	graph.showGraph();
	return 0;
}

void initializeTestGraph(Graph<int64> &graph){
	for(int i = 1; i < 8; i++)
		graph.addVertex(i);

	graph.addEdge(1, 2, 2);
	graph.addEdge(1, 4, 7);
	graph.addEdge(2, 4, 3);
	graph.addEdge(2, 5, 5);
	graph.addEdge(3, 1, 2);
	graph.addEdge(3, 6, 5);
	graph.addEdge(4, 3, 1);
	graph.addEdge(4, 5, 1);
	graph.addEdge(4, 6, 6);
	graph.addEdge(4, 7, 4);
	graph.addEdge(5, 7, 2);
	graph.addEdge(6, 4, 3);
	graph.addEdge(7, 6, 4);
}

void testAstar(Graph<int64> &graph){
	vector<Vertex<int64>* > vs = graph.getVertexSet();

	graph.Astar(graph.getVertex(1) , graph.getVertex(3) );

	stringstream ss;
	for(unsigned int i = 0; i < vs.size(); i++) {
		ss << vs[i]->getID() << "<-";
		if ( vs[i]->path != NULL )  ss << vs[i]->path->getID();
		ss << "|";
	}

	if(("1<-3|2<-1|3<-|4<-2|5<-4|6<-3|7<-5|") == ( ss.str()))
		cout << "PASSED:  ss.str()" << endl;
	else
	   cout << "ERROR:  ss.str() expected: 1<-3|2<-1|3<-|4<-2|5<-4|6<-3|7<-5| got " << ( ss.str()) << endl;


	graph.Astar(graph.getVertex(1) , graph.getVertex(5) );

	ss.str("");
	for(unsigned int i = 0; i < vs.size(); i++) {
		ss << vs[i]->getID() << "<-";
		if ( vs[i]->path != NULL )  ss << vs[i]->path->getID();
		ss << "|";
	}

	if(("1<-|2<-1|3<-4|4<-2|5<-4|6<-4|7<-5|") == ( ss.str()))
		cout << "PASSED:  ss.str()" << endl;
	else
	   cout << "ERROR:  ss.str() expected: 1<-|2<-1|3<-4|4<-2|5<-4|6<-4|7<-5| got " << ( ss.str()) << endl;
}