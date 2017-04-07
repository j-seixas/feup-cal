#ifndef GRAPH_H
#define GRAPH_H

#include "../graph_viewer/graphviewer.h"
#include <vector>
#include <queue>
#include <list>
#include <map>
#include <climits>
#include <string>
#include <iostream>
#include <cmath>
using namespace std;

typedef unsigned long long int uint64;

template<class T> class Edge;
template<class T> class Graph;
template<class T> class Vertex;
//TODO this function should be in Graph, no need to go to utilities.cpp
uint64 nextInteger();
template<class T> pair<int, int> calculatePosition(Vertex<T>* v);

const int INT_INFINITY = INT_MAX;

/*
 * ================================================================================================
 * Class Vertex
 * ================================================================================================
 */
template<class T>
class Vertex {
	/*!
	 *	Vertex information
	 */
	T ID;
	uint64 id_mask;
	double latitudeRadians;
	double longitudeRadians;
	vector<Edge<T>> adjacent;
	/*!
	 *	Reserved for algorithms
	 */
	int dist;
public:
	Vertex(T in, double latRad, double longRad) :
		 ID(in), latitudeRadians(latRad), longitudeRadians(longRad), dist(0) , path(NULL){}
	void addEdge(Vertex<T> *dest, int w);
	void addEdge(Edge<T> *edge);
	void addEdgeID(Vertex<T> *dest, const T &id);
	bool removeEdgeTo(Vertex<T> *d);

	inline T getID() const { return ID; }
	inline uint64 getIDMask() {return this->id_mask;}
	inline uint64 getMaskID() const { return id_mask; }
	inline double getLatitude() const { return latitudeRadians; }
	inline double getLongitude() const { return longitudeRadians; }
	inline vector<Edge<T>>& getAdjacent() { return adjacent; }
	inline int getDist() const { return dist; }

	void setInfo(T id) { ID = id; }
	void setMaskID(uint64 mask) { id_mask = mask; }
	list<Vertex<T> *> backtrace();

	bool operator<(const Vertex<T> vertex);

	Vertex* path;

	friend class Graph<T> ;
};

template<class T>
struct vertex_greater_than {
	bool operator()(Vertex<T> * a, Vertex<T> * b) const {
		return a->getDist() > b->getDist();
	}
};

template<class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d) {
	typename vector<Edge<T> >::iterator it = adjacent.begin();
	typename vector<Edge<T> >::iterator ite = adjacent.end();
	while (it != ite) {
		if (it->dest == d) {
			adjacent.erase(it);
			return true;
		} else
			it++;
	}
	return false;
}

template<class T>
void Vertex<T>::addEdge(Vertex<T> *dest, int w) {
	Edge<T> edgeD(dest, w);
	adjacent.push_back(edgeD);
}

template<class T>
void Vertex<T>::addEdgeID(Vertex<T> *dest, const T &id) {
	int w = calculateDistance(this, dest);
	Edge<T> edgeD(dest, id, w);
	adjacent.push_back(edgeD);
}

template<class T>
void Vertex<T>::addEdge(Edge<T> *edge) {
	adjacent.push_back(*edge);
}

template <class T>
list<Vertex<T> *> Vertex<T>::backtrace(){
	list<Vertex<T> *> temp;
	Vertex<T> * curr = this->path;
	temp.push_front(this);
	while (curr != NULL){
		temp.push_front(curr);
		curr = curr->path;
	}

	return temp;
}

/* ================================================================================================
 * Class Edge
 * ================================================================================================
 */
template<class T>
class Edge {
	Vertex<T> * dest;
	unsigned int weight; //its in m
	T ID;
	const unsigned int max_number_cars;
	vector<uint64> car_dest; //Vector of mask_ids of the destination vertices
	string streetName;
	bool isTwoWays;
	bool is_cut;

	string name_mask;
public:
	Edge(Vertex<T> *d, T id, int w);

	void cutRoad() {is_cut = true;}
	bool isFull() const;
	bool isCut() const {return is_cut;}

	void setName(string s) {streetName = s;}
	void setNameMask(string s) {name_mask = s;}
	void setTwoWays(bool b) {isTwoWays = b;}

