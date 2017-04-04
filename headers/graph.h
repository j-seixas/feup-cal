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

template <class T> class Edge;
template <class T> class Graph;

const int NOT_VISITED   = 0;
const int BEING_VISITED = 1;
const int DONE_VISITED  = 2;
const int INT_INFINITY  = INT_MAX;

/*
 * ================================================================================================
 * Class Vertex
 * ================================================================================================
 */
template <class T>
class Vertex {
	/*!
	*	Vertex information
	*/
	T ID;
	double latitudeRadians;
	double longitudeRadians;
	vector<Edge<T>> adjacent;

	/*!
	*	Reserved for algorithms
	*/
	bool visited;
	bool processing;
	int indegree;
	int dist;
public:
	Vertex(T in);
	Vertex(const Vertex<T> &v);
	Vertex(T in, double latRad, double longRad);
	friend class Graph<T>;
	void addEdge(Vertex<T> *dest, double w);
	void addEdgeID(Vertex<T> *dest, const T &id);
	bool removeEdgeTo(Vertex<T> *d);

	T getID() const { return ID; }
	double getLatitude() const { return latitudeRadians; }
	double getLongitude() const { return longitudeRadians; }
	vector<Edge<T>>& getAdjacent() { return adjacent; }
	void setInfo(T ID);

	int getDist() const;
	int getIndegree() const;

	bool operator<(const Vertex<T> vertex);

	Vertex* path;//used for backtracing
};


template <class T>
struct vertex_greater_than {
    bool operator()(Vertex<T> * a, Vertex<T> * b) const {
        return a->getDist() > b->getDist();
    }
};


template <class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d) {
	d->indegree--;
	typename vector<Edge<T> >::iterator it= adjacent.begin();
	typename vector<Edge<T> >::iterator ite= adjacent.end();
	while (it!=ite) {
		if (it->dest == d) {
			adjacent.erase(it);
			return true;
		}
		else it++;
	}
	return false;
}

template <class T>
Vertex<T>::Vertex(T in): ID(in), visited(false), processing(false), indegree(0), dist(0) {
	path = NULL;
}

template <class T>
Vertex<T>::Vertex(T in, double latRad, double longRad) : ID(in), latitudeRadians(latRad), longitudeRadians(longRad), visited(false), processing(false), indegree(0), dist(0) {
	path = NULL;
}

template <class T>
Vertex<T>::Vertex(const Vertex<T> &v) : Vertex(v.ID, v.latitudeRadians, v.longitudeRadians) { }

template <class T>
void Vertex<T>::addEdge(Vertex<T> *dest, double w) {
	Edge<T> edgeD(dest,w);
	adjacent.push_back(edgeD);
}

template <class T>
void Vertex<T>::addEdgeID(Vertex<T> *dest, const T &id) {
	double w = calculateDistance(this, dest);
	Edge<T> edgeD(dest, id, w);
	adjacent.push_back(edgeD);
}

template <class T>
int Vertex<T>::getDist() const {
	return this->dist;
}


template <class T>
void Vertex<T>::setInfo(T ID) {
	this->ID = ID;
}

template <class T>
int Vertex<T>::getIndegree() const {
	return this->indegree;
}



/* ================================================================================================
 * Class Edge
 * ================================================================================================
 */
template <class T>
class Edge {
	Vertex<T> * dest;
	double weight;
	T ID;
	string streetName;
	bool isTwoWays;
	
public:
	Edge(Vertex<T> *d, double w);
	Edge(Vertex<T> *d, T id, double w);
	void setName(string s) { streetName = s; }
	void setTwoWays(bool b) { isTwoWays = b; }
	T getID() const { return ID; }
	string getName() const { return streetName; }
	bool getTwoWays() const { return isTwoWays; }
	double getWeight() const { return weight; }
	Vertex<T>* getDest() { return dest; }
	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w){}

template <class T>
Edge<T>::Edge(Vertex<T> *d, T id, double w) : dest(d), weight(w), ID(id) {}



