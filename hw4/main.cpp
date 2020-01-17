//modify input file here
#define INPUT_FILE "/Users/frank/git/cse674/hw4/maze.txt"

#define RUN_TESTS false
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <stack>
#include <queue>
#include <algorithm>

using namespace std;

//cell definition
class cell {
public:
    int id;
    int wall;
    bool visited;
    bool in_structure;
    int from;
    cell(int,int,bool,bool,int);
};
cell::cell(int id, int wall, bool visited, bool in_structure, int from){
    this->id = id;
    this->wall = wall;
    this->visited = visited;
    this->in_structure = in_structure;
    this->from = from;
};

//declarations
void construct(int&,int&,int&,vector<cell>&,bool);
string to_binary(int);
void tests();
int is_cell_avaliable(vector<cell>, int, char, int);
vector<int> get_avaliable_cells(vector<cell>, int, int);
bool in_vector(vector<int> v,int target);
vector<int> get_solution_array(vector<cell> &,int, int);
void print_solution(vector<cell> &maze, vector<int> solution, int start, int end, bool without_path);
void DFS(vector<cell> &maze, int n, int start, int end);
void BFS(vector<cell> &maze, int n, int start, int end);

int main() {
    int cell_num;
    int start;
    int target;
    if (RUN_TESTS) tests();
    vector<cell> maze1;
    construct(cell_num, start, target, maze1, true);
    cout << "===============" <<endl;
    DFS(maze1,cell_num,start,target);
    cout << "===============" <<endl;
    vector<cell> maze2;
    construct(cell_num, start, target, maze2,false);
    BFS(maze2,cell_num,start,target);
    return 0;
}

void print_solution(vector<cell> &maze, vector<int> solution, int start, int end, bool without_path=false){
    int size = maze.size();
    int length = sqrt(size);
    for(int i=0;i<length;i++){
        for (int j=0;j<length;j++){
            if(to_binary(maze[i*length+j].wall)[1] == '1'){
                cout << "+---";
            }else{
                cout << "+   ";
            }
        }
        cout << "+" << endl;
        for(int j=0;j<length;j++){
            string mid = "    ";
            if(to_binary(maze[i*length+j].wall)[0] == '1'){
                mid[0] = '|';
            }
            if(!without_path) {
                if (maze[i * length + j].in_structure) {
                    mid[2] = '*';
                }
            }
            cout << mid;
        }
        if(to_binary(maze[i*length+length-1].wall)[2] == '1'){
            cout << "|";
        }
        cout << endl;
    }
    int i = length-1;
    for (int j=0;j<length;j++){
        if(to_binary(maze[i*length+j].wall)[3] == '1'){
            cout << "+---";
        }else{
            cout << "+   ";
        }
    }
    cout << "+" << endl;
}

bool in_vector(vector<int> v,int target){
    if(find(v.begin(), v.end(), target) != v.end()) {
        return true;
    } else {
        return false;
    }
}

vector<int> get_avaliable_cells(vector<cell> maze, int no, int from){
    string walls = to_binary(maze[no].wall);
    // convert the wall value to binary
    // get string like 0001

    vector<int> avaliable_cells;

    int successor;
    if (walls[3] == '0'){
        //south open
        successor = is_cell_avaliable(maze,no,'s',from);
        if (successor >= 0){
            avaliable_cells.push_back(successor);
        }
    }
    if (walls[2] == '0'){
        //east open
        successor = is_cell_avaliable(maze,no,'e',from);
        if (successor >= 0){
            avaliable_cells.push_back(successor);
        }
    }
    if (walls[1] == '0'){
        //north open
        successor = is_cell_avaliable(maze,no,'n',from);
        if (successor >= 0){
            avaliable_cells.push_back(successor);
        }
    }
    if (walls[0] == '0'){
        //west open
        successor = is_cell_avaliable(maze,no,'w',from);
        if(successor >= 0){
            avaliable_cells.push_back(successor);
        }
    }
    return avaliable_cells;
}

int is_cell_avaliable(vector<cell> maze, int no, char direction, int from){
    int size = maze.size();
    int length = sqrt(size);

    if (direction == 'w') {
        if (no % length > 0 && no - 1 != from && !maze[no-1].visited) {
            return no - 1;
        }
    }else if(direction == 'n') {
        if (no / length > 0 && no-length != from && !maze[no-length].visited) {
            return no - length;
        }
    }else if(direction == 'e'){
        if(no%length < length-1 && no+1 != from && !maze[no+1].visited){
            return no+1;
        }
    }else if(direction == 's'){
        if (no/length < length-1 && no+length != from && !maze[no+length].visited){
            return no+length;
        }
    }
    return -1;
}