	T getID() const {return ID;}
	string getName() const {return this->streetName;}
	bool getTwoWays() const {return this->isTwoWays;}
	unsigned int getMaxCars() const {return this->max_number_cars;}
	unsigned int getWeight() const {return this->weight;}
	Vertex<T>* getDest() {return this->dest;}
	string getNameMask() const {return this->name_mask;}

	friend class Graph<T> ;
	friend class Vertex<T> ;
};

template<class T>
Edge<T>::Edge(Vertex<T> *d, T id, int w) :
		dest(d), weight(w), ID(id), max_number_cars(rand() % 25 + 25), isTwoWays(
				true), is_cut(false) {
}

template<class T>
bool Edge<T>::isFull() const {
	return car_dest.size() == this->max_number_cars;
}

/* ================================================================================================
 * Class Graph
 * ================================================================================================
 */
template<class T>
class Graph {
	vector<Vertex<T> *> vertexSet;
public:
	bool addVertex(const T &in);
	bool addVertex(Vertex<T> *v);
	bool addEdge(const T &sourc, const T &dest, int w);
	bool addEdge(Edge<T>* edge, Vertex<T>* from);
	bool removeVertex(const T &in);
	bool removeEdge(const T &sourc, const T &dest);
	
	inline vector<Vertex<T> *> getVertexSet() const {return this->vertexSet;}
	inline int getNumVertex() const {return this->vertexSet.size();}
	Vertex<T>* getVertexByID(const T &v) const;
	Vertex<T>* getVertexByIDMask(const T &v) const;

	void Astar(Vertex<T> *sourc, Vertex<T> *dest);
	bool moveCar(Edge<T> &from, uint64 &car, Edge<T> &to);
	bool cutStreet(string streetName);
	void initDestinations();
	void updateGraphViewer(GraphViewer *gv) const;
};

template<class T>
bool Graph<T>::moveCar(Edge<T> &from, uint64 &car, Edge<T> &to){
	if(to.isFull())
		return false;
	auto it = find(from.car_dest.begin(), from.car_dest.end());
	if(it == from.car_dest.end())
		return false;
	from.car_dest.erase(it);
	to.car_dest.push_back(car);
}

template<class T>
void Graph<T>::initDestinations() {
	for (uint64 i = 0; i < vertexSet.size(); i++) {
		Vertex<T> *v = vertexSet[i];
		for (uint64 j = 0; j < v->adjacent.size(); j++) {
			Edge<T> *e = &(v->adjacent[j]);
			unsigned int max = e->max_number_cars;
			int num_cars = rand() % max;
			for(int k = 0; k < num_cars; k++){
				uint64 dest_id_mask = rand() % vertexSet.size();
				e->car_dest.push_back(dest_id_mask);
			}
		}
	}
}

template<class T>
bool Graph<T>::cutStreet(string streetName) {
	for (uint64 i = 0; i < this->vertexSet.size(); i++) {
		for (uint64 j = 0; j < this->vertexSet[i]->adjacent.size(); j++) {
			if (this->vertexSet[i]->adjacent[j].getNameMask() == streetName) {
				this->vertexSet[i]->adjacent[j].cutRoad();
				return true;
			}
		}
	}
	return false;
}

template<class T>
void Graph<T>::updateGraphViewer(GraphViewer *gv) const {
	int ID = 0;
	for (Vertex<T> * it : this->vertexSet) {
		pair<int, int> position = calculatePosition(it);
		gv->addNode(it->getMaskID(), position.first, position.second);
	}
	for (Vertex<T> *v_it : this->vertexSet)
		for (Edge<T> e_it : v_it->getAdjacent()) {
			gv->addEdge(ID, v_it->getMaskID(), (e_it.dest)->getMaskID(), EdgeType::DIRECTED);
			gv->setEdgeLabel(ID,to_string(e_it.getWeight()) + "m. " + e_it.getNameMask() + " " + to_string(e_it.car_dest.size()) + "/" + to_string(e_it.getMaxCars()) );
			if (e_it.isFull())
				gv->setEdgeColor(ID, "yellow");
			if (e_it.isCut())
				gv->setEdgeColor(ID, "red");
			ID++;
		}
	gv->rearrange();
}

template<class T>
bool Graph<T>::addVertex(Vertex<T> *v) {
	if(getVertexByID(v->ID) != nullptr)
		return false;
	v->id_mask = nextInteger();
	vertexSet.push_back(v);
	return true;
}