/* ================================================================================================
 * Class Graph
 * ================================================================================================
 */
template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;
	void dfs(Vertex<T> *v, vector<T> &res) const;

	int numCycles;
	void dfsVisit(Vertex<T> *v);
	void dfsVisit();
	void getPathTo(Vertex<T> *origin, list<T> &res);

public:
	bool addVertex(const T &in);
	bool addVertex(Vertex<T> &v);
	bool addEdge(const T &sourc, const T &dest, double w);
	bool addEdgeID(const T &sourc, const T &dest, const T &id);
	bool removeVertex(const T &in);
	bool removeEdge(const T &sourc, const T &dest);
	vector<T> dfs() const;
	vector<T> bfs(Vertex<T> *v) const;
	int maxNewChildren(Vertex<T> *v, T &inf) const;
	vector<Vertex<T> * > getVertexSet() const;
	int getNumVertex() const;

	Vertex<T>* getVertex(const T &v) const;
	void resetIndegrees();
	vector<Vertex<T>*> getSources() const;
	int getNumCycles();
	vector<T> topologicalOrder();
	vector<T> getPath(const T &origin, const T &dest);
	void unweightedShortestPath(const T &v);
	bool isDAG();

	void bellmanFordShortestPath(const T &s);
	void dijkstraShortestPath(const T &s);
	void floydWarshallShortestPath();
	int edgeCost(int vOrigIndex, int vDestIndex);
	vector<T> getfloydWarshallPath(const T &origin, const T &dest);
	void getfloydWarshallPathAux(int index1, int index2, vector<T> & res);

	void Astar(Vertex<T> *sourc , Vertex<T> *dest);

	void showGraph() const;

	map<long long int,long long int> big_to_small = *(new map<long long int,long long int>());
	map<string,string> basic_to_street_name = *(new map<string,string>()); //(A -> Rua J , B -> Rua A)
};

template <class T>
void Graph<T>::showGraph() const{
	int ID = 0;
	GraphViewer *gv = new GraphViewer(1000, 800, true);
	gv->createWindow(1000, 800);
	gv->defineVertexColor("blue");
	gv->defineEdgeColor("black");
	for (Vertex<T> * it : this->vertexSet)
		gv->addNode(it->getID());
	for (Vertex<T> *v_it : this->vertexSet)
		for (Edge<T> e_it : v_it->getAdjacent() ){
			gv->addEdge(ID, v_it->getID() , (e_it.dest)->getID(), EdgeType::DIRECTED);
			gv->setEdgeLabel(ID, e_it.getName());
			gv->setEdgeWeight(ID++, e_it.getWeight()*1000); //to show in meters
		}
	gv->rearrange();
	system("pause");
}

template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}
template <class T>
vector<Vertex<T> * > Graph<T>::getVertexSet() const {
	return vertexSet;
}

template <class T>
int Graph<T>::getNumCycles() {
	numCycles = 0;
	dfsVisit();
	return this->numCycles;
}

template <class T>
bool Graph<T>::isDAG() {
	return (getNumCycles() == 0);
}

template <class T>
bool Graph<T>::addVertex(const T &in) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	for (; it!=ite; it++) //checks if vertex already exists
		if ((*it)->ID == in) return false;
	Vertex<T> *v1 = new Vertex<T>(in);
	vertexSet.push_back(v1);
	return true;
}

template <class T>
bool Graph<T>::addVertex(Vertex<T> &v) {
	typename vector<Vertex<T>*>::iterator it = vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite = vertexSet.end();
	for (; it != ite; it++) //checks if vertex already exists
		if ((*it)->ID == v.getID()) return false;
	Vertex<T> *v1 = new Vertex<T>(v);
	vertexSet.push_back(v1);
	return true;
}

