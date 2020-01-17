#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int source;
int nodes_num;
int edges_num;

//graph node
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

//heap
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

//edge defination
class edge{
public:
    int from;
    int to;
    int cost;
    edge(int,int,int);
};

edge::edge(int from, int to, int cost) {
    this->from = from;
    this->to = to;
    this->cost = cost;
}

//declarations
void init_graph(vector<vector<n_node>*>&);
int heap_swap(vector<h_node>& , int, int);
int heap_insert(vector<h_node>& , int, int);
vector<edge> prim(int,vector<vector<n_node>*>, vector<h_node>&);
int heap_remove_by_index(vector<h_node>&, int);
void print_result(vector<edge>);
bool is_in_vector(vector<int>, int);


int main(int argc, const char * argv[]) {

    vector<vector<n_node>*> graph;
    vector<h_node> heap;
    vector<edge> result = prim(source,graph,heap);
    print_result(result);
    getchar();
    getchar();
    return 0;
}

vector<edge> prim(int source, vector<vector<n_node>*> graph, vector<h_node> &heap){
    vector<int> visited_vertex;
    vector<edge> edges;
    vector<edge> result;


    ifstream in("/Users/frank/git/cse674/hw3/case3.txt");

    in >> source;
    in >> nodes_num;
    in >> edges_num;
    cout << nodes_num << ' ' << edges_num << endl;

    for(int i = 0; i < edges_num; i++) {
        //init edges table
        int from, to;
        int cost;
        in >> from >> to >> cost;
        cout << from << ' ' << to << ' ' << cost << endl;
        edges.push_back(edge(from, to, cost));
        heap_insert(heap, edges.size()-1, cost);
    }

    //find a start vertex

    int start_vertex = source;//find_start_vertex(graph);
    visited_vertex.push_back(start_vertex);

    //loop until all of the vertexes are added to the visited vector
    for (int i = 0;i < nodes_num-1;i++){
        //iterate all of the edges connected to the current vertex
        //add the edges to the edges array
        //and add id and cost of the edges to the heap
        //find the edge with least cost
        //select first one in the heap
        int edge_index;
        //add vertex id to visited array
        //add edge to result table

        vector<h_node> temp_heap;

        while(heap.size()>0){
            if ((is_in_vector(visited_vertex, edges[heap[0].id].from) && !is_in_vector(visited_vertex, edges[heap[0].id].to))
            ||
            (is_in_vector(visited_vertex, edges[heap[0].id].to) && !is_in_vector(visited_vertex, edges[heap[0].id].from))) {
                    //selected
                    edge_index = heap[0].id;
                    heap_remove_by_index(heap,0);
                    break;
            }else{
                temp_heap.push_back(heap[0]);
                heap_remove_by_index(heap,0);
            }
        }
        for(int i=0;i<temp_heap.size();i++){
            heap_insert(heap,temp_heap[i].id,temp_heap[i].cost);
        }
        temp_heap.clear();

        if (!is_in_vector(visited_vertex,edges[edge_index].to)){
            visited_vertex.push_back(edges[edge_index].to);
        }

        if (!is_in_vector(visited_vertex,edges[edge_index].from)){
            visited_vertex.push_back(edges[edge_index].from);
        }

        result.push_back(edges[edge_index]);
    }
    return result;
}

int find_start_vertex(vector<vector<n_node>*> graph){
    for(int i=0;i<graph.size();i++){
        if (graph[i] != nullptr){
            return i;
        }
    }
    return -1;
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
        //cout << heap[current_index].cost << heap[parent_index].cost <<endl;
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

int heap_remove_by_index(vector<h_node>& heap, int index) {
    index = 0;
    int last_index = heap.size() - 1;
    heap_swap(heap, 0, last_index);
    //remove last node
    heap.erase(heap.begin() + last_index);
    last_index = heap.size() - 1;
    int current_index = 0;

    while (current_index*2+1 <= last_index) {
        int child_index_1, child_index_2,target;
        child_index_1 = current_index*2+1;
        child_index_2 = current_index*2+2;
        if (current_index*2+1 <= last_index && current_index*2+2 > last_index){
            //left child exist while right do not exist
            if (heap[current_index].cost > heap[child_index_1].cost) {
                //follow left
                target = child_index_1;
            }else{
                break;
            }
        }else{
            //both child exist
            if (heap[current_index].cost <= heap[child_index_1].cost && heap[current_index].cost <= heap[child_index_2].cost) {
                //do not move
                break;
            }else {
                if (heap[child_index_1].cost < heap[child_index_2].cost){
                    target = child_index_1;
                }else{
                    target = child_index_2;
                }
            }
        }
        heap_swap(heap,current_index,target);
        current_index = target;
    }
    return current_index;
}

void print_result(vector<edge> result){
    int weight = 0;
    cout << "result: ========="<<endl;
    for(int i=0;i<result.size();i++){
        cout << result[i].from << ' ' << result[i].to << ' ' << result[i].cost << endl;
        weight += result[i].cost;
    }
    cout << "cost: "<< weight<<endl;
}

bool is_in_vector(vector<int>v, int target){
    if (count(v.begin(), v.end(), target))
        return true;
    else
        return false;
}