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
	unordered_map<long long int,Edge<T>* > adjacent;

	int dist;
	bool resolved = false;
	bool visited = false;
	bool reachable = true;
	bool process = false;
public:
	Vertex(T in, double latRad, double longRad) : 
		ID(in), latitudeRadians(latRad), longitudeRadians(longRad), dist(0) , path(NULL){};
 	Vertex(long long int id_mask) : id_mask(id_mask) {};

	inline void addEdge(Edge<T> *edge) { this->adjacent.emplace( edge->dest->id_mask , edge ); }

	inline T getID() const { return ID; }
	inline long long int getIDMask() const {return this->id_mask;}
	inline double getLatitude() const { return latitudeRadians; }
	inline double getLongitude() const { return longitudeRadians; }
	inline unordered_map<long long int,Edge<T>*> &getAdjacent() { return adjacent; }
	inline int getDist() const { return dist; }
	inline bool getReachable() const {return this->reachable;}

	inline void setReachable(bool r) {this->reachable = r;}
	inline void setResolved(bool res) {this->resolved = res;}
	inline void setInfo(T id) { ID = id; }
	inline void setMaskID(long long int mask) { id_mask = mask; }
	list<Vertex<T> *> backtrace();

	Vertex* path;

	friend class Graph<T> ;
};

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
	string streetName;
	bool isTwoWays;
	bool is_cut;

	string name_mask;
	int graph_ID;
	bool is_path  = false;
public:
	Edge(Vertex<T> *d, T id, int w) :
		dest(d), weight(w), ID(id), max_number_cars(rand() % 75 + 25), isTwoWays(true), is_cut(false) { }
	Edge() : dest(NULL), weight(0) , ID(0) , max_number_cars(0){};

	inline void cutRoad() {is_cut = true;}
	inline bool isFull() const {return this->curr_number_cars == this->max_number_cars;}
	inline bool isCut() const {return is_cut;}
	inline bool isPath() const {return this->is_path;}

	inline void setPath(bool p) {this->is_path = p;}
	inline void setName(string s) {streetName = s;}
	inline void setNameMask(string s) {name_mask = s;}
	inline void setTwoWays(bool b) {isTwoWays = b;}
	inline void setGraphID(int x) {this->graph_ID = x;}

	inline int getGraphID() {return this->graph_ID;}
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

/* ================================================================================================
 * Class Graph
 * ================================================================================================
 */
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

template<class T>
class Graph {
	unordered_set<Vertex<T> *,hashFuncs,hashFuncs> vertexSet;
	list<Vertex<T> *> cars_destination;
	unsigned long int counter = 0;

public:
	bool show_name = true;
	void addVertex(Vertex<T> *v);
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
	void generateCarPaths( Vertex<T> *v, unsigned long int &n_nodes);
	void Astar(Vertex<T> *sourc, Vertex<T> *dest,const unsigned long int NODES_LIMIT);
	bool moveCar(Edge<T> &from, long long int &car, Edge<T> &to);
	Vertex<T> * cutStreet(string streetName, unsigned long int &n_nodes);
	void initDestinations();
	void initializeGraphViewer(GraphViewer *gv) const;
	void updateGraphViewer(GraphViewer *gv) const;
	void reset();
};

/**
	@brief Resets the variables needed by the algorithms
	@detail Time Complexity O(V) , Space Complexity O(1)
*/
template<class T>
void Graph<T>::resetAlgorithmVars(){
	for (Vertex<T> * v : this->vertexSet){
		v->visited = false;
	}
}

/**
	@brief Generates paths of cars beyond the desired vertex
	@param v Vertex to start generation from
	@detail Time Complexity O(V+E) , Space Complexity O(V)
*/
template <class T>
void Graph<T>::generateCarPaths(Vertex<T> *v, unsigned long int &n_nodes) {
	v->visited = true;
	if ( (rand() % 10) == 1 ){
		this->cars_destination.push_back(v);
	}
	for (pair<long long int , Edge<T> *> p : v->adjacent){
	    if ( p.second->dest->visited == false )
	    	generateCarPaths(p.second->dest , ++n_nodes);
	}
}