template<class T>
bool Graph<T>::removeVertex(const T &in) {
	typename vector<Vertex<T>*>::iterator it = vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite = vertexSet.end();
	for (; it != ite; it++) {
		if ((*it)->ID == in) {
			Vertex<T> * v = *it;
			vertexSet.erase(it);
			typename vector<Vertex<T>*>::iterator it1 = vertexSet.begin();
			typename vector<Vertex<T>*>::iterator it1e = vertexSet.end();
			for (; it1 != it1e; it1++) {
				(*it1)->removeEdgeTo(v);
			}

			delete v;
			return true;
		}
	}
	return false;
}

template<class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, int w) {
	typename vector<Vertex<T>*>::iterator it = vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite = vertexSet.end();
	int found = 0;
	Vertex<T> *vS, *vD;
	while (found != 2 && it != ite) {
		if ((*it)->ID == sourc) {
			vS = *it;
			found++;
		}
		if ((*it)->ID == dest) {
			vD = *it;
			found++;
		}
		it++;
	}
	if (found != 2)
		return false;
	vS->addEdge(vD, w);

	return true;
}

template<class T>
bool Graph<T>::addEdge(Edge<T>* edge, Vertex<T>* source) {
	source->addEdge(edge);
	return true;
}

template<class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
	typename vector<Vertex<T>*>::iterator it = vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite = vertexSet.end();
	int found = 0;
	Vertex<T> *vS, *vD;
	while (found != 2 && it != ite) {
		if ((*it)->ID == sourc) {
			vS = *it;
			found++;
		}
		if ((*it)->ID == dest) {
			vD = *it;
			found++;
		}
		it++;
	}
	if (found != 2)
		return false;

	return vS->removeEdgeTo(vD);
}

template <class T>
Vertex<T>* Graph<T>::getVertexByID(const T &v) const {
	for(unsigned int i = 0; i < vertexSet.size(); i++)
		if (vertexSet[i]->ID == v) 
			return vertexSet[i];
	return nullptr;
}

template <class T>
Vertex<T>* Graph<T>::getVertexByIDMask(const T &v) const {
	for(unsigned int i = 0; i < vertexSet.size(); i++)
		if (vertexSet[i]->id_mask == v) return vertexSet[i];
	return nullptr;
}

template<class T>
void Graph<T>::Astar(Vertex<T> *sourc, Vertex<T> *dest) {
	for (unsigned int i = 0; i < vertexSet.size(); i++) {
		vertexSet[i]->path = NULL;
		vertexSet[i]->dist = INT_INFINITY;
	}
	sourc->dist = 0;
	list<Vertex<T> *> closed_list;
	vector<Vertex<T> *> open_list;
	open_list.push_back(sourc);
	while ( !open_list.empty() ){
		make_heap( open_list.begin() , open_list.end() , [&] (Vertex<T> *v1 , Vertex<T> *v2) {
			return v1->getID() < v2->getID();
		} );
		Vertex<T> *curr = open_list.front();  open_list.pop_back();
		cout << "Processing v=" << curr->getIDMask() << endl;
		for ( Edge<T> edge : curr->adjacent){
			Vertex<T> *adjacent = edge.dest;
			if (adjacent->getID() == dest->getID()){
				adjacent->path = curr;
				return;
			}
			//cout << "Adj (" << adjacent->getIDMask() << ") " << adjacent->dist << endl;
			//cout << "Curr dist = " << curr->dist << ", edge w = " << edge.weight*1000 << " , heuristic = " << calculateDistance(adjacent,dest)*1000 << endl;
			adjacent->dist = curr->dist + edge.weight*1000 + calculateDistance(adjacent,dest)*1000;
			//cout << " ,after " << adjacent->dist << endl;
			//TODO can be optimized to just one cycle (with more branches)
			for (Vertex<T> *temp : open_list)
				if ((temp->getID() == adjacent->getID())
						&& (temp->dist < adjacent->dist))
					continue;
			for (Vertex<T> *temp : closed_list)
				if ((temp->getID() == adjacent->getID())
						&& (temp->dist < adjacent->dist))
					continue;
			open_list.push_back(adjacent);
			cout << "	PATH = " << curr->getIDMask() << endl;
			adjacent->path = curr;
		}
		closed_list.push_back(curr);
	}
}

#endif /* GRAPH_H */
