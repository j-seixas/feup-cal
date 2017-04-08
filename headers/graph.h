#ifndef GRAPH_H
#define GRAPH_H

#include "../graph_viewer/graphviewer.h"
#include <vector>
#include <map>
#include <unordered_set>
#include <list>
#include <climits>
#include <string>
#include <iostream>
using namespace std;

template<class T> class Edge;
template<class T> class Graph;
template<class T> class Vertex;
//TODO this function should be in Graph, no need to go to utilities.cpp
long long int nextInteger();
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
	long long int id_mask;
	double latitudeRadians;
	double longitudeRadians;
	map<long long int,Edge<T>> adjacent;
	/*!
	 *	Reserved for algorithms
	 */
	int dist;
	bool visited = false;
public:
	Vertex(T in, double latRad, double longRad) :
		 ID(in), latitudeRadians(latRad), longitudeRadians(longRad), dist(0) , path(NULL){};
 	Vertex(long long int id_mask) : id_mask(id_mask) {};
	void addEdge(Vertex<T> *dest, int w);
	void addEdge(Edge<T> *edge);
	void addEdgeID(Vertex<T> *dest, const T &id);

	inline T getID() const { return ID; }
	inline long long int getIDMask() const {return this->id_mask;}
	inline double getLatitude() const { return latitudeRadians; }
	inline double getLongitude() const { return longitudeRadians; }
	inline map<long long int,Edge<T>> &getAdjacent() { return adjacent; }
	inline int getDist() const { return dist; }

	void setInfo(T id) { ID = id; }
	void setMaskID(long long int mask) { id_mask = mask; }
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
void Vertex<T>::addEdge(Vertex<T> *dest, int w) {
	Edge<T> edgeD(dest, w);
	adjacent.insert(pair<long long int , Edge<T> > (edgeD.ID , edgeD) );
}

template<class T>
void Vertex<T>::addEdgeID(Vertex<T> *dest, const T &id) {
	int w = calculateDistance(this, dest);
	Edge<T> edgeD(dest, id, w);
	adjacent.push_back(edgeD);
}