/**
	@brief Cuts the designed street
	@param streetName Name of the street to cut
	@return Origin Vertex of the Street or NULL if the street was not found
	@detail Time Complexity O(V+E) , Space Complexity O(1)
*/
template<class T>
Vertex<T> * Graph<T>::cutStreet(string streetName, unsigned long int &n_nodes) {
	for (Vertex<T> * vertex : this->vertexSet) {
		for(pair<long long int , Edge<T>* > p : vertex->adjacent){
			Edge<T> * edge = p.second;
			if (edge->getNameMask() == streetName) {
				this->resetAlgorithmVars();
				this->generateCarPaths(edge->dest, n_nodes);
				edge->cutRoad();
				this->show_name = false;
				return vertex;
			}
		}
	}
	return NULL;
}

/**
	@brief Resets the graph to its original state
	@detail Time Complexity O(V+E) , Space Complexity O(1)
*/
template<class T>
void Graph<T>::reset(){
	this->cars_destination.clear();
	for (Vertex<T> * vertex : this->vertexSet) {
		vertex->dist = INT_MAX;
		vertex->resolved = false;
		vertex->visited = false;
		vertex->reachable = true;
		vertex->process = false;
		vertex->path = nullptr;
		for(pair<long long int , Edge<T>* > p : vertex->adjacent){
			Edge<T> * edge = p.second;
			edge->is_cut = false;
			edge->is_path = false;
			edge->curr_number_cars = 0;
		}
	}
}

/**
	@brief Initializes the nodes and edges of the Graph
	@param gv Pointer to the graphviewer
	@detail Time Complexity O( (V^2) + E) , Space Complexity O(1)
*/
template<class T>
void Graph<T>::initializeGraphViewer(GraphViewer *gv) const {
	int ID = 0;
	for (Vertex<T> * node : this->vertexSet){
		pair<int, int> position = calculatePosition(node);
		gv->addNode(node->getIDMask(), position.first, position.second);
	}
	for (Vertex<T> * node : this->vertexSet) {
		for (pair<long long int, Edge<T>* > p : node->getAdjacent()) {
			string label = ( p.second->getNameMask() + " " + to_string(p.second->curr_number_cars) + "/" + to_string(p.second->getMaxCars()) + " " + to_string(p.second->getWeight()) + "m. ");
			gv->addEdge(ID, node->getIDMask(), (p.second->dest)->getIDMask(), EdgeType::DIRECTED);
			gv->setEdgeLabel(ID, label);
			gv->setEdgeThickness(ID,1);
			p.second->setGraphID(ID);
			ID++;
		}
	}
	gv->rearrange();
}

/**
	@brief Updates the nodes and edges of the Graph
	@param gv Pointer to the graphviewer
	@detail Time Complexity O(V+E) , Space Complexity O(1)
*/
template<class T>
void Graph<T>::updateGraphViewer( GraphViewer *gv) const{
	for (Vertex<T> * node : this->vertexSet){
		gv->setVertexColor(node->id_mask, WHITE);
		for (pair<long long int , Edge<T> *> p : node->adjacent){
			Edge<T> * edge = p.second;
			string label = ( edge->getNameMask() + " " + to_string(edge->curr_number_cars) + "/" + to_string(edge->getMaxCars()) + " " + to_string(edge->getWeight()) + "m. ");
			gv->setEdgeLabel(edge->getGraphID(), label);
			if (edge->isPath()){
				gv->setEdgeThickness(edge->getGraphID(),12);
				gv->setEdgeColor(edge->getGraphID(),YELLOW);
				edge->setPath(false);
			}
			else if (edge->isFull())
				gv->setEdgeColor(edge->getGraphID() , YELLOW);
			else if (edge->isCut() ){
				gv->setVertexColor(edge->dest->getIDMask() , RED);
				gv->setVertexColor(node->getIDMask() , RED);
				gv->setEdgeColor( edge->getGraphID() , RED);
				gv->setEdgeThickness(edge->getGraphID() , 15);
			}
			else{
				gv->setEdgeColor(edge->getGraphID(),GREEN);
				gv->setEdgeThickness(edge->getGraphID() , (((double)edge->curr_number_cars)/((double)edge->max_number_cars))*10 + 1 );
			}

		}
	}
	for (Vertex<T> * v : this->cars_destination)
		gv->setVertexColor(v->getIDMask() , ((v->resolved) ? ORANGE : ((v->reachable) ? BLUE : RED) ) );

	gv->rearrange();
}

