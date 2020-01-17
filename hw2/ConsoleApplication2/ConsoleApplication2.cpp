#include <iostream>
#include <fstream>
#include <vector>
#include <limits>

using namespace std;

int source;
int nodes_num;
int edges_num;


class n_node {
public:
	int id;  //neighbor id
	int weight; //edge weight
	n_node(int, int);
};

n_node::n_node(int id, int weight) {
	this->id = id;
	this->weight = weight;
}


class h_node {
public:
	int id;
	int cost;
	h_node(int, int);
};

h_node::h_node(int id, int cost) {
	this->id = id;
	this->cost = cost;
}

//define routing table node
class rt_node {
public:
	bool is_visited;
	int cost;
	int from;
	int h_pos;
	rt_node(bool, int, int, int);
};

rt_node::rt_node(bool visited, int cost, int from, int heap_position) {
	this->is_visited = visited;
	this->cost = cost;
	this->from = from;
	this->h_pos = heap_position;
}


//declarations
void init_graph(vector<vector<n_node>*>&);
void init_routing_table(vector<rt_node>&, vector<vector<n_node>*>);
bool is_in_vector(vector<int>, int);
void init_heap(vector<h_node>&, vector<rt_node>&);
void dijkstra(vector<vector<n_node>*>, vector<rt_node>&, vector<h_node>&);
int heap_insert(vector<h_node>&,int,int);
bool have_unvisited(vector<rt_node>);
int heap_swap(vector<h_node>&, int, int);
void print_routing_table(vector<rt_node>);
int heap_remove_head(vector<h_node>&, int);
int heap_remove_by_index(vector<h_node>&, int);
int first_unvisited_in_rt(vector<rt_node>& );
int heap_rm(vector<h_node>&, int);

int main() {
	//define routing table
	vector<rt_node> Routing_Table;
	//define heap
	vector<h_node> Heap;
	//define graph
	vector<vector<n_node>*> Graph;
	init_graph(Graph);
	init_routing_table(Routing_Table, Graph);
	init_heap(Heap, Routing_Table);
	dijkstra(Graph, Routing_Table, Heap);
	print_routing_table(Routing_Table);
 	getchar();
	getchar();
	return 0;
}

//initialize graph
void init_graph(vector<vector<n_node>*>&graph) {

	int from, to;
	int cost;

	ifstream in("graph2.txt");

	in >> source;
	in >> nodes_num;
	in >> edges_num;
	cout << nodes_num << ' ' << edges_num << endl;
	
	for (int i = 0; i < nodes_num; i++) {
		graph.push_back(nullptr);
	}

	for(int i = 0; i < edges_num; i++) {
		in >> from >> to >> cost;
		cout << from << ' ' << to << ' ' << cost << endl;
		if (graph[from] != nullptr) {
			//exist
			graph[from]->push_back(n_node(to,cost));
		}else{
			//insert new
			graph[from] = new vector<n_node>;
			graph[from]->push_back(n_node(to, cost));
		}
	}
	in.close();
	return;
}

//init routing table
void init_routing_table(vector<rt_node>& routing_table, vector<vector<n_node>*> Graph) {
	vector<int> added;

	for (int i = 0; i < nodes_num; i++) {
		routing_table.push_back(rt_node(true, numeric_limits<int>::max(), -1, -1));
	}

	for(int i = 0;i<Graph.size();i++){
		//outer vector
		if (Graph[i]!=nullptr && !is_in_vector(added, i)){
			//not added
			added.push_back(i);
			routing_table[i] = rt_node(false, numeric_limits<int>::max(), -1, -1);
		}
		//inner
		if (Graph[i] != nullptr) {
			for (int j = 0; j < Graph[i]->size(); j++) {
				if (!is_in_vector(added, j)) {
					//not added
					added.push_back(j);
					routing_table[j] = rt_node(false, numeric_limits<int>::max(), -1, -1);
				}
			}
		}
	}
	routing_table[source].cost = 0;
	routing_table[source].from = source;
}

void dijkstra(vector<vector<n_node>*> graph, vector<rt_node>& routing_table, vector<h_node>& heap) {
	int from_id;
	//loop until all nodes have been visited
	while (have_unvisited(routing_table)) {
		if (heap.size() == 0) {
			break;
		}
		from_id = heap[0].id;
		if (graph[from_id] == nullptr) {
			routing_table[from_id].is_visited = true;
			heap_remove_head(heap, 0);
			continue;
		}
		//check if node visited
		int current_index = from_id;
		routing_table[current_index].is_visited = true;
		heap_remove_head(heap, 0);

		//iterate the successors of a source node
		for(int i=0;i<graph[from_id]->size();i++){
			int to = (*graph[from_id])[i].id;
			int cost = (*graph[from_id])[i].weight;
			int index = (*graph[from_id])[i].id;
			//there is a cost update
			if (routing_table[current_index].cost + cost < routing_table[index].cost) {
				routing_table[index].from = from_id;
				routing_table[index].cost = routing_table[current_index].cost + cost;
				//remove the heap node if it exist
				//then insert it with a updated cost
				//as the update to the heap
				//a node won't be inserted to the heap until its value changes to a valid value from infinity
				heap_rm(heap, index);
				int inserted_index = heap_insert(heap, to, cost);
				//update heap position in routing table
				routing_table[index].h_pos = inserted_index;
			}
		}
	}
}