template<class T>
void Vertex<T>::addEdge(Edge<T> *edge) {
	adjacent.insert(pair<long long int , Edge<T> >(edge->ID , *edge));
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
	unsigned int curr_number_cars = 0;
	vector<long long int> car_dest; //Vector of mask_ids of the destination vertices
	string streetName;
	bool isTwoWays;
	bool is_cut;

	string name_mask;
public:
	Edge(Vertex<T> *d, T id, int w);
	Edge() : dest(NULL), weight(0) , ID(0) , max_number_cars(0){};

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

	bool operator<(const Edge<T> e) {return this->ID < e.ID;}

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

struct hashFuncs{
	template<class T>
	size_t operator()(const Vertex<T> *v) const{
		return v->getIDMask();
	}
	template<class T>
	bool operator()(const Vertex<T> *v1 , const Vertex<T> *v2) const{
		return v1->getIDMask() == v2->getIDMask();
	}
};

/* ================================================================================================
 * Class Graph
 * ================================================================================================
 */
template<class T>
class Graph {
	unordered_set<Vertex<T> *,hashFuncs,hashFuncs> vertexSet;
	list<Vertex<T> *> cars_destination;
	unsigned long int counter = 0;
public:
	bool show_name = true;
	bool addVertex(Vertex<T> *v);
	bool addEdge(const T &sourc, const T &dest, int w);
	bool addEdge(Edge<T>* edge, Vertex<T>* from);
	bool removeEdge(const T &sourc, const T &dest);
	inline void initializeSet(const unsigned int size) {this->vertexSet = unordered_set<Vertex<T> *,hashFuncs,hashFuncs>(size);}
	
	inline unordered_set<Vertex<T> *,hashFuncs,hashFuncs> &getVertexSet() {return this->vertexSet;}
	inline int getNumVertex() const {return this->vertexSet.size();}
	inline unsigned long int getCounter() const {return this->counter;}
	Vertex<T>* getVertexByID(const T &v) const;
	Vertex<T>* getVertexByIDMask(long long int id) const;

	void resetAlgorithmVars();
	void generateCarPaths( Vertex<T> *v);
	void Astar(Vertex<T> *sourc, Vertex<T> *dest);
	bool moveCar(Edge<T> &from, long long int &car, Edge<T> &to);
	bool cutStreet(string streetName);
	void initDestinations();
	void updateGraphViewer(GraphViewer *gv) const;
};

template<class T>
void Graph<T>::resetAlgorithmVars(){
	for (Vertex<T> * v : this->vertexSet){
		v->visited = false;
	}
}

//basically a dfs to generate paths beyond the road which was cut
template <class T>
void Graph<T>::generateCarPaths(Vertex<T> *v) {
	v->visited = true;
	if ( (rand() % 5) == 0 )
		this->cars_destination.push_back(v);
	for (pair<long long int , Edge<T> > p : v->adjacent)
	    if ( p.second.dest->visited == false )
	    	generateCarPaths(p.second.dest);
}

template<class T>
bool Graph<T>::moveCar(Edge<T> &from, long long int &car, Edge<T> &to){
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
	for (Vertex<T> *v : this->vertexSet){
		for(pair<long long int , Edge<T> > p : v->adjacent){
			Edge<T> *e = &(p.second);
			unsigned int max = e->max_number_cars;
			int num_cars = rand() % max;
			for(int k = 0; k < num_cars; k++){
				long long int dest_id_mask = rand() % vertexSet.size();
				e->car_dest.push_back(dest_id_mask);
			}
		}
	}
}

template<class T>
bool Graph<T>::cutStreet(string streetName) {
	cout << "Cutting street  |" << streetName << "| \n";
	for (Vertex<T> * vertex : this->vertexSet) {
		for(pair<long long int , Edge<T> > p : vertex->adjacent){
			//cout << (p.second.getNameMask() == streetName) << " - |" << p.second.getNameMask() << "| |"
			// 	 << streetName << "|\n";
			if (p.second.getNameMask() == streetName) {
				cout << "	CUTTING " << p.second.getNameMask() << endl;
				this->resetAlgorithmVars();
				this->generateCarPaths(p.second.dest);

				for (Vertex<T> *v : this->cars_destination)
					cout << "ID = " << v->getIDMask() << endl;

				p.second.cutRoad();
				//this->show_name = false;
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
		gv->addNode(it->getIDMask(), position.first, position.second);
	}
	for (Vertex<T> *v_it : this->vertexSet){
		for (pair<long long int, Edge<T> > e_it : v_it->getAdjacent()) {
			string label = ( (this->show_name) ? (e_it.second.getNameMask() + " ") : "" ) + to_string(e_it.second.car_dest.size()) + "/" + to_string(e_it.second.getMaxCars()) + " " + to_string(e_it.second.getWeight()) + "m. ";
			gv->addEdge(ID, v_it->getIDMask(), (e_it.second.dest)->getIDMask(), EdgeType::DIRECTED);
			gv->setEdgeLabel(ID, label);
			if (e_it.second.isFull())
				gv->setEdgeColor(ID, "yellow");
			else if (e_it.second.isCut())
				gv->setEdgeColor(ID, "red");
			ID++;
		}
	}

	cout << "Rearranging" << endl;
	gv->rearrange();
}

//TODO check if there really is a need to check if vertex already exists, if there is a hash_table might
// 	   not be the most appropriate structure
template<class T>
bool Graph<T>::addVertex(Vertex<T> *v) {
	//if(getVertexByID(v->ID) != nullptr)
	//	return false;
	v->id_mask = this->counter++;
	this->vertexSet.insert(v);
	return true;
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

template <class T>
Vertex<T>* Graph<T>::getVertexByID(const T &v) const {
	for(Vertex<T> * vertex : this->vertexSet)
		if (vertex->getID() == v) 
			return vertex;
	return nullptr;
}

template <class T>
Vertex<T>* Graph<T>::getVertexByIDMask(long long int id) const {
	Vertex<T> * temp = new Vertex<T>(id);
	auto ret = this->vertexSet.find( temp );
	delete temp;
	return *ret;
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
		//cout << "Processing v=" << curr->getIDMask() << endl;
		for ( Edge<T> edge : curr->adjacent){
			if (edge.curr_number_cars == edge.max_number_cars) //ignore if street full
				continue;
			Vertex<T> *adjacent = edge.dest;
			if (adjacent->getID() == dest->getID()){
				adjacent->path = curr;
				return;
			}
			
			adjacent->dist = curr->dist + edge.weight*1000 + calculateDistance(adjacent,dest)*1000;
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