template <class T>
bool Graph<T>::removeVertex(const T &in) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	for (; it!=ite; it++) {
		if ((*it)->ID == in) {
			Vertex<T> * v= *it;
			vertexSet.erase(it);
			typename vector<Vertex<T>*>::iterator it1= vertexSet.begin();
			typename vector<Vertex<T>*>::iterator it1e= vertexSet.end();
			for (; it1!=it1e; it1++) {
				(*it1)->removeEdgeTo(v);
			}

			typename vector<Edge<T> >::iterator itAdj= v->adjacent.begin();
			typename vector<Edge<T> >::iterator itAdje= v->adjacent.end();
			for (; itAdj!=itAdje; itAdj++) {
				itAdj->dest->indegree--;
			}
			delete v;
			return true;
		}
	}
	return false;
}

template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	int found=0;
	Vertex<T> *vS, *vD;
	while (found!=2 && it!=ite ) {
		if ( (*it)->ID == sourc )
			{ vS=*it; found++;}
		if ( (*it)->ID == dest )
			{ vD=*it; found++;}
		it ++;
	}
	if (found!=2) return false;
	vD->indegree++;
	vS->addEdge(vD,w);

	return true;
}

template <class T>
bool Graph<T>::addEdgeID(const T &sourc, const T &dest, const T &id) {
	typename vector<Vertex<T>*>::iterator it = vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite = vertexSet.end();
	int found = 0;
	Vertex<T> *vS = nullptr, *vD = nullptr;
	while (found != 2 && it != ite) {
		if ((*it)->ID == sourc)	{
			vS = *it; found++;
		}
		if ((*it)->ID == dest)	{
			vD = *it; found++;
		}
		it++;
	}
	if (found != 2) return false;
	vD->indegree++;
	vS->addEdgeID(vD, id);

	return true;
}


template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	int found=0;
	Vertex<T> *vS, *vD;
	while (found!=2 && it!=ite ) {
		if ( (*it)->ID == sourc )
			{ vS=*it; found++;}
		if ( (*it)->ID == dest )
			{ vD=*it; found++;}
		it ++;
	}
	if (found!=2)
		return false;

	vD->indegree--;

	return vS->removeEdgeTo(vD);
}


template <class T>
vector<T> Graph<T>::dfs() const {
	typename vector<Vertex<T>*>::const_iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::const_iterator ite= vertexSet.end();
	for (; it !=ite; it++)
		(*it)->visited=false;
	vector<T> res;
	it=vertexSet.begin();
	for (; it !=ite; it++)
	    if ( (*it)->visited==false )
	    	dfs(*it,res);
	return res;
}

template <class T>
void Graph<T>::dfs(Vertex<T> *v,vector<T> &res) const {
	v->visited = true;
	res.push_back(v->ID);
	typename vector<Edge<T> >::iterator it= (v->adjacent).begin();
	typename vector<Edge<T> >::iterator ite= (v->adjacent).end();
	for (; it !=ite; it++)
	    if ( it->dest->visited == false ){
	    	dfs(it->dest, res);
	    }
}

template <class T>
vector<T> Graph<T>::bfs(Vertex<T> *v) const {
	vector<T> res;
	queue<Vertex<T> *> q;
	q.push(v);
	v->visited = true;
	while (!q.empty()) {
		Vertex<T> *v1 = q.front();
		q.pop();
		res.push_back(v1->ID);
		typename vector<Edge<T> >::iterator it=v1->adjacent.begin();
		typename vector<Edge<T> >::iterator ite=v1->adjacent.end();
		for (; it!=ite; it++) {
			Vertex<T> *d = it->dest;
			if (d->visited==false) {
				d->visited=true;
				q.push(d);
			}
		}
	}
	return res;
}

