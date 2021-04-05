#include <iostream>
#include <vector>
#include <ctime> 
#include <queue>
using namespace std;

const float max_distance = 10.0f;

//==========================================================
//	Funcions for random number generation and probability
//==========================================================
inline float prob() {
	return (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
}

inline float genRandDistance() {
	return (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (max_distance)));
}

//==========================================================
//	Element "node"
//==========================================================
struct node {
	node() :vertice(0), costToReach(0.0f) {}
	node(int v) :vertice(v) { costToReach = genRandDistance(); }
	node(int v, float c) :vertice(v), costToReach(c) {}
	int vertice; float costToReach;
};
// Overloading operators for element "node"
inline node operator++(node& myNode)
{
	myNode.vertice++;
	return myNode;
}
ostream& operator<< (ostream& out, const node& myNode)
{
	out << "(" << myNode.vertice << " ," << myNode.costToReach << ") ";
	return out;
}
bool operator < (const node& lhs, const node& rhs)
{
	if (lhs.vertice < rhs.vertice)
		return true;
	else
		return false;
}
bool operator > (const node& lhs, const node& rhs)
{
	if (lhs.vertice > rhs.vertice)
		return true;
	else
		return false;
}
bool operator == (const node& lhs, const node& rhs)
{
	if (lhs.vertice == rhs.vertice)
		return true;
	else
		return false;
}
bool operator == (const node& lhs, int& rhs)
{
	if (lhs.vertice == rhs)
		return true;
	else
		return false;
}

//==========================================================
//	Linked List "slist"
//==========================================================
struct slistelem {
	slistelem() {}
	node data = 0;
	slistelem* next = nullptr;
};
class slist { // singly linked list
public:
	slist() :h(0) {} //empty list
	~slist(); // { release(); } // destructor
	void prepend(node c); // adds to front of slist
	void prependOrdered(node c); // adds in order of vertex number
	void release();
	void del();
	void delete_chosen(int x);
	slistelem* first() const { return h; }
	void print() const;
	vector<node> neighbors();
	bool isEmptyList();
private:
	slistelem* h; // list head
};
// Definitions of methods:
void slist::prepend(node c)
{
	slistelem* temp = new slistelem;
	//allocation fails bad_alloc exception thrown
	temp->next = h; //single link
	temp->data = c;
	h = temp; // update h
}
void slist::prependOrdered(node myElem)
{
	if ((h != nullptr) && (h->data == myElem))
		return; // same vertex, no need to insert again
	if ((h == nullptr) || (h->data > myElem))
		prepend(myElem);	// if the list is empty or the new element is already 
							// the smallest one, just prepent
	else
	{
		slistelem* tempToIterate;
		tempToIterate = h;
		while (tempToIterate->next != nullptr && tempToIterate->next->data < myElem)
		{
			tempToIterate = tempToIterate->next;
		}
		if ((tempToIterate->next != nullptr) && (tempToIterate->next->data == myElem))
			return;
		slistelem* temp = new slistelem;
		//allocation fails bad_alloc exception thrown
		temp->next = tempToIterate->next; //single link
		temp->data = myElem;
		tempToIterate->next = temp; // update next
	}
}
void slist::print() const // object is unchanged
{
	slistelem* temp = h;
	while (temp != 0) { // detect end of slist
		cout << temp->data << " -> ";
		temp = temp->next;
	}
	cout << "###" << endl;
}
vector<node> slist::neighbors() {
	vector<node> reachable;
	slistelem* temp = h;
	while (temp != 0) { // detect end of slist
		reachable.push_back(temp->data);
		temp = temp->next;
	}
	return reachable;
}
void slist::del()
{
	slistelem* temp = h;
	h = h->next; // presumes nonempty slist
	delete temp;
}
void slist::delete_chosen(int x)
{
	if (h == nullptr)
		return;
	slistelem* temp = h;
	slistelem* temp_delete = nullptr;
	if (h->data == x)
	{
		temp_delete = h;
		h = temp_delete->next;
		delete temp_delete;
	}
	else
	{
		while (temp != nullptr)
		{
			if (temp->next->data == x)
			{
				temp_delete = temp->next;
				temp->next = temp_delete->next;
				delete temp_delete;
				return;
			}
			else
			{
				temp = temp->next;
			}
		}
	}
}
void slist::release()
{
	while (h != 0)
		del();
}
bool slist::isEmptyList()
{
	if (h == nullptr)
		return true;
	return false;
}
slist::~slist() {
	release();	// march thru list with deletes
}