void print_routing_table(vector<rt_node> routing_table) {
	for (int i = 0; i < routing_table.size(); i++) {
		cout << "The cost from node " << source << " to node " << i << " is " << routing_table[i].cost << "; from node is " << routing_table[i].from << endl;
	}
	return;
}

bool have_unvisited(vector<rt_node> routing_table) {
	for (int i = 0; i < routing_table.size(); i++) {
		if (routing_table[i].is_visited == false) {
			return true;
		}
	}
	return false;
}

int heap_rm(vector<h_node>& heap, int index) {
	for (int i = 0; i < heap.size(); i++) {
		if (index == heap[i].id) {
			return heap_remove_by_index(heap, i);
		}
	}
	return -1;
}

int heap_remove_by_index(vector<h_node>& heap, int index) {
	if (index > heap.size() - 1 || index < 1) {
		return -1;
	}
	else {
		//swap with last node	
		heap_swap(heap, index, heap.size() - 1);
		//remove last node
		heap.erase(heap.begin() + heap.size() - 1);
		int current_index = index;
		int last_index = heap.size() - 1;
		//compare with parent
		int parent_index;
		if (current_index % 2 == 0) {
			parent_index = (current_index - 2) / 2;
		}else {
			parent_index = (current_index - 1) / 2;
		}
		
		//invalid
		if (parent_index < 0 || current_index >= heap.size()) {
			return current_index;
			//smaller than parent
		}else if (heap[current_index].cost < heap[parent_index].cost) {
			while (current_index > 0) {
				int parent_index;
				if (current_index % 2 == 0) {
					parent_index = (current_index - 2) / 2;
				}
				else {
					parent_index = (current_index - 1) / 2;
				}
				if (heap[current_index].cost < heap[parent_index].cost) {
					heap_swap(heap, current_index, parent_index);
					current_index = parent_index;
				}
				else {
					break;
				}
			}
			return current_index;
		}else if (heap[current_index].cost == heap[parent_index].cost) {
			return current_index;
		}else {
			//larger than child
			while (current_index * 2 + 1 <= last_index) {
				int child_index_1, child_index_2, target;
				child_index_1 = current_index * 2 + 1;
				child_index_2 = current_index * 2 + 2;
				if (current_index * 2 + 1 <= last_index && current_index * 2 + 2 > last_index) {
					//left child exist while right do not exist
					if (heap[current_index].cost > heap[child_index_1].cost) {
						//follow left
						target = child_index_1;
					}
					else {
						break;
					}
				}
				else {
					//both child exist
					if (heap[current_index].cost <= heap[child_index_1].cost && heap[current_index].cost <= heap[child_index_2].cost) {
						//do not move
						break;
					}
					else {
						if (heap[child_index_1].cost < heap[child_index_2].cost) {
							target = child_index_1;
						}
						else {
							target = child_index_2;
						}
					}
				}
				heap_swap(heap, current_index, target);
				current_index = target;
			}
			return current_index;
		}
		return index;
	}
}


int heap_remove_head(vector<h_node>& heap, int index) {
	index = 0;
	int last_index = heap.size() - 1;
	heap_swap(heap, 0, last_index);
	//remove last node
	heap.erase(heap.begin() + last_index);
	last_index = heap.size() - 1;
	int current_index = 0;

	while (current_index * 2 + 1 <= last_index) {
		int child_index_1, child_index_2, target;
		child_index_1 = current_index * 2 + 1;
		child_index_2 = current_index * 2 + 2;
		if (current_index * 2 + 1 <= last_index && current_index * 2 + 2 > last_index) {
			//left child exist while right do not exist
			if (heap[current_index].cost > heap[child_index_1].cost) {
				//follow left
				target = child_index_1;
			}
			else {
				break;
			}
		}
		else {
			//both child exist
			if (heap[current_index].cost <= heap[child_index_1].cost && heap[current_index].cost <= heap[child_index_2].cost) {
				//do not move
				break;
			}
			else {
				if (heap[child_index_1].cost < heap[child_index_2].cost) {
					target = child_index_1;
				}
				else {
					target = child_index_2;
				}
			}
		}
		heap_swap(heap, current_index, target);
		current_index = target;
	}
	return current_index;
}

int heap_insert(vector<h_node>& heap, int id, int cost) {
	//insert nodes to construct a max heap
	//every child should be smaller than parent
	h_node heap_node = h_node(id, cost);
	heap.push_back(heap_node);
	int current_index = heap.size() - 1;
	//check if new node larger than parent, if does, swap
	while (current_index > 0) {
		int parent_index;
		if (current_index % 2 == 0) {
			parent_index = (current_index - 2) / 2;
		} else {
			parent_index = (current_index - 1) / 2;
		}
		if (heap[current_index].cost < heap[parent_index].cost) {
			heap_swap(heap, current_index, parent_index);
			current_index = parent_index;
		} else {
			break;
		}
	}
	return current_index;
}

int heap_swap(vector<h_node>& heap, int index1, int index2) {
	h_node temp = heap[index2];
	heap[index2] = heap[index1];
	heap[index1] = temp;
	return 0;
}

void init_heap(vector<h_node>& heap, vector<rt_node>& routing_table) {
	heap.push_back(h_node(source, 0));
	return;
}

int first_unvisited_in_rt(vector<rt_node>& routing_table) {
	for (int i = 0; i < routing_table.size(); i++) {
		if (routing_table[i].is_visited == false) {
			return i;
		}
	}
	return -1;
}

bool is_in_vector(vector<int> vec, int key) {
	if (count(vec.begin(), vec.end(), key)) {
		return true;
	}
	else {
		return false;
	}
}