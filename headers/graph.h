#ifndef GRAPH_H
#define GRAPH_H

#include "../graph_viewer/graphviewer.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <climits>
#include <algorithm>
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

/** 
	@brief Class Vertex 
	@var ID Id used to identify the map (gotten from the files)
	@var id_mask Id_mask that is used by the program (assigned at the loading stage, starts from 0 and increments)
	@var latitudeRadians Latitude of the vertex
	@var longitudeRadians Longitude of the vertex
	@var adjacent Hash map where key is id_mask of destination Vertex, value is the Edge
	@var dist Used for the A* algorithm
	@var visited Used for the dfs like visit
 */
template<class T>
class Vertex {
	T ID;
	long long int id_mask;
	double latitudeRadians;
	double longitudeRadians;
	unordered_map<long long int,Edge<T>*> adjacent;

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
	inline unordered_map<long long int,Edge<T>*> &getAdjacent() { return adjacent; }
	inline int getDist() const { return dist; }
	Edge<T> * getEdgeDestTo( Vertex<T> * dest) const;


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
	Edge<T> *edgeD = new Edge<T>(dest, w);
	adjacent.insert(pair<long long int , Edge<T>* > (edgeD->dest->id_mask , edgeD) );
}

template<class T>
void Vertex<T>::addEdgeID(Vertex<T> *dest, const T &id) {
	int w = calculateDistance(this, dest);
	Edge<T> *edgeD = new Edge<T>(dest, id, w);
	adjacent.insert(pair<long long int , Edge<T>* > (edgeD->ID , edgeD) );
}

template<class T>
void Vertex<T>::addEdge(Edge<T> *edge) {
	adjacent.insert(pair<long long int , Edge<T> *>(edge->dest->id_mask , edge));
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
	Vertex<T> * sourc;
	Vertex<T> * dest;
	unsigned int weight; //its in m
	T ID;
	const unsigned int max_number_cars;
	unsigned int curr_number_cars = 0;
	string streetName;
	bool isTwoWays;
	bool is_cut;

	string name_mask;
public:
	Edge(Vertex<T> * s, Vertex<T> *d, T id, int w);
	Edge() : dest(NULL), weight(0) , ID(0) , max_number_cars(0){};

	inline void cutRoad() {is_cut = true;}
	inline bool isFull() const {return this->curr_number_cars == this->max_number_cars;}
	inline bool isCut() const {return is_cut;}

	inline void setName(string s) {streetName = s;}
	inline void setNameMask(string s) {name_mask = s;}
	inline void setTwoWays(bool b) {isTwoWays = b;}

	inline T getID() const {return ID;}
	inline string getName() const {return this->streetName;}
	inline bool getTwoWays() const {return this->isTwoWays;}
	inline unsigned int getMaxCars() const {return this->max_number_cars;}
	inline unsigned int getWeight() const {return this->weight;}
	inline Vertex<T>* getDest() {return this->dest;}
	inline string getNameMask() const {return this->name_mask;}

	inline bool operator<(const Edge<T> e) {return this->ID < e.ID;}

	friend class Graph<T> ;
	friend class Vertex<T> ;
};

template<class T>
Edge<T>::Edge(Vertex<T> *sourc , Vertex<T> *d, T id, int w) :
		sourc(sourc), dest(d), weight(w), ID(id), max_number_cars(rand() % 25 + 25), isTwoWays(true), is_cut(false) { }

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
	inline list<Vertex<T> *> &getCarsDest() {return this->cars_destination;}
	Vertex<T>* getVertexByID(const T &v) const;
	Vertex<T>* getVertexByIDMask(long long int id) const;


	void updatePath( Vertex<T> *v);
	void resetAlgorithmVars();
	void generateCarPaths( Vertex<T> *v);
	void Astar(Vertex<T> *sourc, Vertex<T> *dest);
	bool moveCar(Edge<T> &from, long long int &car, Edge<T> &to);
	Vertex<T> * cutStreet(string streetName);
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
	if ( (rand() % 10) == 1 ){
		this->cars_destination.push_back(v);
	}
	for (pair<long long int , Edge<T> *> p : v->adjacent){
	    if ( p.second->dest->visited == false )
	    	generateCarPaths(p.second->dest);
	}
}

/**
	@brief Cuts the designed street
	@param streetName Name of the street to cut
	@return Origin Vertex of the Street or NULL if the street was not found
*/
template<class T>
Vertex<T> * Graph<T>::cutStreet(string streetName) {
	for (Vertex<T> * vertex : this->vertexSet) {
		for(pair<long long int , Edge<T>* > p : vertex->adjacent){
			if (p.second->getNameMask() == streetName) {
				this->resetAlgorithmVars();
				this->generateCarPaths(p.second->dest);
				p.second->cutRoad();
				this->show_name = false;
				return vertex;
			}
		}
	}
	return NULL;
}