//==========================================================
//	Class "graph"
//==========================================================
class graph {
public:
	graph() :m_nVertices(0), m_nEdges(0) {};
	graph(int N, float density) :m_nVertices(N), m_nEdges(0) {
		m_edgeList.reserve(N);
		for (int i = 0; i < m_nVertices; ++i) {
			m_edgeList.push_back(slist());
		}

		genGraphWDensity(density);
	};
	void genGraphWDensity(float density);
	void print();
	int V() { return m_nVertices; }
	int E() { return m_nEdges; }
	bool adjacent(int x, int y);
	vector<node> neighbors(int x);
	void add(int x, int y);
	void delete_edge(int x, int y);

private:
	int m_nVertices;
	int m_nEdges;

public:
	vector<slist> m_edgeList;
};
// Definitions of methods:
void graph::genGraphWDensity(float density) {
	for (int i = 0; i < m_nVertices; ++i)
		for (int j = 0; j < m_nVertices; ++j)
			if (i != j) // no loops
			{
				if (prob() < density)
				{
					float distance = genRandDistance();
					m_edgeList[i].prependOrdered({ j, distance });
					m_edgeList[j].prependOrdered({ i, distance });
					m_nEdges++;
				}
			}
}
void graph::print() {
	for (int i = 0; i < m_nVertices; ++i)
	{
		cout << "Vertex " << i << ": ";
		m_edgeList[i].print();
	}
}
bool graph::adjacent(int x, int y) {
	if ((x >= m_nVertices) || (y >= m_nVertices))
		return false;
	else
	{
		slistelem* temp = m_edgeList[x].first();
		while (temp != nullptr) {
			if (temp->data == y)
				return true;
			else
				if (temp->next != nullptr)
					temp = temp->next;
				else
					return false;
		}
		return false;
	}
}
vector<node> graph::neighbors(int x) {
	vector<node> reachable;
	if (x < m_nVertices)
	{
		reachable = m_edgeList[x].neighbors();
	}
	return reachable;
}
void graph::add(int x, int y) {
	if (!(adjacent(x, y)))
	{
		float distance = genRandDistance();
		m_edgeList[x].prependOrdered({ y, distance });
		m_edgeList[y].prependOrdered({ x, distance });
		m_nEdges++;
	}
}
void graph::delete_edge(int x, int y) {
	if (adjacent(x, y))
	{
		m_edgeList[x].delete_chosen(y);
		m_edgeList[y].delete_chosen(x);
		m_nEdges--;
	}
}

//==========================================================
//	Class "PriorityQueue"
//==========================================================