string to_binary(int input){
    if (input == 0){
        return "0000";
    }
    string r;
    for(int i=0; input >0; i++){
        int temp = input%2;
        char temp_char = '0'+temp;
        r+=temp_char;
        input/=2;
    }

    string result;
    for(int i=3;i>=0;i--){
        if (i>r.size()-1){
            result+='0';
        }else{
            result+=r[i];
        }
    }
    return result;
}

void DFS(vector<cell> &maze, int n, int start, int end) {
    stack<int> stack;
    stack.push(start);
    vector<int> solution;
    int current = -1;
    int last = -1;
    bool success = false;
    while (!stack.empty()) {
        current = stack.top();
        //maze[current].visited = true;
        if (current == end) {
            success = true;
            break;
        }
        //cout << "current:  ==== " << current <<endl;
        //cout << "stack size" <<stack.size() << endl;
        vector<int> cells = get_avaliable_cells(maze, current, last);
        if (cells.size() == 0){
            //backtrack
            stack.pop();
        }
        for (int i=0;i < cells.size(); i++) {
            maze[cells[i]].from = current;
	    maze[cells[i]].visited = true;
            stack.push(cells[i]);
        }
        last = current;
    }
    if(success){
        solution = get_solution_array(maze,start,end);
        cout << "DFS Solution: ";
        for(int i=0;i<solution.size();i++){
            cout << solution[i] << ' ';
        }
        cout << endl;
        print_solution(maze,solution,start,end);
    }else{
        cout << "DFS No solution" <<endl;
        print_solution(maze,solution,start,end,true);
    }
}

void BFS(vector<cell> &maze, int n, int start, int end){
    queue<int> queue;
    queue.push(start);
    vector<int> solution;
    int current = -1;
    int last = -1;
    bool success = false;
    while (!queue.empty()) {
        current = queue.front();
        maze[current].visited = true;
        if (current == end) {
            success = true;
            break;
        }
        //cout << "current:  ==== " << current <<endl;
        //cout << "queue size" <<queue.size() << endl;
        queue.pop();
        vector<int> cells = get_avaliable_cells(maze, current, last);
        //cout << "cell size" <<cells.size() << endl;
        for (int i = 0; i < cells.size(); i++) {
            queue.push(cells[i]);
            maze[cells[i]].visited = true;
            maze[cells[i]].from = current;
        }
        last = current;
    }
    if(success){
        solution = get_solution_array(maze,start,end);
        cout << "BFS Solution: ";
        for(int i=0;i<solution.size();i++){
            cout << solution[i] << ' ';
        }
        cout << endl;
        print_solution(maze,solution,start,end);
    }else{
        cout << "BFS No solution" <<endl;
        print_solution(maze,solution,start,end,true);
    }
}

vector<int> get_solution_array(vector<cell> &maze,int start, int end){
    int current;
    vector<int> solution_reverse;
    vector<int> solution;
    current = end;
    while(true){
        solution_reverse.push_back(current);
        maze[current].in_structure = true;
        if (current == start){
            break;
        }
        current = maze[current].from;
    }

    for(int i = solution_reverse.size()-1;i>=0;i--){
        solution.push_back(solution_reverse[i]);
    }
    return solution;
}

void construct(int &cell_num, int &start, int &target, vector<cell> &maze, bool print){
    ifstream file(INPUT_FILE);

    string cell_num_s;
    string start_s;
    string target_s;

    getline(file,cell_num_s);
    getline(file,start_s);
    getline(file,target_s);

    stringstream cell_num_stream(cell_num_s);
    stringstream start_stream(start_s);
    stringstream target_stream(target_s);

    cell_num_stream >> cell_num;
    start_stream >> start;
    target_stream >> target;

    string line;
    getline(file,line);
    vector<int> lineData;
    stringstream lineStream(line);

    int v;
    vector<int> walls;
    while (lineStream >> v){
        walls.push_back(v);
    }

    if(print){
        cout << cell_num <<endl;
        cout << start << endl;
        cout << target << endl;
    }

    for (int i = 0;i<walls.size();i++){
        if(print){
            cout << walls[i] << " ";
        }
        cell m = cell(i,walls[i],false,false,-1);
        maze.push_back(m);
    }
    cout << endl;
}

void tests(){
    cout << 2/3 <<endl;
    cout << 7/3 <<endl;
    cout << 3%3 <<endl;
    cout << 5%3 <<endl;
    cout << 4%3 <<endl;
    cout << 4/3 <<endl;
    cout << "======" <<endl;
    cout << to_binary(0)<<endl;
    cout << to_binary(2)<<endl;
    cout << to_binary(4)<<endl;
    cout << to_binary(8)<<endl;
    cout << "====tests done===="<<endl;
}

