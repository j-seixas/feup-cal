#ifndef UI_H_
#define UI_H_

#include "utilities.h"
#include <string>

using namespace std;


template<class T>
void menu(Graph<T> graph){
	while(true){
		cout << "Menu" << endl << endl
			<< "  1. Cut road" << endl << endl
			<< "  0. Exit" << endl << endl;
		uint16 option = getInput();
		if(option == 0){
			return;
		}
		else if(option == 1){
			string streetName = getStreetName();
			graph.cutStreet(streetName);
		}
	}
}


#endif /* UI_H */