// this is a strucure which implements the 
// operator overlading for priority queue
struct CompareDistances {
	bool operator()(node const& n1, node const& n2)
	{
		// return "true" if "p1" is ordered  
		// before "p2", for example: 
		return n1.costToReach > n2.costToReach;
	}
};
// Inheriting from the standard priority queue class so we can search for elements in it
typedef node Val_TYPE;
typedef vector<Val_TYPE> Container_TYPE;
typedef priority_queue<Val_TYPE, Container_TYPE, CompareDistances> pri_queue;
class IterableQueue : public pri_queue {
public:
	Container_TYPE::iterator begin() {
		return pri_queue::c.begin();
	}
	Container_TYPE::iterator end() {
		return pri_queue::c.end();
	}
};
class PriorityQueue {
public:
	void chgPrioirity(int vrt, float priority);
	node minPrioirty();
	float contains(int queue_element);
	void Insert(node queue_element);
	node top();
	int size();
	bool empty();

private:
	IterableQueue m_Q;

};
// Definitions of methods:
void PriorityQueue::chgPrioirity(int vrt, float priority)
{
	bool flag = false;
	for (vector<node>::iterator p = m_Q.begin(); p != m_Q.end(); p++)
	{
		if (*p == vrt)
		{
			flag = true;
			p->costToReach = priority;
			break;
		}
	}
	// Re-sorting the queue after the change:
	std::make_heap(const_cast<node*>(&m_Q.top()),
		const_cast<node*>(&m_Q.top()) + m_Q.size(),
		CompareDistances());
}
node PriorityQueue::minPrioirty()
{
	node p = m_Q.top();
	m_Q.pop();
	return p;
}
float PriorityQueue::contains(int queue_element)
{
	bool flag = false;
	for (vector<node>::iterator p = m_Q.begin(); p != m_Q.end(); p++)
	{
		if (*p == queue_element)
		{
			return p->costToReach;
		}
	}
	return -1.0f;
}
void PriorityQueue::Insert(node queue_element)
{
	m_Q.push(queue_element);
}
node PriorityQueue::top()
{
	node p = m_Q.top();
	return p;
}
int PriorityQueue::size()
{
	return m_Q.size();
}
bool PriorityQueue::empty()
{
	return m_Q.empty();
}

//==========================================================
//	Class "ShortestPath"
//==========================================================
class ShortestPath {
public:
	ShortestPath(int iniClosedSet, graph* myGraph) :
		m_path_size(0), m_myGraph(myGraph), m_path_found(false), m_initialVrt(-1) {
		m_closedSet.reserve(iniClosedSet);
	};
	bool CheckIfInClosedSet(int n);
	void AddToClosedSet(node n);
	vector<node> CheckAllReachable(int vrt);
	void vertices();
	void findShortestPath(int src, int dst);
	float path_size(int src, int dst);
	void setInitial(int src) { m_initialVrt = src; m_prev[src] = -1; };
	void iterateThroughReachables(int cur_vrt, float cur_cost);
	vector<int> path(int src, int dst);
	bool pathFound();

private:
	vector<node> m_closedSet;
	PriorityQueue m_openSet;
	vector<int> m_prev; // tracks the previous node in the path
	float m_path_size; // total cost of the shortest path
	int m_initialVrt; // initial vertex (source)
	graph* m_myGraph;
	vector<node> m_reachable; // dynamic array containing all reachable nodes from the current node
	bool m_path_found;
	vector<int> m_path; // shortest path (obtained by back-tracking from "m_prev"
};
// Definitions of methods:
bool ShortestPath::CheckIfInClosedSet(int n_val)
{
	for (node my_node : m_closedSet) {
		if (my_node == n_val)
			return true;
	}
	return false;
}
void ShortestPath::AddToClosedSet(node n)
{
	m_closedSet.push_back(n);
}
vector<node> ShortestPath::CheckAllReachable(int vrt)
{
	return m_myGraph->neighbors(vrt);
}
void ShortestPath::vertices() {
	m_myGraph->print();
}
vector<int> ShortestPath::path(int src, int dst) {
	return m_path;
}
float ShortestPath::path_size(int src, int dst) {
	return m_path_size;
}
bool ShortestPath::pathFound() {
	return m_path_found;
}
// Auxiliary method for a task that repeats itself in the main algorithm:
// for each node that is reachable from the current node:
// 1) check if already in the closed set (in this case don't bother)
// 2) get the total cost to reach that node from the source
// 3) check if it already is in the open set. 
// 3.a) If not, add it with the cost
// 3.b) If yes, compare the costs and add if the total cost to reach it is 
// smaller than the existing one
// 4) Update previous node in "m_prev"
void ShortestPath::iterateThroughReachables(int cur_vrt, float cur_cost) {
	float tempCostToReach;
	for (node myNode : m_reachable) {
		if (!CheckIfInClosedSet(myNode.vertice))
		{
			myNode.costToReach += cur_cost;
			tempCostToReach = m_openSet.contains(myNode.vertice);
			if (tempCostToReach == -1.0f)
			{
				m_openSet.Insert(myNode);
				m_prev[myNode.vertice] = cur_vrt;
			}
			else
			{
				if (myNode.costToReach < tempCostToReach)
				{
					m_openSet.chgPrioirity(myNode.vertice, myNode.costToReach);
					m_prev[myNode.vertice] = cur_vrt;
				}
			}
		}
	}
}
// Method that implements Dijkstra's algorithm as explained in the lesson video
// (it is slightly different from the Wikipedia version, because it does not add
// every node to the open set first with infinite cost)
// This method needs to be called before methods "path", "path_size" or "path_found"
void ShortestPath::findShortestPath(int src, int dst) {
	int cur_vrt; // current vertex
	float cur_cost; // current cost to reach that vertex
	node temp_node;
	m_path_found = false;
	m_prev.clear(); // clear the dynamic array which tracks the previous node in the path
	int n_vrts = m_myGraph->V();
	m_prev.reserve(n_vrts);
	m_prev.assign(n_vrts, -2);
	setInitial(src);
	// add source to closed set
	AddToClosedSet(node(src, 0));
	// initialize current node
	cur_vrt = src;
	cur_cost = 0;
	while (cur_vrt != dst) {
		// check neighbors of current node
		m_reachable.clear();
		m_reachable = m_myGraph->neighbors(cur_vrt);
		if (!m_reachable.empty())
		{
			// iterate through all reachable vertices:
			iterateThroughReachables(cur_vrt, cur_cost);
		}
		// get vertex of least cost from open set
		if (m_openSet.empty()) break;
		temp_node = m_openSet.minPrioirty();
		AddToClosedSet(temp_node);
		cur_vrt = temp_node.vertice;
		cur_cost = temp_node.costToReach;
		if (cur_vrt == dst)
		{
			m_path_found = true;
			m_path_size = cur_cost;
		}
	}
	if (m_path_found)
	{
		// copy the shortest path to m_path:
		m_path.clear();
		int index = dst;
		do
		{
			m_path.push_back(index);
			index = m_prev[index];
		} while (index != -1);
		reverse(m_path.begin(), m_path.end());
	}

}