// I wrote a Python script to generate test cases: https://git.guoduhao.cn/snippets/9
/* test case
400
0
399
14 12 6 14 14 13 7 13 7 12 4 4 7 12 4 4 4 7 13 7 8 1 2 9 2 14 14 15 12 3 9 0 7 11 10 11 9 6 14 14 8 5 2 13 0 0 0 6 11 14 13 3 12 5 3 14 13 3 9 3 8 5 0 5 1 0 2 9 4 3 12 4 2 12 5 0 6 14 13 6 10 15 8 6 12 3 8 7 8 5 1 2 8 3 14 11 9 0 7 10 10 15 9 0 3 12 0 5 0 7 12 2 10 14 11 12 7 9 4 3 9 4 4 2 14 11 11 13 1 7 11 8 3 9 5 0 7 12 2 15 13 1 3 9 3 14 12 5 5 4 7 11 13 6 13 2 12 0 0 6 13 5 5 4 4 1 1 5 5 3 12 4 7 9 4 2 8 2 10 10 12 4 4 1 1 6 12 6 14 12 2 9 4 6 9 0 2 8 1 2 11 8 0 6 15 8 2 9 0 1 1 6 8 0 4 1 2 9 4 2 13 0 2 8 6 9 0 6 10 14 14 8 2 8 0 4 0 4 1 3 13 0 1 0 3 14 9 0 2 10 10 10 10 9 1 3 9 3 14 15 14 11 13 1 6 9 6 11 10 10 10 8 0 4 4 4 5 6 9 6 11 14 13 5 1 5 1 6 10 8 3 9 2 10 9 2 13 0 5 3 14 11 15 15 12 5 4 0 0 2 13 4 0 2 13 3 13 1 4 6 11 14 14 12 1 6 11 8 3 9 4 0 0 3 13 4 6 14 11 10 15 11 9 3 12 2 12 0 6 12 1 3 9 4 6 8 1 3 14 10 12 6 15 15 11 9 1 3 9 0 6 13 6 9 2 8 5 7 10 10 11 11 13 5 7 13 7 15 13 1 1 7 9 5 1 1 5 5 1 3

+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
|   |       |   |   |       |       |               |                   |       |
+   +   +   +   +   +---+---+---+---+   +   +   +---+   +   +   +   +---+---+---+
|           |       |   |   |   |       |           |   |   |   |       |   |   |
+   +---+   +---+   +   +   +---+   +---+---+   +---+---+   +---+---+   +   +   +
|           |                   |   |   |       |           |   |       |       |
+   +---+   +---+   +   +   +   +---+   +---+---+   +---+---+   +---+---+---+---+
|                           |           |           |               |   |       |
+   +---+   +---+---+   +   +---+   +---+   +   +   +   +---+   +   +   +---+   +
|   |   |       |       |       |               |       |   |   |           |   |
+   +---+   +   +   +---+   +---+   +---+---+   +   +---+   +---+---+   +---+   +
|   |   |           |                   |       |   |   |   |       |           |
+   +---+---+   +---+   +   +---+   +---+   +   +   +   +---+   +---+---+   +---+
|               |   |   |   |           |   |       |               |       |   |
+---+   +   +   +   +---+---+---+---+---+---+   +---+---+---+   +---+   +   +---+
|           |       |   |                   |   |       |       |               |
+---+---+---+---+---+   +   +---+---+   +---+---+---+   +---+   +   +   +   +   +
|                                       |           |           |       |   |   |
+---+---+---+   +   +---+---+---+---+---+   +   +---+---+   +   +   +   +   +   +
|                       |       |   |       |           |           |           |
+   +   +   +---+---+   +   +   +   +   +   +---+   +   +---+   +   +   +---+   +
|   |           |   |       |                   |                   |           |
+---+   +   +   +---+   +   +---+   +---+---+   +   +   +   +---+   +---+   +   +
|           |       |           |   |   |   |       |                           |
+---+   +   +   +   +---+   +   +   +   +   +   +   +   +   +   +   +   +---+---+
|                   |   |           |   |   |   |   |           |       |   |   |
+---+   +---+   +---+   +---+   +   +   +   +   +   +---+---+---+---+---+   +---+
|   |   |           |       |   |   |   |   |                           |       |
+   +---+---+---+   +---+   +---+   +   +   +   +   +   +   +   +---+   +---+   +
|   |   |                       |   |       |       |   |       |               |
+---+   +---+---+---+---+---+   +   +   +---+---+   +   +---+   +---+   +---+---+
|   |   |   |   |                       |               |       |               |
+   +---+---+---+   +---+   +   +   +   +---+   +   +   +---+---+---+---+   +   +
|   |   |   |           |   |       |                   |           |   |   |   |
+---+   +   +   +---+   +---+   +---+---+   +   +   +---+---+   +   +   +---+   +
|   |   |       |       |           |           |           |           |   |   |
+---+---+---+---+   +   +   +   +   +   +---+---+---+   +   +   +---+---+   +   +
|       |   |   |   |           |           |       |       |           |   |   |
+   +   +---+---+---+---+---+---+---+   +   +---+   +---+   +   +---+---+   +   +
|   |   |           |       |   |               |                               |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
*/
