#include <iostream>

using namespace std;

class Node {
public:
    int value;
    Node* next;
    Node(int i) { value = i; next = nullptr; }
    Node() { next = nullptr; }
};

class LinkedList {
public:
    int numNodes;
    Node* head;
    LinkedList() { numNodes = 0; head = nullptr; }
    void makeRandomList(int m, int n);
    void printList();
    //Recursively merge sort i numbers starting at node pointed by p
    void mergeSort(Node*& p, int i);//in-place recursive merge sort
    //Merge i1 numbers starting at node pointed by p1 with i2 numbers
    //starting at node pointed by p2
    void merge(Node*& p1, int i1, Node* p2, int i2);
};

void LinkedList::mergeSort(Node*& p, int i) {
    //cout << p->value << ' ' << i << endl;
    if (i <= 1) {
        return;
    }
    int mid = i / 2;
    //p2 must be assigned before next recursion, otherwise p will be modified after merge.
    Node* p2 = p;
    for (int j = 0; j < mid; j++) {
        p2 = p2->next;
    }
    mergeSort(p, mid);
    mergeSort(p2, i - mid);
    merge(p, mid, p2, i - mid);
}

void LinkedList::merge(Node*& p1, int i1, Node* p2, int i2) {
    int count1 = 0;
    int count2 = 0;
    Node * current = nullptr;
    Node * c1 = p1;
    Node * c2 = p2;
    Node * h = nullptr;
    if (p1->value <= p2->value){
        h = p1;
        count1++;
        c1 = c1->next;
    }else{
        h = p2;
        count2++;
        c2 = c2->next;
    }
    current = h;
    while (count1 < i1 || count2 < i2) {
        //cout <<count1 <<' '<< count2<<endl;//<<' '<< c1->value<<' '<<c2->value <<endl;
        //compare the value between two candidates and pick up the smaller one.
        //use var count1 and count2 to count the amount of nodes that have been proceed, to avoid encountering null pointer.
        //if all of the nodes in one list have been proceeded, pick the node on the other list.
        if (count1 < i1 && (c2 == nullptr ||count2>=i2|| c1->value <= c2->value)) {
            current->next = c1;
            current = current->next;
            c1 = c1->next;
            count1++;
        }
        else if (count2 < i2 && (c1 == nullptr ||count1>=i1 || c1->value > c2->value)) {
            current->next = c2;
            current = current->next;
            c2 = c2->next;
            count2++;
        }
    }
    //always set the next pointer of the tail to null to avoid circle in the result.
    current->next = nullptr;
    p1 = h;
}

void LinkedList::makeRandomList(int m, int n) {

    for (int i = 0; i < m; i++) {
        Node* p1 = new Node(rand() % n);
        p1->next = head;
        head = p1;
        numNodes++;
    }
}

void LinkedList::printList() {
    cout << endl;
    Node* p1 = head;
    while (p1 != nullptr) {
        cout << p1->value << " ";
        p1 = p1->next;
    }
}

int main() {

    LinkedList d1;

    d1.makeRandomList(40, 20);
    d1.printList();

    d1.mergeSort(d1.head, d1.numNodes);
    d1.printList();

    getchar();
    getchar();
    return 0;
}