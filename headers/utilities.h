#ifndef UTILITIES_H
#define UTILITIES_H

#include "graph.h"
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cmath>

using namespace std;

typedef unsigned int uint16;
typedef long long int int64;
typedef unsigned long long int uint64;

string nextStreetName();
string getStreetName();
uint16 getInput();
void printSquareArray(int ** arr, unsigned int size);

template<class T>
void loadNodes(Graph<T> &graph) {
	//Format: nodeID;latitudeDegrees;longitudeDegrees;longitudeRadians;latitudeRadians
	string line;
	ifstream file;
	file.open("rsc/Nodes.txt");
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
		graph.addVertex(new Vertex<T>(nodeID, latitudeRadians, longitudeRadians));
	}
	file.close();
}

template<class T>
void loadEdges(Graph<T> &graph) {
	//Format: edgeID;node1ID;node2ID;
	string line;
	ifstream file;
	file.open("rsc/Edges.txt");
	if (!file.is_open()) {
		cout << "Failed to open Edges txt file!\n";
		exit(1);
	}
	while (getline(file, line)) {
		T edgeID, srcID, dstID;
		char delimiter;
		istringstream iss(line);
		iss >> edgeID >> delimiter >> srcID >> delimiter >> dstID;
		Vertex<T>* src = graph.getVertex(srcID);
		Vertex<T>* dst = graph.getVertex(dstID);
		if(src != nullptr && dst != nullptr)
			graph.addEdge(new Edge<T>(dst, edgeID, calculateDistance(src, dst)), src);
	}
	file.close();
}

template<class T>
void loadStreets(Graph<T> &graph) {
	//Format: edgeID;streetName;isTwoWays;
	string line;
	ifstream file;
	file.open("rsc/Streets.txt");
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
					edge->setNameMask(nextStreetName());
					edge->setTwoWays(isTwoWays);
					if(isTwoWays){
						//If it's two ways, create another edge which is the opposite of the original and so is its ID.
						Edge<T>* oppositeEdge = new Edge<T>(vertex, -1 * edge->getID(), calculateDistance(vertex, edge->getDest()));
						oppositeEdge->setName(streetName);
						oppositeEdge->setNameMask(nextStreetName());
						graph.addEdge(oppositeEdge, edge->getDest());
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
