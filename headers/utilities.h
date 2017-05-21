#ifndef UTILITIES_H
#define UTILITIES_H

#include "graph.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <string>
#include <map>
#include <cmath>

#define NODES_FILE "rsc/_nodes.txt"
#define EDGES_FILE "rsc/_edges.txt"
#define STREETS_FILE "rsc/_streets.txt"

using namespace std;

typedef unsigned int uint16;
typedef long long int int64;

unsigned int numberOfLines(const char * file_name);
string nextStreetName();
string getStreetName();
uint16 getInput();
void printSquareArray(int ** arr, unsigned int size);

static map<long long int, unsigned long int> node_big_to_small;
static bool init = false;
static double minLatitute;
static double maxLatitude;
static double minLongitude;
static double maxLongitude;

const int WIDTH = 1000;
const int HEIGHT = 1000;

template<class T>
pair<int, int> calculatePosition(Vertex<T>* v) {
	int x;
	if(v->getLongitude() > 0.0)
		x = (v->getLongitude() - minLongitude) * WIDTH / (maxLongitude - minLongitude);
	else
		x = (minLongitude - v->getLongitude()) * WIDTH / (maxLongitude - minLongitude) + WIDTH;

	int y;
	if(v->getLatitude() > 0.0)
		y = (v->getLatitude() - minLatitute) * HEIGHT / (maxLatitude - minLatitute);
	else
		y = (minLatitute - v->getLatitude()) * HEIGHT / (maxLatitude - minLatitute) + HEIGHT;

	return pair<int,int>(x, y);
}

template<class T>
void updateBounds(const Vertex<T> *v) {
	if (init) {
		if (v->getLatitude() < minLatitute)
			minLatitute = v->getLatitude();
		else if (v->getLatitude() > maxLatitude)
			maxLatitude = v->getLatitude();
		if (v->getLongitude() < minLongitude)
			minLongitude = v->getLongitude();
		else if (v->getLongitude() > maxLongitude)
			maxLongitude = v->getLongitude();
	} else {
		minLatitute = v->getLatitude();
		maxLatitude = v->getLatitude();
		minLongitude = v->getLongitude();
		maxLongitude = v->getLongitude();
		init = true;
	}
}

template<class T>
void loadNodes(Graph<T> &graph) {
	//Format: nodeID;latitudeDegrees;longitudeDegrees;longitudeRadians;latitudeRadians
	string line, file_name = NODES_FILE;
	ifstream file;
	file.open(file_name);
	if (!file.is_open()) {
		cout << "Failed to open Node txt file!\n";
		exit(1);
	}
	unsigned int n_lines = numberOfLines( file_name.c_str() );
	graph.initializeSet(n_lines+1);
	while (getline(file, line)) {
		T nodeID;
		double latitudeDegrees, longitudeDegrees;
		double longitudeRadians, latitudeRadians;
		char delimiter;
		istringstream iss(line);
		iss >> nodeID >> delimiter >> latitudeDegrees >> delimiter
				>> longitudeDegrees >> delimiter >> longitudeRadians
				>> delimiter >> latitudeRadians;
		Vertex<T> *v = new Vertex<T>(nodeID, latitudeRadians, longitudeRadians);
		updateBounds(v);
		graph.addVertex(v);
		node_big_to_small.insert( pair<long long int , unsigned long int>(nodeID , graph.getCounter()-1) );
	}
	file.close();
}

template<class T>
void loadEdges(Graph<T> &graph) {
	//Format: edgeID;node1ID;node2ID;
	string line;
	ifstream file;
	file.open(EDGES_FILE);
	if (!file.is_open()) {
		cout << "Failed to open Edges txt file!\n";
		exit(1);
	}
	while (getline(file, line)) {
		T edgeID, srcID, dstID;
		char delimiter;
		istringstream iss(line);
		iss >> edgeID >> delimiter >> srcID >> delimiter >> dstID;
		Vertex<T>* src = graph.getVertexByIDMask( node_big_to_small[srcID] );
		Vertex<T>* dst = graph.getVertexByIDMask( node_big_to_small[dstID] );
		if (src != nullptr && dst != nullptr)
			src->addEdge( new Edge<T>(dst, edgeID, calculateDistance(src, dst)) );
	}
	file.close();
}

template<class T>
void loadStreets(Graph<T> &graph) {
	node_big_to_small.clear();
	//Format: edgeID;streetName;isTwoWays;
	string line;
	ifstream file;
	file.open(STREETS_FILE);
	if (!file.is_open()) {
		cout << "Failed to open Streets txt file!\n";
		exit(1);
	}
	while (getline(file, line)) {
		T edgeID;
		string streetName, isTwoWaysStr;
		bool isTwoWays;
		char delimiter;
		string isTwoWayStr;
		istringstream iss(line);
		iss >> edgeID >> delimiter;
		getline(iss, streetName, delimiter);
		iss >> delimiter;
		getline(iss, isTwoWaysStr, '\n');
		isTwoWays = (delimiter == 'T');

		int i = 0;
		for (Vertex<T> * vertex : graph.getVertexSet() ){
			for( pair<long long int , Edge<T> *> p : vertex->getAdjacent() ){
				if (p.second->getID() == edgeID){
					i++;
					Edge<T> *ed = p.second;
					ed->setName(streetName + to_string(i));
					ed->setTwoWays(isTwoWays);
					ed->setSourc(vertex);
					string triename = streetName + to_string(i);
					transform(triename.begin(), triename.end(), triename.begin(), ::toupper);
					graph.insertNameToEdge(triename, ed);
					graph.insertWordToTrie(triename);
					if (isTwoWays) {
						Edge<T>* oppositeEdge = new Edge<T>(vertex, (-1 * ed->getID()), calculateDistance(vertex, ed->getDest()));
						oppositeEdge->setName(streetName + to_string(i)+"B");
						ed->getDest()->addEdge(oppositeEdge);
						triename += "B";
						graph.insertNameToEdge(triename, oppositeEdge);
						graph.insertWordToTrie(triename);
					}
				}
			}
		}
	}
	file.close();
}

/*!
 *	Calculates the distance between two points on earth (in m) using Haversine's formula
 */
template<class T>
int calculateDistance(Vertex<T> *v1, Vertex<T> *v2) {
	double earthRadius = 6371;
	double deltaLatitude = v2->getLatitude() - v1->getLatitude();
	double deltaLongitude = v2->getLongitude() - v1->getLongitude();

	double a = sin(deltaLatitude / 2) * sin(deltaLatitude / 2)
			+ cos(v1->getLatitude()) * cos(v2->getLatitude())
					* sin(deltaLongitude / 2) * sin(deltaLongitude / 2);
	double b = 2 * atan2(sqrt(a), sqrt(1 - a));
	double c = earthRadius * b;
	return abs(c*1000);
}


#endif // UTILITIES_H
