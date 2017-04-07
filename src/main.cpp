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

	/*
	Vertex<int64> *sourc = graph.getVertexByIDMask(88) , *dest = graph.getVertexByIDMask(138);
	graph.Astar( sourc , dest );
	for (Vertex<int64> * v : dest->backtrace() ){
		cout << v->getIDMask() << " ";
	}
	cout << endl;
	*/
	GraphViewer *gv = new GraphViewer(WIDTH, HEIGHT, false);
	initGraphViewer(gv);
	do{
		graph.updateGraphViewer(gv);
	} while(menu(graph));
	gv->closeWindow();
	delete gv;
	
}

int main(int argc , char *argv[]) {
	if (argc != 2 && argc != 1){
		cout << "Usage program <1 or 0 , optional>" << endl;
		return 1;
	}	
	//if (argc == 2)
			//debug = (atoi(argv[1]) == 1);
	run();
	return 0;
}
