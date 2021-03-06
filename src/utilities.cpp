#include <string>
#include <iostream>
#include "../headers/utilities.h"
using namespace std;

uint16 getInput() {
	string line;
	uint16 input;

	while (true) {
		cin.clear();
		cout << "Option: ";
		getline(cin, line);
		if (cin.eof())
			exit(1);
		istringstream iss(line);
		iss >> input;
		if (!iss.fail() && line.length() > 0)
			return input;
	}
}

string getStreetName(){
	string streetName;
	cin.clear();
	cout << "Name of the street: ";
	getline(cin, streetName);
	if (cin.eof())
		exit(1);
	cout << endl;
	return streetName;
}

unsigned int numberOfLines(const char * file_name){
    unsigned int number_of_lines = 0;
    FILE *in = fopen(file_name, "r");
    int ch;
    while (EOF != ( ch =getc(in)))
        if ('\n' == ch)
            ++number_of_lines;
    return number_of_lines;
}

void printSquareArray(int ** arr, unsigned int size){
	for(unsigned int k = 0; k < size; k++){
		if(k == 0){
			cout <<  "   ";
			for(unsigned int i = 0; i < size; i++)
				cout <<  " " << i+1 << " ";
			cout << endl;
		}

		for(unsigned int i = 0; i < size; i++){
			if(i == 0)
				cout <<  " " << k+1 << " ";

			if(arr[k][i] == INT_INFINITY)
				cout << " - ";
			else
				cout <<  " " << arr[k][i] << " ";
		}

		cout << endl;
	}
}
