#ifndef UI_H
#define UI_H

#include "../headers/utilities.h"
#include <string>
#include <chrono>

using namespace std;


template<class T>
void  carsMovingMenu( Graph<T> &graph , Vertex<T> *sourc , GraphViewer *gv, unsigned long int &n_nodes){
	bool run_all = false;
	char chr;
	cout << "Generating alternatives at character inserted \n";
	for (Vertex<T> *dest : graph.getCarsDest() ){
		cout << "Generating for " << sourc->getIDMask() << " -> " << dest->getIDMask();
		if(!run_all){
			if (cin.peek() != EOF)
				cin.get( chr );
			if (chr == ' ')
				run_all = true;
		}

		std::chrono::high_resolution_clock::time_point current = std::chrono::high_resolution_clock::now();
		graph.Astar(sourc,dest,n_nodes);
		cout << "	A* took " << std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - current).count() << "s \n";
		if (dest->path != NULL){
			dest->setResolved(true);
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
	unsigned long int n_nodes = 0;
	cout << "Menu" << endl
		 << "  1. Cut road" << endl
		 << "  2. Reset" << endl
		 << "  0. Exit" << endl;
		uint16 option = getInput();
		cout << endl;
		if(option == 1) {
			string streetName = getStreetName();
			transform(streetName.begin(), streetName.end(), streetName.begin(), ::toupper);
			if(graph.exactWordSearch(streetName)){
				Vertex<T> * v = graph.cutStreet(streetName,n_nodes);
				if ( v != nullptr){
					graph.updateGraphViewer(gv);
					carsMovingMenu(graph,v,gv,n_nodes);
				}
			} else {
				list<string>* similarNames = graph.approximateWordSearch(streetName);
				auto it = similarNames->begin();
				auto ite = similarNames->end();
				if(it != ite){
					cout << "Street name not found. Did you mean any of these streets?\n";
					while(it != ite){
						cout << "    " << *it << endl;
						it++;
					}
				} else
					cout << "Street name found. No similar names found.\n";
			}
			return true;
		} else if(option == 2) {
			graph.resetGraph();
			return true;
		}

		return false;
}
#endif /* UI_H */