int main()
{
	// seeding the random number generator
	srand((unsigned)time(NULL));

	// get the number of vertices and density for the graphs
	std::cout << "Type the number of vertices of the graph\n";
	int N;
	cin >> N;
	std::cout << "Type the graph density\n";
	float density;
	cin >> density;
	// get the number of graphs to simulate
	std::cout << "Type the number of graphs to simulate\n";
	int nGraphs;
	cin >> nGraphs;

	// instantiate the graph
	graph myGraph(N, density);


	// get source and destination vertices
	int my_node_nr;
	cout << "Type the source vertex for finding the shortest path: " << endl;
	cin >> my_node_nr;
	cout << "Source vertex is: " << my_node_nr << endl;
	node initial_node(my_node_nr, 0.0f);
	int my_dst_vrt;
	cout << "Type destination vertex:";
	cin >> my_dst_vrt;

	// initialize auxiliary variables
	float sum_paths = 0.0f;
	int num_has_path = 0;

	// Run the Monte Carlo simulator
	for (int myCount = 0; myCount < nGraphs; myCount++)
	{
		cout << "Simulation: " << myCount << endl;
		graph myGraph(N, density);
		ShortestPath mySp(static_cast<int>(N * density), &myGraph);
		mySp.findShortestPath(my_node_nr, my_dst_vrt);
		if (mySp.pathFound())
		{
			float pathSize = mySp.path_size(my_node_nr, my_dst_vrt);
			cout << "Shortest path has been found: " << pathSize << endl;
			sum_paths += pathSize;
			num_has_path++;
		}
		else
			cout << "Path not found in current simulation" << endl;
	}

	// Print out the results to the screen
	cout << "Number of cases that found a shortest path: " << num_has_path << endl;
	if(num_has_path != 0)
		cout << "Average of all found shortest paths: " << sum_paths / num_has_path << endl;
	else
		cout << "Average of all found shortest paths: 0" << endl;
}

