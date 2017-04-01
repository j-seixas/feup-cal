#ifndef UTILITIES_H
#define UTILITIES_H

#include "utilities.h"
#include "graph.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

typedef unsigned long long int uint64;

template <class T>
void loadNodes(Graph<T> &graph) {
	//Format: nodeID;latitudeDegrees;longitudeDegrees;longitudeRadians;latitudeRadians
	string line;
	ifstream file;
	file.open("NodeTest.txt");
	if (file.is_open()) {
		while (getline(file, line)) {
			T nodeID;
			double latitudeDegrees, longitudeDegrees;
			double longitudeRadians, latitudeRadians;
			char delimiter;
			istringstream iss(line);
			iss >> nodeID >> delimiter
				>> latitudeDegrees >> delimiter
				>> longitudeDegrees >> delimiter
				>> longitudeRadians >> delimiter
				>> latitudeRadians;
			graph.addVertex(Vertex<T>(nodeID, latitudeRadians, longitudeRadians));
		}
		file.close();
	}
}

template<class T>
void loadEdges(Graph<T> &graph) {
	//Format: edgeID;node1ID;node2ID;
	string line;
	ifstream file;
	file.open("EdgeTest.txt");
	if (file.is_open()) {
		while (getline(file, line)) {
			T edgeID, node1ID, node2ID;
			char delimiter;
			istringstream iss(line);
			iss >> edgeID >> delimiter
				>> node1ID >> delimiter
				>> node2ID;
			graph.addEdgeID(node1ID, node2ID, edgeID);
		}
		file.close();
	}
}

/*!
*	Calculates the distance between two points on earth (in km) using Haversine's formula
*/
template<class T>
double calculateDistance(Vertex<T> *v1, Vertex<T> *v2) {
	double earthRadius = 6371;
	double deltaLatitude = v2->getLatitude() - v1->getLatitude();
	double deltaLongitude = v2->getLongitude() - v1->getLongitude();
	
	double a =
		sin(deltaLatitude / 2) * sin(deltaLatitude / 2) +
		cos(v1->getLatitude()) * cos(v2->getLatitude()) *
		sin(deltaLongitude / 2) * sin(deltaLongitude / 2);
	double b = 2 * atan2(sqrt(a), sqrt(1 - a));
	double c = earthRadius * b;
	return c;
}

#endif // UTILITIES_H