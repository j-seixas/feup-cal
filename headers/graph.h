#ifndef GRAPH_H
#define GRAPH_H

#include "../graph_viewer/graphviewer.h"
#include "../headers/trie.h"
#include <vector>
#include <unordered_map>
#include <map>
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
	@var resolved Whether the vertex was already solved or not (used for graphviewer purposes) if(resolved) color=ORANGE
	@var visited Used for the dfs like visit
	@var reachable Whether the vertex is reachable from the start node or not (start node is the origin node of cut edge)
	@var process Used for the A* algorithm, if vertex was processed it is in the closed list
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

	Vertex* path;

	friend class Graph<T> ;
};

/**
	@brief Class Edge (represents a road)
	@var dest Vertex to which the Edge points
	@var weight Length of the road (in m)
	@var ID Unique ID of the Edge
	@var max_number_cars Maximum number of cars allowed to traverse this road
	@var curr_number_cars Current number of cars traversing this road
	@var streetName Name of the street
	@var isTwoWays Whether the road is two ways or not
	@var is_cut Whether the road is cut or not (if a road is cut it cannor be traversed)
	@var name_mask Mask used to easily reference the Edge
	@var graph_ID ID of this Edge in the graphviewer
	@var is_path Whether this edge is a path to some destination or not (used for graphviewer purposes)
*/
template<class T>
class Edge {
	Vertex<T> *sourc;
	Vertex<T> * dest;
	unsigned int weight; //its in m
	T ID;
	const unsigned int max_number_cars;
	unsigned int curr_number_cars = 0;
	string streetName;
	bool isTwoWays;
	bool is_cut;

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
	inline void setTwoWays(bool b) {isTwoWays = b;}
	inline void setGraphID(int x) {this->graph_ID = x;}
	inline void setSourc(Vertex<T> *sourc) {this->sourc = sourc;}

	inline int getGraphID() {return this->graph_ID;}
	inline T getID() const {return this->ID;}
	inline string getName() const {return this->streetName;}
	inline bool getTwoWays() const {return this->isTwoWays;}
	inline unsigned int getMaxCars() const {return this->max_number_cars;}
	inline unsigned int getWeight() const {return this->weight;}
	inline Vertex<T>* getDest() {return this->dest;}
	inline Vertex<T>* getSourc() {return this->sourc;}

	inline bool operator<(const Edge<T> e) {return this->ID < e.ID;}

	friend class Graph<T> ;
	friend class Vertex<T> ;
};

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
/**
	@brief Graph class, represents a map
	@var vertexSet All vertexes of the graph
	@var cars_destination List which represents the various cars in the closed road
	@var counter Used to give a unique id_mask to vertexes
*/
template<class T>
class Graph {
	unordered_set<Vertex<T> *,hashFuncs,hashFuncs> vertexSet;
	map<string, Edge<T>*> nameToEdge;
	Trie *trie;
	list<Vertex<T> *> cars_destination;
	unsigned long int counter = 0;

public:
	Graph() { this->trie = new Trie; }
	void addVertex(Vertex<T> *v);
	bool addEdge(const T &sourc, const T &dest, int w);
	bool addEdge(Edge<T>* edge, Vertex<T>* from);
	bool removeEdge(const T &sourc, const T &dest);
	inline void initializeSet(const unsigned int size) {this->vertexSet = unordered_set<Vertex<T> *,hashFuncs,hashFuncs>(size);}

	inline unordered_set<Vertex<T> *,hashFuncs,hashFuncs> &getVertexSet() {return this->vertexSet;}
	inline int getNumVertex() const {return this->vertexSet.size();}
	inline unsigned long int getCounter() const {return this->counter;}
	inline list<Vertex<T> *> &getCarsDest() {return this->cars_destination;}
	inline void insertWordToTrie(string &word) {this->trie->insertWord(word);}
	inline bool exactWordSearch(string &word) const {return this->trie->exactWordSearch(word);}
	inline list<string>* approximateWordSearch(string &word) const { return this->trie->approximateWordSearch(word); }
	inline void insertNameToEdge(const string &word, Edge<T> *ptr) { this->nameToEdge.insert(std::pair< string,Edge<T>* >(word, ptr)); }
	Vertex<T>* getVertexByIDMask(long long int id) const;


	void updatePath( Vertex<T> *v);
	void resetAlgorithmVars();
	void generateCarPaths( Vertex<T> *v, unsigned long int &n_nodes);
	void Astar(Vertex<T> *sourc, Vertex<T> *dest,const unsigned long int NODES_LIMIT);
	bool moveCar(Edge<T> &from, long long int &car, Edge<T> &to);
	Vertex<T> * cutStreet(string &streetName, unsigned long int &n_nodes);
	void initDestinations();
	void initializeGraphViewer(GraphViewer *gv) const;
	void updateGraphViewer(GraphViewer *gv) const;
	void resetGraph();
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
Vertex<T> * Graph<T>::cutStreet(string &streetName, unsigned long int &n_nodes) {
	auto it = this->nameToEdge.find(streetName);
	if ( it != this->nameToEdge.end() ) { //Edge found
		this->resetAlgorithmVars();
		this->generateCarPaths(it->second->dest, n_nodes);
		it->second->cutRoad();
		return it->second->sourc;
	}

	return NULL;
}

/**
	@brief Resets the graph to its original state
	@detail Time Complexity O(V+E) , Space Complexity O(1)
*/
template<class T>
void Graph<T>::resetGraph(){
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
			string label = ( p.second->getName() + " " + to_string(p.second->curr_number_cars) + "/" + to_string(p.second->getMaxCars()) + " " + to_string(p.second->getWeight()) + "m. ");
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
			string label = ( edge->getName() + " " + to_string(edge->curr_number_cars) + "/" + to_string(edge->getMaxCars()) + " " + to_string(edge->getWeight()) + "m. ");
			gv->setEdgeLabel(edge->getGraphID(), label);
			if (edge->isPath()){
				gv->setEdgeThickness(edge->getGraphID(),12);
				gv->setEdgeColor(edge->getGraphID(),ORANGE);
				edge->setPath(false);
			}
			else if (edge->isCut() ){
				gv->setVertexColor(edge->dest->getIDMask() , RED);
				gv->setVertexColor(node->getIDMask() , ORANGE);
				gv->setEdgeColor( edge->getGraphID() , RED);
				gv->setEdgeThickness(edge->getGraphID() , 15);
			}
			else{
				gv->setEdgeColor(edge->getGraphID(), ( edge->isFull() ) ? YELLOW : GREEN);
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
	while( src != NULL){
		Edge<T> * edge = src->adjacent[dest->id_mask];
		edge->curr_number_cars++;
		edge->setPath(true);
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
	@param id id_mask of the vertex
	@return Vertex specified
	@detail Time Complexity O(1) , Space Complexity O(1)
*/
template <class T>
Vertex<T>* Graph<T>::getVertexByIDMask(long long int id) const {
	Vertex<T> * temp = new Vertex<T>(id);
	auto ret = this->vertexSet.find( temp );
	delete temp;
	return *ret;
}

/**
	@brief Searches the graph for a path using A*
	@param sourc Pointer to start node
	@param dest Pointer to end node
	@param NODES_LIMIT Limits the number of nodes to explore (should be equal to number of nodes reachable from start)
	@detail Time Complexity O( (V+E)*log(V) ), Space Complexity ( 2*V )
	@detail Algorithm based on http://web.mit.edu/eranki/www/tutorials/search/
	@detail Time Complexity based on https://stackoverflow.com/questions/11070248/a-time-complexity#comment41548859_11070326
*/
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
