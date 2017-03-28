#ifndef UTILITIES_H
#define UTILITIES_H

#include "utilities.h"
#include "graph.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

typedef unsigned long long int uint64;

template <class T>
void loadNodes(Graph<T> graph) {
	//Format: node_id;latitude_in_degrees;longitude_in_degrees;longitude_in_radians;latitude_in_radians
	string line;
	ifstream file;
	file.open("NodeTest.txt");
	if (file.is_open()) {
		while (getline(file, line)) {
			uint64 node_id;
			double latitude_in_degrees, longitude_in_degrees;
			double longitude_in_radians, latitude_in_radians;
			char delimiter;
			istringstream iss(line);
			iss >> node_id >> delimiter
				>> latitude_in_degrees >> delimiter
				>> longitude_in_degrees >> delimiter
				>> longitude_in_radians >> delimiter
				>> latitude_in_radians;
			graph.addVertex(node_id);
		}
		file.close();
	}
}

#endif // UTILITIES_H