/**
	@brief Updates the number of cars of the path leading to vertex
	@param v Pointer to destination vertex (member variable path gives the path to that vertex)
	@detail Time Complexity O(N) , Space Complexity O(1)
*/
template<class T>
void Graph<T>::updatePath( Vertex<T> *v){
	Vertex<T> *src = v->path;
	Vertex<T> *dest = v;
	cout << dest->getIDMask() << " <- ";
	while( src != NULL){
		Edge<T> * edge = src->adjacent[dest->id_mask];
		edge->curr_number_cars++;
		edge->setPath(true);
		cout << src->getIDMask() << " <- ";
		dest->path = NULL;
		dest = src;
		src = src->path;
	}
	cout << endl;
}

/**
	@brief Adds a vertex to the graph
	@param v Vertex to add
	@detail Time Complexity O(1) , Space Complexity O(1);
*/
template<class T>
void Graph<T>::addVertex(Vertex<T> *v) {
	v->id_mask = this->counter++;
	this->vertexSet.insert(v);
}

/**
	@brief Gets the designated vertex
	@param id iID of the vertex
	@return Vertex specified
	@detail Time Complexity O(V) , Space Complexity O(1);
*/
template <class T>
Vertex<T>* Graph<T>::getVertexByID(const T &v) const {
	for(Vertex<T> * vertex : this->vertexSet)
		if (vertex->getID() == v) 
			return vertex;
	return nullptr;
}

/**
	@brief Gets the designated vertex
	@param id id_mask of the vertex
	@return Vertex specified
	@detail Time Complexity O(1) , Space Complexity O(1);
*/
template <class T>
Vertex<T>* Graph<T>::getVertexByIDMask(long long int id) const {
	Vertex<T> * temp = new Vertex<T>(id);
	auto ret = this->vertexSet.find( temp );
	delete temp;
	return *ret;
}

template<class T>
void Graph<T>::Astar(Vertex<T> *sourc, Vertex<T> *dest, const unsigned long int NODES_LIMIT) {
	bool ignore = false;
	for (Vertex<T> * v : this->vertexSet) {
		v->path = NULL; v->dist = INT_INFINITY; v->process = false;
	}
	sourc->dist = 0;
	list<Vertex<T> *> closed_list;
	vector<Vertex<T> *> open_list;
	open_list.push_back(sourc);

	while ( !open_list.empty() ){
		make_heap( open_list.begin() , open_list.end() , [] (Vertex<T> *v1 , Vertex<T> *v2) { return v1->getDist() > v2->getDist(); } );
		Vertex<T> curr = *(open_list.front()) , *curr_ptr = open_list.front();  
		open_list.erase(open_list.begin());
		if (curr.id_mask == dest->id_mask){ //Here to guarantee optimal path
			cout << "	!SUCESS!	\n";
			cout << "	A* explored " << closed_list.size() << " nodes\n";
			return;
		}
		
		for ( pair<long long int,Edge<T>*> p : curr.adjacent){
			Edge<T> *edge = p.second;  Vertex<T> *adjacent = edge->dest;
			if (edge->curr_number_cars == edge->max_number_cars || edge->isCut()) //ignore if street full
				continue;

			int dist = curr_ptr->dist + edge->weight + //G
							 calculateDistance(adjacent,dest); //H
			auto o_it = open_list.begin();
			for ( ; o_it != open_list.end() ; o_it++)
				if ( (*o_it)->id_mask == adjacent->id_mask ){ //if vertex is in open list
					if ( (*o_it)->dist <= dist){
						ignore = true;
						break;
					}
					else{ //current path better than the one in openlist
						(*o_it)->dist = dist;
						(*o_it)->path = curr_ptr;
						break;
					}
				}
			if ( ignore ){ ignore = false; continue; }

			for (Vertex<T> *temp : closed_list)
				if ((temp->id_mask == adjacent->id_mask) && (temp->dist <= dist)){
					ignore = true;
					break;
				}
			if ( ignore ){ ignore = false; continue; }

			if( o_it == open_list.end()){ //node not in open_list
				adjacent->dist = dist;
				open_list.push_back( adjacent );
				adjacent->path = curr_ptr;
			}
		}
		if ( !curr.process){ //if node not yet processed add to closed list 
			curr.process = true; 
			closed_list.push_back(curr_ptr);
		}
		if(closed_list.size() >= NODES_LIMIT ){ //if no path was found
			dest->path = NULL;
			cout << "	A* explored " << closed_list.size() << " nodes\n";
			return;
		}
	}
	cout << "	A* explored " << closed_list.size() << " nodes\n";
}

#endif /* GRAPH_H */
