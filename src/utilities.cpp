#include <string>
#include <iostream>
#include "../headers/utilities.h"
using namespace std;
static string previous = "";
static uint64 counter = 0;

string nextName(string name){
	int sz = previous.size();
	if (sz == 0)
		previous = "A";
	else{
		if (previous[ sz-1 ] == 'Z' ){
			int i = sz-1;
			for (; i >= 0 ; i--){  //searches string for first char different from Z
				if ('Z' != previous[i]){
					previous[i]++;
					break;
				}
			}
			if (-1 == i)
				previous+="A";
			
			for(i=i+1; i<sz ; i++)
					previous[i] = 'A';
		}
		else
			previous[ sz - 1]++;
	}
	return previous;
}

string nextStreetName(){
	return nextName(previous);
}

uint64 nextInteger(){
	return counter++;
}

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