template <class T>
int Graph<T>::maxNewChildren(Vertex<T> *v, T &inf) const {
	vector<T> res;
	queue<Vertex<T> *> q;
	queue<int> level;
	int maxChildren=0;
	inf =v->ID;
	q.push(v);
	level.push(0);
	v->visited = true;
	while (!q.empty()) {
		Vertex<T> *v1 = q.front();
		q.pop();
		res.push_back(v1->ID);
		int l=level.front();
		level.pop(); l++;
		int nChildren=0;
		typename vector<Edge<T> >::iterator it=v1->adjacent.begin();
		typename vector<Edge<T> >::iterator ite=v1->adjacent.end();
		for (; it!=ite; it++) {
			Vertex<T> *d = it->dest;
			if (d->visited==false) {
				d->visited=true;
				q.push(d);
				level.push(l);
				nChildren++;
			}
		}
		if (nChildren>maxChildren) {
			maxChildren=nChildren;
			inf = v1->ID;
		}
	}
	return maxChildren;
}


template <class T>
Vertex<T>* Graph<T>::getVertex(const T &v) const {
	for(unsigned int i = 0; i < vertexSet.size(); i++)
		if (vertexSet[i]->ID == v) return vertexSet[i];
	return NULL;
}

template<class T>
void Graph<T>::resetIndegrees() {
	//colocar todos os indegree em 0;
	for(unsigned int i = 0; i < vertexSet.size(); i++) vertexSet[i]->indegree = 0;

	//actualizar os indegree
	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		//percorrer o vector de Edges, e actualizar indegree
		for(unsigned int j = 0; j < vertexSet[i]->adjacent.size(); j++) {
			vertexSet[i]->adjacent[j].dest->indegree++;
		}
	}
}


template<class T>
vector<Vertex<T>*> Graph<T>::getSources() const {
	vector< Vertex<T>* > buffer;
	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		if( vertexSet[i]->indegree == 0 ) buffer.push_back( vertexSet[i] );
	}
	return buffer;
}


template <class T>
void Graph<T>::dfsVisit() {
	typename vector<Vertex<T>*>::const_iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::const_iterator ite= vertexSet.end();
	for (; it !=ite; it++)
		(*it)->visited=false;
	it=vertexSet.begin();
	for (; it !=ite; it++)
	    if ( (*it)->visited==false )
	    	dfsVisit(*it);
}

template <class T>
void Graph<T>::dfsVisit(Vertex<T> *v) {
	v->processing = true;
	v->visited = true;
	typename vector<Edge<T> >::iterator it= (v->adjacent).begin();
	typename vector<Edge<T> >::iterator ite= (v->adjacent).end();
	for (; it !=ite; it++) {
		if ( it->dest->processing == true) numCycles++;
	    if ( it->dest->visited == false ){
	    	dfsVisit(it->dest);
	    }
	}
	v->processing = false;
}

template<class T>
vector<T> Graph<T>::topologicalOrder() {
	//vector com o resultado da ordenacao
	vector<T> res;

	//verificar se é um DAG
	if( getNumCycles() > 0 ) {
		cout << "Ordenacao Impossivel!" << endl;
		return res;
	}

	//garantir que os "indegree" estao inicializados corretamente
	this->resetIndegrees();

	queue<Vertex<T>*> q;

	vector<Vertex<T>*> sources = getSources();
	while( !sources.empty() ) {
		q.push( sources.back() );
		sources.pop_back();
	}

	//processar fontes
	while( !q.empty() ) {
		Vertex<T>* v = q.front();
		q.pop();

		res.push_back(v->ID);

		for(unsigned int i = 0; i < v->adjacent.size(); i++) {
			v->adjacent[i].dest->indegree--;
			if( v->adjacent[i].dest->indegree == 0) q.push( v->adjacent[i].dest );
		}
	}

	//testar se o procedimento foi bem sucedido
	if ( res.size() != vertexSet.size() ) {
		while( !res.empty() ) res.pop_back();
	}

	//garantir que os "indegree" ficam atualizados ao final
	this->resetIndegrees();

	return res;
}

