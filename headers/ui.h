#ifndef UI_H
#define UI_H

#include "../headers/utilities.h"
#include <string>
#include <chrono>

using namespace std;


template<class T>
void  carsMovingMenu( Graph<T> &graph , Vertex<T> *sourc , GraphViewer *gv){
	cout << "Generating alternatives each second or character inserted \n";
	for (Vertex<T> *dest : graph.getCarsDest() ){
		cout << "Generating for " << sourc->getIDMask() << " -> " << dest->getIDMask() << " \n";
		//wait either 1 second or less if user inputs something
		std::chrono::high_resolution_clock::time_point current = std::chrono::high_resolution_clock::now();
		//while ( (std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - current).count() < 1) && cin.peek() == EOF ){}	
		if (cin.peek() != EOF)
			cin.get();
		current = std::chrono::high_resolution_clock::now();
		graph.Astar(sourc,dest);
		cout << "	A* took " << std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - current).count() << "s \n";
		if (dest->path != NULL){
			dest->setResolved(true);
			for (Vertex<T> * v : dest->backtrace() )
				cout << v->getIDMask() << "->";
			cout << "\n";
			graph.updatePath(dest);
		}
		else{
			dest->setReachable(false);
			cout << "No path found for " << sourc->getIDMask() << " -> " << dest->getIDMask() << "\n"; 
		}
		graph.updateGraphViewer(gv);
	}
}

template<class T>
bool menu(Graph<T> &graph, GraphViewer *gv){
	cout << "Menu" << endl
		 << "  1. Cut road" << endl
		 << "  2. Update" << endl
		 << "  0. Exit" << endl;
		uint16 option = getInput();
		cout << endl;
		if(option == 1){
			string streetName = getStreetName();
			Vertex<T> * v = graph.cutStreet(streetName);
			if ( v != NULL){
				graph.updateGraphViewer(gv);
				carsMovingMenu(graph,v,gv);
			}
			graph.show_name = true;
			return true;
		}

		return false;
}
#endif /* UI_H */