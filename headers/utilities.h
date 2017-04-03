#ifndef UTILITIES_H
#define UTILITIES_H

#include "graph.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

typedef long long int int64;
typedef unsigned long long int uint64;

template<class T>
void loadNodes(Graph<T> &graph) {
	//Format: nodeID;latitudeDegrees;longitudeDegrees;longitudeRadians;latitudeRadians
	string line;
	ifstream file;
	file.open("rsc/NodeTest.txt");
	if (!file.is_open()){
		cout << "Failed to open Node txt file!\n";
		exit(1);
	}
	while (getline(file, line)) {
		T nodeID;
		double latitudeDegrees, longitudeDegrees;
		double longitudeRadians, latitudeRadians;
		char delimiter;
		istringstream iss(line);
		iss >> nodeID >> delimiter >> latitudeDegrees >> delimiter
				>> longitudeDegrees >> delimiter >> longitudeRadians
				>> delimiter >> latitudeRadians;
		//this is a temporary variable must use new
		graph.addVertex(
				*(new Vertex<T>(nodeID, latitudeRadians, longitudeRadians)));
	}
	file.close();
}

template<class T>
void loadEdges(Graph<T> &graph) {
	//Format: edgeID;node1ID;node2ID;
	string line;
	ifstream file;
	file.open("rsc/EdgeTest.txt");
	if (!file.is_open()) {
		cout << "Failed to open Edges txt file!\n";
		exit(1);
	}
	while (getline(file, line)) {
		T edgeID, node1ID, node2ID;
		char delimiter;
		istringstream iss(line);
		iss >> edgeID >> delimiter >> node1ID >> delimiter >> node2ID;
		graph.addEdgeID(node1ID, node2ID, edgeID);
	}
	file.close();
}

template<class T>
void loadStreets(Graph<T> &graph) {
	//Format: edgeID;streetName;isTwoWays;
	string line;
	ifstream file;
	file.open("rsc/StreetTest.txt");
	if (!file.is_open()) {
		cout << "Failed to open Streets txt file!\n";
		exit(1);
	}
	while (getline(file, line)) {
		T edgeID;
		string streetName, isTwoWaysStr;
		bool isTwoWays;
		char delimiter;
		istringstream iss(line);
		iss >> edgeID >> delimiter;
		getline(iss, streetName, delimiter);
		getline(iss, isTwoWaysStr);
		isTwoWays = (isTwoWaysStr == "True");
		for (uint64 i = 0; i < graph.getVertexSet().size(); i++) {
			Vertex<T> *vertex =  graph.getVertexSet()[i];
			for (uint64 j = 0;	j < vertex->getAdjacent().size(); j++) {
				Edge<T> *edge = &vertex->getAdjacent()[j];
				if (edge->getID() == edgeID) {
					edge->setName(streetName);
					edge->setTwoWays(isTwoWays);
					if(isTwoWays){
						//If it's two ways, create another edge which is the opposite of the original and so is its ID.
						graph.addEdgeID(edge->getDest()->getID(),
									vertex->getID(),
									-1 * edge->getID());
					}
				}
			}
		}
	}
	file.close();
}

/*!
 *	Calculates the distance between two points on earth (in km) using Haversine's formula
 */
template<class T>
double calculateDistance(Vertex<T> *v1, Vertex<T> *v2) {
	double earthRadius = 6371;
	double deltaLatitude = v2->getLatitude() - v1->getLatitude();
	double deltaLongitude = v2->getLongitude() - v1->getLongitude();

	double a = sin(deltaLatitude / 2) * sin(deltaLatitude / 2)
			+ cos(v1->getLatitude()) * cos(v2->getLatitude())
					* sin(deltaLongitude / 2) * sin(deltaLongitude / 2);
	double b = 2 * atan2(sqrt(a), sqrt(1 - a));
	double c = earthRadius * b;
	return c;
}

#endif // UTILITIES_H