template<class T>
vector<T> Graph<T>::getPath(const T &origin, const T &dest){

	list<T> buffer;
	Vertex<T>* v = getVertex(dest);

	//cout << v->info << " ";
	buffer.push_front(v->ID);
	while ( v->path != NULL &&  v->path->ID != origin) {
		v = v->path;
		buffer.push_front(v->ID);
	}
	if( v->path != NULL )
		buffer.push_front(v->path->ID);


	vector<T> res;
	while( !buffer.empty() ) {
		res.push_back( buffer.front() );
		buffer.pop_front();
	}
	return res;
}

template<class T>
void Graph<T>::unweightedShortestPath(const T &s) {

	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		vertexSet[i]->path = NULL;
		vertexSet[i]->dist = INT_INFINITY;
	}

	Vertex<T>* v = getVertex(s);
	v->dist = 0;
	queue< Vertex<T>* > q;
	q.push(v);

	while( !q.empty() ) {
		v = q.front(); q.pop();
		for(unsigned int i = 0; i < v->adjacent.size(); i++) {
			Vertex<T>* w = v->adjacent[i].dest;
			if( w->dist == INT_INFINITY ) {
				w->dist = v->dist + 1;
				w->path = v;
				q.push(w);
			}
		}
	}
}
/*
template<class T>
vector<T> Graph<T>::getfloydWarshallPath(const T &origin, const T &dest){

	int originIndex = -1, destinationIndex = -1;

	for(unsigned int i = 0; i < vertexSet.size(); i++)
	{
		if(vertexSet[i]->info == origin)
			originIndex = i;
		if(vertexSet[i]->info == dest)
			destinationIndex = i;

		if(originIndex != -1 && destinationIndex != -1)
			break;
	}
	vector<T> res;

	//se nao foi encontrada solucao possivel, retorna lista vazia
	if(W[originIndex][destinationIndex] == INT_INFINITY)
		return res;

	res.push_back(vertexSet[originIndex]->info);

	//se houver pontos intermedios...
	if(P[originIndex][destinationIndex] != -1)
	{
		int intermedIndex = P[originIndex][destinationIndex];
		getfloydWarshallPathAux(originIndex, intermedIndex, res);
		res.push_back(vertexSet[intermedIndex]->info);
		getfloydWarshallPathAux(intermedIndex,destinationIndex, res);
	}
	res.push_back(vertexSet[destinationIndex]->info);

	return res;
}

template<class T>
void Graph<T>::getfloydWarshallPathAux(int index1, int index2, vector<T> & res){
	if(P[index1][index2] != -1)
	{
		getfloydWarshallPathAux(index1, P[index1][index2], res);

		res.push_back(vertexSet[P[index1][index2]]->info);

		getfloydWarshallPathAux(P[index1][index2],index2, res);
	}
}
*/
template<class T>
void Graph<T>::bellmanFordShortestPath(const T &s) {
	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		vertexSet[i]->path = NULL;
		vertexSet[i]->dist = INT_INFINITY;
	}

	Vertex<T>* v = getVertex(s);
	v->dist = 0;
	queue< Vertex<T>* > q;
	q.push(v);

	while( !q.empty() ) {
		v = q.front(); q.pop();
		for(unsigned int i = 0; i < v->adjacent.size(); i++) {
			Vertex<T>* w = v->adjacent[i].dest;
			if(v->dist + v->adjacent[i].weight < w->dist) {
				w->dist = v->dist + v->adjacent[i].weight;
				w->path = v;
				q.push(w);
			}
		}
	}
}

template<class T>
void Graph<T>::dijkstraShortestPath(const T &s) {
	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		vertexSet[i]->path = NULL;
		vertexSet[i]->dist = INT_INFINITY;
		vertexSet[i]->processing = false;
	}

	Vertex<T>* v = getVertex(s);
	v->dist = 0;

	vector< Vertex<T>* > pq;
	pq.push_back(v);

	make_heap(pq.begin(), pq.end());


	while( !pq.empty() ) {

		v = pq.front();
		pop_heap(pq.begin(), pq.end());
		pq.pop_back();

		for(unsigned int i = 0; i < v->adjacent.size(); i++) {
			Vertex<T>* w = v->adjacent[i].dest;

			if(v->dist + v->adjacent[i].weight < w->dist ) {
				w->dist = v->dist + v->adjacent[i].weight;
				w->path = v;

				//se já estiver na lista, apenas a actualiza
				if(!w->processing){
					w->processing = true;
					pq.push_back(w);
				}

				make_heap (pq.begin(),pq.end(),vertex_greater_than<T>());
			}
		}
	}
}

