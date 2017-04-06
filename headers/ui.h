#ifndef UI_H
#define UI_H

#include "../headers/utilities.h"
#include <string>

using namespace std;


template<class T>
bool menu(Graph<T> graph){
		cout << "Menu" << endl << endl
			<< "  1. Cut road" << endl << endl
			<< "  0. Exit" << endl << endl;
		uint16 option = getInput();
		if(option == 0){
			return false;
		}
		else if(option == 1){
			string streetName = getStreetName();
			graph.cutStreet(streetName);
			return true;
		}

		return false;
}


#endif /* UI_H */