template<class T>
void Graph<T>::updateGraphViewer(GraphViewer *gv) const {
	int ID = 0;
	for (Vertex<T> * it : this->vertexSet) {
		pair<int, int> position = calculatePosition(it);
		gv->addNode(it->getIDMask(), position.first, position.second);
	}
	for (Vertex<T> *v_it : this->vertexSet){
		for (pair<long long int, Edge<T>* > e_it : v_it->getAdjacent()) {
			string label = ( (this->show_name) ? (e_it.second->getNameMask() + " ") : "" ) + to_string(e_it.second->curr_number_cars) + "/" + to_string(e_it.second->getMaxCars()) + " " + to_string(e_it.second->getWeight()) + "m. ";
			gv->addEdge(ID, v_it->getIDMask(), (e_it.second->dest)->getIDMask(), EdgeType::DIRECTED);
			gv->setEdgeLabel(ID, label);
			gv->setEdgeThickness(ID, (e_it.second->curr_number_cars/e_it.second->max_number_cars)*10 + 1);
			if (e_it.second->isFull())
				gv->setEdgeColor(ID, "yellow");
			else if (e_it.second->isCut())
				gv->setEdgeColor(ID, "red");
			ID++;
		}
	}
	gv->rearrange();
}

/**
	@brief Updates the number of cars of the path leading to vertex
	@param v Pointer to destination vertex (member variable path gives the path to that vertex)
*/
template<class T>
void Graph<T>::updatePath( Vertex<T> *v){
	Vertex<T> *src = v->path;
	Vertex<T> *dest = v;
	while( src != NULL){
		cout << "Updating edge " << src->id_mask << "->" << dest->id_mask << "\n";
		(src->adjacent[dest->id_mask])->curr_number_cars++;
		dest = src;
		src = src->path;
	}
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
	for (Vertex<T> * v : this->vertexSet) {
		v->path = NULL;
		v->dist = INT_INFINITY;
	}
	const unsigned int MAX_EXPLORE_DEPTH = this->vertexSet.size() * 0.75;
	sourc->dist = 0;
	list<Vertex<T> > closed_list;
	vector<Vertex<T> > open_list;
	open_list.reserve(MAX_EXPLORE_DEPTH + 1);
	open_list.push_back(*sourc);
	while ( !open_list.empty() ){
		if(closed_list.size() >= MAX_EXPLORE_DEPTH ){ //if no path was found
			dest->path = NULL;
			return;
		}
		make_heap( open_list.begin() , open_list.end() , [] (Vertex<T> v1 , Vertex<T> v2) {
			return v1.getDist() > v2.getDist(); //to make heap minimum to max
		} );
		Vertex<T> curr = open_list.front();  open_list.erase(open_list.begin());
		//cout << "VERTEX = " << curr.getIDMask() << endl;
		for ( pair<long long int,Edge<T>*> p : curr.adjacent){
			Edge<T> *edge = p.second;
			Vertex<T> *adjacent = edge->dest;
			//cout << "Processing, " << curr.getIDMask() << " -> " <<  edge->dest->getIDMask() << endl;
			if (edge->curr_number_cars == edge->max_number_cars || edge->isCut()){ //ignore if street full
				//cout << "	Road full/cut\n"	;
				continue;
			}
			
			if (adjacent->getIDMask() == dest->getIDMask()){
				cout << "		!SUCCESS! FOUND SOLUTION\n";
				adjacent->path = &curr;
				return;
			}
			
			adjacent->dist = curr.dist + edge->weight + calculateDistance(adjacent,dest);
			//cout << "	Total = " << adjacent->dist << "	F = " << curr.dist + edge->weight << "	H = " << calculateDistance(adjacent,dest) << "\n";
			for (Vertex<T> temp : open_list)
				if ((temp.getIDMask() == adjacent->getIDMask()) && (temp.dist <= adjacent->dist)){
					//cout << "	smaller in openlist \n";
					continue;
				}
			
			for (Vertex<T> temp : closed_list)
				if ((temp.getIDMask() == adjacent->getIDMask()) && (temp.dist <= adjacent->dist)){
					//cout << "	smaller in closedlist \n";
					continue;
				}
			
			//cout << "FINISED PROCESSING\n";
			open_list.push_back(*adjacent);
			//cout << "[ ";
			//for (Vertex<T> temp : open_list)
				//cout << temp.getIDMask() << ", ";
			//cout << " ]\n[ ";
			//for(Vertex<T> temp : closed_list)
				//cout << temp.getIDMask() << ", ";
			//cout << " ]\n";
			adjacent->path = &curr;
		}
		closed_list.push_back(curr);
	}
}

#endif /* GRAPH_H */