template<class T>
int Graph<T>::edgeCost(int vOrigIndex, int vDestIndex){
	if(vertexSet[vOrigIndex] == vertexSet[vDestIndex])
		return 0;

	for(unsigned int i = 0; i < vertexSet[vOrigIndex]->adjacent.size(); i++){
		if(vertexSet[vOrigIndex]->adjacent[i].dest == vertexSet[vDestIndex])
			return vertexSet[vOrigIndex]->adjacent[i].weight;
	}

	return INT_INFINITY;
}

template<class T>
void Graph<T>::Astar(Vertex<T> *sourc , Vertex<T> *dest){
	for(unsigned int i = 0; i < vertexSet.size(); i++) {
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
		cout << "Processing v=" << curr->getID() << endl;
		for ( Edge<T> edge : curr->adjacent){
			Vertex<T> *adjacent = edge.dest;
			if (adjacent->getID() == dest->getID())
				return;
			cout << "Adj (" << adjacent->getID() << ") " << adjacent->dist;
			adjacent->dist += curr->dist + edge.weight + calculateDistance(adjacent,dest);
			cout << " ,after " << adjacent->dist << endl;
			//TODO can be optimized to just one cycle (with more branches)
			for( Vertex<T> *temp : open_list )
				if ( (temp->getID() == adjacent->getID()) && (temp->dist < adjacent->dist) )
					continue;
			for( Vertex<T> *temp : closed_list )
				if ( (temp->getID() == adjacent->getID()) && (temp->dist < adjacent->dist) )
					continue;
			open_list.push_back(adjacent);
			adjacent->path = curr;
		}
		closed_list.push_back(curr);
	}
}

void printSquareArray(int ** arr, unsigned int size){
	for(unsigned int k = 0; k < size; k++){
		if(k == 0){
			cout <<  "   ";
			for(unsigned int i = 0; i < size; i++)
				cout <<  " " << i+1 << " ";
			cout << endl;
		}

		for(unsigned int i = 0; i < size; i++){
			if(i == 0)
				cout <<  " " << k+1 << " ";

			if(arr[k][i] == INT_INFINITY)
				cout << " - ";
			else
				cout <<  " " << arr[k][i] << " ";
		}

		cout << endl;
	}
}

/*
template<class T>
void Graph<T>::floydWarshallShortestPath() {

	W = new int * [vertexSet.size()];
	P = new int * [vertexSet.size()];
	for(unsigned int i = 0; i < vertexSet.size(); i++){
		W[i] = new int[vertexSet.size()];
		P[i] = new int[vertexSet.size()];
		for(unsigned int j = 0; j < vertexSet.size(); j++){
			W[i][j] = edgeCost(i,j);
			P[i][j] = -1;
		}
	}


	for(unsigned int k = 0; k < vertexSet.size(); k++)
		for(unsigned int i = 0; i < vertexSet.size(); i++)
			for(unsigned int j = 0; j < vertexSet.size(); j++){
				//se somarmos qualquer coisa ao valor INT_INFINITY, ocorre overflow, o que resulta num valor negativo, logo nem convém considerar essa soma
				if(W[i][k] == INT_INFINITY || W[k][j] == INT_INFINITY)
					continue;

				int val = min ( W[i][j], W[i][k]+W[k][j] );
				if(val != W[i][j]){
					W[i][j] = val;
					P[i][j] = k;
				}
			}

}
*/
#endif /* GRAPH_H */
