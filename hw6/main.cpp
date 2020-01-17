//CSE674 HW6  Due: 11:59PM, Nov. 25 (Monday)
#include <iostream>
using namespace std;

#define RED true
#define BLACK false

#define NIL -999

#define DIREC_L 0
#define DIREC_R 1

#define PRED 0
#define SUCC 1


class node {
public:
    node* pParent;
    node* p_Lchild;
    node* p_Rchild;
    int value;
    bool color; //true: red, false: black
    node() { pParent = p_Lchild = p_Rchild = nullptr; }
    node(int i) { value = i; pParent = p_Lchild = p_Rchild = nullptr; }
    /*node(int i, bool color, node * parent,  node * left, node * right){
        this->value = i;
        this->color = color;
        this->pParent = parent;
        this->p_Lchild = left;
        this->p_Rchild = right;
    }*/
};

class tree {
public:
    node* root;
    tree() { root = nullptr; }

    //For insert, the new node should be the last of nodes with the same value in the order of
    //inorder traversal.  (The same as HW5.)
    void insert_node(int i);
    void insert_R_rotate(node* p);//For insert_node, R-rotate at node pointed by p
    void insert_L_rotate(node* p);//for insert_node, L-rotate at node pointed by p
    //All other siutions of insert_node should be directly implemented inside add_node


    //For delete, the deleted node shoud be the first of nodes with the same value in the order of
    //inorder traversal. (The same as HW5).
    //For replacement, always recursively replace it with predecessor, unless there is no predecessor. (In this
    //case, replace it with sucessor.)


    void delete_node(int i);
    void delete_1(node* p); //case 1 of delete_node; p points to the parent of double black node
    void delete_1_R_rotate(node* p);//R_totate used in case 1 of delete_node; p points to the parent of double black node
    void delete_1_L_rotate(node* p);//L_totate used in case 1 of delete_node; p points to the parent of double black node
    void delete_2A(node* p); //case 2A of delete_node; p points to the parent of double black node
    void delete_2B(node* p);//case 2B of delete_node; p points to the parent of double black node
    void delete_3(node* p);//case 3 of delete_node; p points to the parent of double black node
    //All other cases of delete_node should be directly implemented inside the delete_node.

    //For the following, during traveral, print (value, color) of each node.
    void InorderPrint(node *);
    void PostorderPrint(node *);
    void PreorderPrint(node *);
    node * global_double_black = nullptr;
};


void tree::InorderPrint(node *p){
    if (!p){
        return;
    }
    char color;
    if (p->color == BLACK){
        color = 'B';
    }else{
        color = 'R';
    }
    this->InorderPrint(p->p_Lchild);
    cout << "(" << p->value << ", "<< color <<") ";
    this->InorderPrint(p->p_Rchild);
}

void tree::PreorderPrint(node *p) {
    if (!p){
        return;
    }
    char color;
    if (p->color == BLACK){
        color = 'B';
    }else{
        color = 'R';
    }
    cout << "(" << p->value << ", "<< color <<") ";
    this->InorderPrint(p->p_Lchild);
    this->InorderPrint(p->p_Rchild);
}

void tree::PostorderPrint(node *p) {
    if (!p){
        return;
    }
    char color;
    if (p->color == BLACK){
        color = 'B';
    }else{
        color = 'R';
    }
    this->InorderPrint(p->p_Lchild);
    this->InorderPrint(p->p_Rchild);
    cout << "(" << p->value << ", "<< color <<") ";
}

void tree::insert_node(int i) {
    node * new_node = new node(i);
    //set default color for new nodes to red
    new_node->color = RED;
    node * current = this->root;
    //case1 new is root
    if (!this->root){
        this->root = new_node;
        this->root->color = BLACK;
        //if new root, set to black
        return;
    }
    while (true){
        if (new_node->value < current->value) {
            if (current->p_Lchild != nullptr){
                current = current->p_Lchild;
            }else{
                current->p_Lchild = new_node;
                new_node->pParent = current;
                break;
            }
        }else if(new_node->value >= current->value){
            if (current->p_Rchild != nullptr){
                current = current->p_Rchild;
            }else {
                current->p_Rchild = new_node;
                new_node->pParent = current;
                break;
            }
        }
    }
    //new_node is the new insert node
    //get grandparent of new insert node

    node * x = new_node;

    before_adj:

    if (x == root){
        x->color = BLACK;
        return;
    }

    node * grandparent = x->pParent->pParent;
    node * uncle = nullptr;
    node * parent = x->pParent;
    if (grandparent){
        if(x->pParent == x->pParent->pParent->p_Lchild){
            uncle = x->pParent->pParent->p_Rchild;
        }else{
            uncle = x->pParent->pParent->p_Lchild;
        }
    }

    if (parent->color == RED /*or x != root*/){
        //case2 new node's parent is black - do nothing

        //case3 (p and) u is red
        //recolor
        if (uncle and uncle->color == RED){
            parent->color = BLACK;
            uncle->color = BLACK;
            grandparent->color = RED;
            //if (!grandparent->pParent){
                //if grandparent is root
            //    grandparent->color = BLACK;
            //}
            x = grandparent;
            goto before_adj;
        }

        //case4 (p is red and) u is black or null
        if (grandparent and (!uncle or uncle->color == BLACK)){
            //Left Left Case (p is left child of g and x is left child of p)
            if(x == parent->p_Lchild and parent == grandparent->p_Lchild){
                //right rotate at g, swap color of g and p
                this->insert_R_rotate(grandparent);
                int tempcolor = grandparent->color;
                grandparent->color = parent->color;
                parent->color = tempcolor;
                //switch root in case g is root
                if (grandparent == root){
                    this->root = grandparent->pParent;
                }
            }
            //Left Right Case (p is left child of g and x is right child of p)
            else if(x == parent->p_Rchild and parent == grandparent->p_Lchild){
                //left rotate at p
                this->insert_L_rotate(parent);
                this->insert_R_rotate(grandparent);

                if (grandparent == root){
                    this->root = grandparent->pParent;
                }
                int tempcolor = grandparent->color;
                grandparent->color = x->color;
                x->color = tempcolor;
                /*int tempcolor = grandparent->color;
                grandparent->color = parent->color;
                parent->color = tempcolor;*/
                //x = x->p_Rchild;

            }
            //Right Right Case (Mirror of case a)
            else if(x == parent->p_Rchild and parent == grandparent->p_Rchild){
                //left rotate at g
                this->insert_L_rotate(grandparent);
                //swap color of g and p
                int tempcolor = grandparent->color;
                grandparent->color = parent->color;
                parent->color = tempcolor;
                //switch root in case g is root
                if (grandparent == root){
                    this->root = grandparent->pParent;
                }
            }
            //Right Left Case (Mirror of case c)
            else if (x == parent->p_Lchild and parent == grandparent->p_Rchild){
                //right rotate at p
                this->insert_R_rotate(parent);
                this->insert_L_rotate(grandparent);
                if (grandparent == root){
                    this->root = grandparent->pParent;
                }
                int tempcolor = grandparent->color;
                grandparent->color = x->color;
                x->color = tempcolor;
            }

        }

    }
    return;
}

void tree::insert_R_rotate(node *p) {
    node * parent = p->pParent;
    int direction;
    if (parent != nullptr){
        if (p==parent->p_Lchild){
            direction = DIREC_L;
        }else{
            direction = DIREC_R;
        }
    }
    node * pivot = p->p_Lchild;
    p->p_Lchild = nullptr;

    if(pivot->p_Rchild != nullptr){
        node * temp = pivot->p_Rchild;
        temp->pParent = p;
        p->p_Lchild = temp;
    }

    pivot->p_Rchild = p;
    p->pParent = pivot;
    if (parent != nullptr) {
        //parent->p_Rchild = pivot;
        //parent_direction = pivot;
        if (direction == DIREC_L){
            parent->p_Lchild = pivot;
        }else if(direction == DIREC_R){
            parent->p_Rchild = pivot;
        }
    }
    pivot->pParent = parent;
    return;
}

void tree::insert_L_rotate(node *p) {
    node * parent = p->pParent;
    int direction;
    if (parent != nullptr){
        if (p==parent->p_Lchild){
            direction = DIREC_L;
        }else{
            direction = DIREC_R;
        }
    }
    node * pivot = p->p_Rchild;
    p->p_Rchild = nullptr;

    if(pivot->p_Lchild != nullptr) {
        node *temp = pivot->p_Lchild;
        temp->pParent = p;
        p->p_Rchild = temp;
    }

    pivot->p_Lchild = p;
    p->pParent = pivot;
    if (parent != nullptr){
        //parent_direction = pivot;
        if (direction == DIREC_L){
            parent->p_Lchild = pivot;
        }else if(direction == DIREC_R){
            parent->p_Rchild = pivot;
        }
    }
    pivot->pParent = parent;
    return;
}


void tree::delete_node(int i){
    auto is_leaf = [](node * p) -> bool {
        return p->p_Lchild == nullptr and p->p_Rchild == nullptr;
    };
    node * current = this->root;
    while (current){
        if (i == current->value){
            if (current->p_Lchild != nullptr && current->p_Lchild->value == i){
                current = current->p_Lchild;
                continue;
            }
            //do_delete
            node * x = current;
            //if remove root
            if (current == this->root and !root->p_Lchild and !root->p_Rchild){
                this->root = nullptr;
                delete(current);
                return;
            }

            //other cases
            //need to find predcessor and successor
            node * pred_current = x->p_Lchild;
            node * predecessor = nullptr;
            while (true){
                //find predecessor
                if (pred_current and pred_current->p_Rchild != nullptr){
                    pred_current = pred_current->p_Rchild;
                }else{
                    predecessor = pred_current;
                    break;
                }
            }
            node * succ_current = x->p_Rchild;
            node * successor;
            while(true){
                //find successor
                if (succ_current and succ_current->p_Lchild != nullptr){
                    succ_current = succ_current->p_Lchild;
                }else{
                    successor = succ_current;
                    break;
                }
            }
            node * selected = nullptr;
            int selected_type;

            /*if (current->l_child->height > current->r_child->height){
                selected = predecessor;
                selected_type = PRED;
            }else if(current->r_child->height > current->l_child->height){
                selected = successor;
                selected_type = SUCC;
            }else{
                selected = predecessor;
                selected_type = PRED;
            }*/
            if (!predecessor){
                selected = successor;
                selected_type = SUCC;
            }else{
                selected = predecessor;
                selected_type = PRED;
            }

            if (selected){
                //swap value and color with x and selected
                int tempv = x->value;
                //int tempc = x->color;
                x->value = selected->value;
                //x->color = selected->color;
                selected->value = tempv;
                //selected->color = tempc;

                x = selected;
            }

            bool u_color;
            node * u = nullptr;
            if (x->p_Lchild){
                u = x->p_Lchild;
                u_color = u->color;
            }else if (x->p_Rchild){
                u = x->p_Rchild;
                u_color = u->color;
            }else if(!x->p_Lchild and !x->p_Rchild){//x is leaf
                u = nullptr;
                u_color = BLACK;
            }
            //either u or v is black.
            //remove u and replace it with v

            if ((!x->p_Lchild and !x->p_Rchild) and x->color == RED){
                //x is red leaf
                node * p = x->pParent;
                if (p->p_Lchild and p->p_Lchild == x){
                    //x is left of parent
                    p->p_Lchild = nullptr;
                }else{
                    p->p_Rchild = nullptr;
                }
                delete(x);
                break;
            }


            //double black
            //double black case only happen when v is leaf.
            //because two consecutive black is not allowed
            if(x->color == BLACK and u_color == BLACK){
                x->value = NIL;

                node * s = nullptr;
                if (x->pParent->p_Lchild and x->pParent->p_Lchild->value != NIL){
                    s = x->pParent->p_Lchild;
                }else if(x->pParent->p_Rchild and x->pParent->p_Rchild->value != NIL){
                    s = x->pParent->p_Rchild;
                }
                bool sib_l_color, sib_r_color;
                if(s->p_Lchild){
                    sib_l_color = s->p_Lchild->color;
                }else{
                    sib_l_color = BLACK;
                }

                if (s->p_Rchild){
                    sib_r_color = s->p_Rchild->color;
                }else{
                    sib_r_color = BLACK;
                }
                //case1 - one or two of child nodes of s is red.
                //parameter pointer points to parent of double black.
                if (s->color == BLACK and (sib_l_color == RED or sib_r_color == RED)){
                    delete_1(x->pParent);
                    /*
                    node * r = nullptr;
                    if (sib_r_color == RED){
                        r = s->p_Rchild;
                    }else{
                        r = s->p_Lchild;
                    }*/
                }else if (s->color == BLACK and (sib_l_color != RED and sib_r_color != RED)){
                    //case2 - sib is black, no child of s is black
                    //case2A - parent of nil is black
                    if (x->pParent->color == BLACK){
                        delete_2A(x->pParent);
                    }
                    //case2B - parent of nil is red
                    if (x->pParent->color == RED){
                        delete_2B(x->pParent);
                    }
                }else if(s->color == RED){
                    //case3 - sib is red
                    delete_3(x->pParent);
                }
                //actually remove x
                node * p = x->pParent;
                node * rm;
                //node * s,
                node * r;
                int direction;
            if (p->p_Rchild and p->p_Rchild->value == NIL){
                    //node to remove is right of p
                    s = p->p_Lchild;
                    rm = p->p_Rchild;
                    direction = DIREC_R;
                }else{
                    //node to remove is left of p
                    s = p->p_Rchild;
                    rm = p->p_Lchild;
                    direction = DIREC_L;
                }
                if (direction == DIREC_L){
                    p->p_Lchild = nullptr;
                } else{
                    p->p_Rchild = nullptr;
                }
                delete(rm);


            }else{
                //u and v is red and black
                node * p = x->pParent;
                if (p->p_Lchild and p->p_Lchild == x){
                    //x is left of parent
                    p->p_Lchild = u;
                }else{
                    p->p_Rchild = u;
                }
                delete(x);
                u->color = BLACK;
                u->pParent = p;
            }

            break;
        }else if (i > current->value){
            current = current->p_Rchild;
        }else if (i < current->value){
            current = current->p_Lchild;
        }
    }
}

void tree::delete_1(node* p){
    //case 1 of delete_node; p points to the parent of double black node
    node * rm;
    node * s, * r;
    int direction;

    /*if (p->p_Lchild and p->p_Lchild->value != NIL){
        //node to remove is right of p
        s = p->p_Lchild;
        rm = p->p_Rchild;
        direction = DIREC_R;
    }else{
        //node to remove is left of p
        s = p->p_Rchild;
        rm = p->p_Lchild;
        direction = DIREC_L;
    }*/
    if (p->p_Lchild and (p->p_Lchild->value == NIL or p->p_Lchild == this->global_double_black)){
        //double black is left of p
        s = p->p_Rchild;
        rm = p->p_Lchild;
        direction = DIREC_L;
    }else{
        //double black is right of p
        s = p->p_Lchild;
        rm = p->p_Rchild;
        direction = DIREC_R;
    }
    /*
    if (direction == DIREC_L){
        p->p_Lchild = nullptr;
    } else{
        p->p_Rchild = nullptr;
    }
    delete(rm);*/

    bool root_flag = false;
    if (p == this->root){
        root_flag = true;
    }
    if (direction == DIREC_R){
        //Lx case
        if (s->p_Lchild and s->p_Lchild->color == RED){
            //ll pattern - left child of s is red
            //R rotate at parent of s
            r = s->p_Lchild;
            node * s_original_parent = s->pParent;
            delete_1_R_rotate(s->pParent);
            r->color = BLACK;
            //swap color of p and s
            /*bool temp = s->color;
            s->color = s->pParent->color;
            s->pParent->color = temp;*/
            bool temp = s->color;
            s->color = s_original_parent->color;
            s_original_parent->color = temp;
            if (root_flag){
                this->root = s;
            }
        }else if (s->p_Rchild and s->p_Rchild->color == RED){
            //lr pattern - right child of s is red
            //l rotate at s
            //then r rotate at parent of r
            r = s->p_Rchild;

            //====
            bool p_color = s->pParent->color;
            s->pParent->color = BLACK;
            r->color = p_color;
            //=======
            delete_1_L_rotate(s);
            //swap color with s and its new parent
            /*
            bool temp = s->color;
            s->color = s->pParent->color;
            s->pParent->color = temp;*/
            delete_1_R_rotate(r->pParent);
            //s->color = BLACK;//s = r->parent
            if (root_flag){
                this->root = r;
            }
        }


    }else{
        //Rx case
        if (s->p_Rchild and s->p_Rchild->color == RED){
            //rr pattern - right child of s is red
            //L rotate at parent of s
            r = s->p_Rchild;
            node * s_original_parent = s->pParent;
            delete_1_L_rotate(s->pParent);
            r->color = BLACK;
            //swap color of p and s
            bool temp = s->color;
            s->color = s_original_parent->color;
            s_original_parent->color = temp;
            if (root_flag){
                this->root = s;
            }
        }else if (s->p_Lchild and s->p_Lchild->color == RED){
            //rl pattern - right child of s is red
            //r rotate at s
            //then l rotate at parent of r
            r = s->p_Lchild;
            bool p_color = s->pParent->color;
            s->pParent->color = BLACK;
            r->color = p_color;
            delete_1_R_rotate(s);
            //swap color with s and its new parent
            //bool temp = s->color;
            //s->color = s->pParent->color;
            //s->pParent->color = temp;
            delete_1_L_rotate(r->pParent);
            //s->color = BLACK;//s = r->parent
            if (root_flag){
                this->root = r;
            }
        }
    }

}

void tree::delete_2A(node *p) {
    node * rm;
    node * s, * r;
    int direction;
    if (p->p_Rchild and p->p_Rchild->value == NIL){
        //node to remove is right of p
        s = p->p_Lchild;
        rm = p->p_Rchild;
        direction = DIREC_R;
    }else{
        //node to remove is left of p
        s = p->p_Rchild;
        rm = p->p_Lchild;
        direction = DIREC_L;
    }
    /*
    if (direction == DIREC_L){
        p->p_Lchild = nullptr;
    } else{
        p->p_Rchild = nullptr;
    }*/

    //node * double_black = rm->pParent;
    node * double_black = rm;


    //change color of s
    //s->color = !s->color;
    //delete(rm);

    while (double_black->pParent /*and double_black->pParent->color == BLACK*/){
        p = double_black->pParent;
        if (p->p_Lchild and p->p_Lchild==double_black){
            s = p->p_Rchild;
            direction = DIREC_L;//double black on the left of parent
        }else{
            s = p->p_Lchild;
            direction = DIREC_R;//double black on the right of parent
        }

        if (s and s->color == RED){
            //goto case 3
            this->global_double_black = double_black;
            delete_3(p);
            //not sure, test
            break;

        }else if (s and s->color == BLACK and (
                (s->p_Lchild and s->p_Lchild->color == RED) or (s->p_Rchild and s->p_Rchild->color == RED)
                )){
            //s == black and have at least one red child
            this->global_double_black = double_black;
            delete_1(p);
            break;//not sure

        }else if(s and s->color == BLACK and ((!s->p_Lchild or s->p_Lchild->color == BLACK) and (!s->p_Rchild or s->p_Rchild->color == BLACK)) and s->pParent->color == RED){
            //sib black, sib no red child, sib parent red -> sib to red, sib parent to black
            s->pParent->color = BLACK;
            s->color = RED;
            break;
        }else if(p->color == RED){
            //s->color = RED;//when parent = red, simply make parent black
            p->color = BLACK;
            break;
        }else {
            // s->color == BLACK and without red child
            s->color = RED;
        }
        if (!double_black->pParent){
            break;
        }else{
            double_black = double_black->pParent;
        }
    }
}

void tree::delete_2B(node *p) {
    node * rm;
    node * s, * r;
    int direction;
    if (p->p_Lchild and (p->p_Lchild->value == NIL or p->p_Lchild == this->global_double_black)){
        /*node to remove is right of p
        s = p->p_Lchild;
        rm = p->p_Rchild;
        direction = DIREC_R;
        */
        //double black is left of p
        s = p->p_Rchild;
        rm = p->p_Lchild;
        direction = DIREC_L;
    }else{
        /*node to remove is left of p
        s = p->p_Rchild;
        rm = p->p_Lchild;
        direction = DIREC_L;*/

        //double black is right of p
        s = p->p_Lchild;
        rm = p->p_Rchild;
        direction = DIREC_R;
    }

    /*
    if (direction == DIREC_L){
        p->p_Lchild = nullptr;
    } else{
        p->p_Rchild = nullptr;
    }
    delete(rm);*/
    s->color = RED;
    p->color = BLACK;
    return;
}

void tree::delete_3(node* p){
    //case 3 of delete_node; p points to the parent of double black node
    node * rm;
    node * s, * r;
    int direction;
    if (p->p_Lchild and (p->p_Lchild->value == NIL or p->p_Lchild == this->global_double_black)){
        //double black is left of p
        s = p->p_Rchild;
        rm = p->p_Lchild;
        direction = DIREC_L;
    }else{
        //double black is right of p
        s = p->p_Lchild;
        rm = p->p_Rchild;
        direction = DIREC_R;
    }
    /*if (direction == DIREC_L){
        p->p_Lchild = nullptr;
    } else{
        p->p_Rchild = nullptr;
    }
    delete(rm);*/
    if (direction == DIREC_L){
        //r pattern, l rotate at p
        delete_1_L_rotate(p);
    }else{
        delete_1_R_rotate(p);
    }
    if (p== this->root){
        this->root = s;
    }
    //reverse color of p and s
    if (s->color == RED){
        s->color = BLACK;
    }
    if (p->color == BLACK){
        p->color = RED;
    }

    //re calc s and p
    p = rm->pParent;
    if (p->p_Lchild and (p->p_Lchild->value == NIL or p->p_Lchild == this->global_double_black)){
        //double black is left of p
        s = p->p_Rchild;
        rm = p->p_Lchild;
        direction = DIREC_L;
    }else{
        //double black is right of p
        s = p->p_Lchild;
        rm = p->p_Rchild;
        direction = DIREC_R;
    }

    //p->color = !p->color;
    //s->color = !s->color;
    //if another child of s(other than nil) have 2 black child, goto case2b
    bool sib_l_color, sib_r_color;
    if(s->p_Lchild){
        sib_l_color = s->p_Lchild->color;
    }else{
        sib_l_color = BLACK;
    }

    if (s->p_Rchild){
        sib_r_color = s->p_Rchild->color;
    }else{
        sib_r_color = BLACK;
    }
    if (s->color == BLACK and (sib_l_color != RED and sib_r_color != RED)){
        node * x = rm;
        //if (x->pParent->color == BLACK){
        //    //TODO check upper
        //    delete_2A(x->pParent);
        //}
        //case2B - parent of nil is red
        if (x->pParent->color == RED){
            delete_2B(x->pParent);
        }
    }

}

void tree::delete_1_L_rotate(node *p) {
    node * parent = p->pParent;
    int direction;
    if (parent != nullptr){
        if (p==parent->p_Lchild){
            direction = DIREC_L;
        }else{
            direction = DIREC_R;
        }
    }
    node * pivot = p->p_Rchild;
    p->p_Rchild = nullptr;

    if(pivot->p_Lchild != nullptr) {
        node *temp = pivot->p_Lchild;
        temp->pParent = p;
        p->p_Rchild = temp;
    }

    pivot->p_Lchild = p;
    p->pParent = pivot;
    if (parent != nullptr){
        //parent_direction = pivot;
        if (direction == DIREC_L){
            parent->p_Lchild = pivot;
        }else if(direction == DIREC_R){
            parent->p_Rchild = pivot;
        }
    }
    pivot->pParent = parent;
    return;
}

void tree::delete_1_R_rotate(node* p){
    node * parent = p->pParent;
    int direction;
    if (parent != nullptr){
        if (p==parent->p_Lchild){
            direction = DIREC_L;
        }else{
            direction = DIREC_R;
        }
    }
    node * pivot = p->p_Lchild;
    p->p_Lchild = nullptr;

    if(pivot->p_Rchild != nullptr){
        node * temp = pivot->p_Rchild;
        temp->pParent = p;
        p->p_Lchild = temp;
    }

    pivot->p_Rchild = p;
    p->pParent = pivot;
    if (parent != nullptr) {
        //parent->p_Rchild = pivot;
        //parent_direction = pivot;
        if (direction == DIREC_L){
            parent->p_Lchild = pivot;
        }else if(direction == DIREC_R){
            parent->p_Rchild = pivot;
        }
    }
    pivot->pParent = parent;
    return;
}

//testcases def
void test_rm_not_exist();
void test_0_31();
void test_same();
void test_31_0();
void test_rm2();
void test_rm_1();
void test_1_10();
void test_case1();

int main(){

    test_case1();
    getchar();

    test_1_10();

     //test_rm_not_exist();
     test_0_31();
     test_same();
     test_31_0();
     getchar();
     //test_rm2();
     //test_rm_1();//*/

    tree rbtree = tree();
    rbtree.insert_node(50);
    rbtree.insert_node(100);
    rbtree.insert_node(200);
    rbtree.insert_node(150);
    rbtree.insert_node(300);
    rbtree.delete_node(150);
    return 0;
}

void test_case1(){
    tree nt;
    nt.insert_node(50);
    nt.InorderPrint(nt.root); cout << endl;
    nt.insert_node(40);
    nt.InorderPrint(nt.root); cout << endl;
    nt.insert_node(20);
    nt.InorderPrint(nt.root); cout << endl;
    nt.insert_node(35);
    nt.InorderPrint(nt.root); cout << endl;
    nt.insert_node(30);
    nt.InorderPrint(nt.root); cout << endl;
    nt.insert_node(10);
    nt.InorderPrint(nt.root); cout << endl;
    nt.insert_node(5);
    nt.InorderPrint(nt.root); cout << endl;
    nt.insert_node(25);
    nt.InorderPrint(nt.root); cout << endl;
    nt.insert_node(45);
    nt.InorderPrint(nt.root); cout << endl;
    nt.insert_node(15);
    nt.InorderPrint(nt.root); cout << endl;

    nt.insert_node(50);
    nt.InorderPrint(nt.root); cout << endl;
    nt.insert_node(40);
    nt.InorderPrint(nt.root); cout << endl;
    nt.insert_node(30);
    nt.InorderPrint(nt.root); cout << endl;
    nt.insert_node(40);
    nt.InorderPrint(nt.root); cout << endl;
    nt.insert_node(10);
    nt.InorderPrint(nt.root); cout << endl;

    nt.insert_node(40);
    nt.InorderPrint(nt.root); cout << endl;
    nt.insert_node(40);
    nt.InorderPrint(nt.root); cout << endl;
    nt.insert_node(40);
    nt.InorderPrint(nt.root); cout << endl;
    nt.insert_node(40);
    nt.InorderPrint(nt.root); cout << endl;
    nt.insert_node(40);
    nt.InorderPrint(nt.root); cout << endl;
    nt.insert_node(40);
    nt.InorderPrint(nt.root); cout << endl;
    nt.insert_node(40);
    nt.InorderPrint(nt.root); cout << endl;
    //========
    nt.delete_node(30);
    nt.InorderPrint(nt.root); cout << endl; cout << endl;
    nt.delete_node(50);
    nt.InorderPrint(nt.root); cout << endl; cout << endl;
    nt.delete_node(40);//1
    nt.InorderPrint(nt.root); cout << endl; cout << endl;
    nt.delete_node(40);//2
    nt.InorderPrint(nt.root); cout << endl; cout << endl;
    nt.delete_node(40);//3
    nt.InorderPrint(nt.root); cout << endl; cout << endl;
    nt.delete_node(40);//4
    nt.InorderPrint(nt.root); cout << endl; cout << endl;
    nt.delete_node(40);//5
    nt.InorderPrint(nt.root); cout << endl; cout << endl;
    nt.delete_node(40);//6th
    nt.InorderPrint(nt.root); cout << endl; cout << endl;
    nt.delete_node(40);//7
    nt.InorderPrint(nt.root); cout << endl; cout << endl;
    nt.delete_node(40);//8
    nt.InorderPrint(nt.root); cout << endl; cout << endl;
    nt.delete_node(40);
    nt.InorderPrint(nt.root); cout << endl; cout << endl;
    nt.delete_node(40);
    nt.InorderPrint(nt.root); cout << endl; cout << endl;
    nt.delete_node(30);
    nt.InorderPrint(nt.root); cout << endl; cout << endl;


}

/*
//=====test cases
void test_rm_1(){
    tree rb_tree = tree();
    node* root = new node(10, BLACK, nullptr, nullptr, nullptr);
// Left subtree;
    node* node_5 = new node(5, RED, root, nullptr, nullptr);
    node* node_m5 = new node(-5, BLACK, node_5, nullptr, nullptr);
    node* node_7 = new node(7, BLACK, node_5, nullptr, nullptr);
    node_5->p_Lchild = node_m5;
    node_5->p_Rchild = node_7;
    node* node_6 = new node(6, RED, node_7, nullptr, nullptr);
    node_7->p_Lchild = node_6;
// right subtree;
    node* node_35 = new node(35, RED, root, nullptr, nullptr);
    node* node_20 = new node(20, BLACK, node_35, nullptr, nullptr);
    node* node_38 = new node(38, BLACK, node_35, nullptr, nullptr);
    node_35->p_Lchild = node_20;
    node_35->p_Rchild = node_38;
    node* node_36 = new node(36, RED, node_38, nullptr, nullptr);
    node_38->p_Lchild = node_36;
    root->p_Lchild = node_5;
    root->p_Rchild = node_35;
    rb_tree.root = root;
    rb_tree.delete_node(5);
    rb_tree.InorderPrint(root);

}

void test_rm2(){
    tree rb_tree = tree();
    node * root = new node(10,BLACK,nullptr, nullptr, nullptr);
    node * node_m10 = new node(-10,BLACK,root, nullptr, nullptr);
    node * node_30 = new node(30,BLACK,root,nullptr,nullptr);
    root->p_Lchild = node_m10;
    root->p_Rchild = node_30;
    rb_tree = tree();
    rb_tree.root = root;
    rb_tree.delete_node(-10);

}
 */

void test_1_10(){
    tree rbtree = tree();
    rbtree.insert_node(1);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(2);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(3);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(4);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(5);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(6);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(7);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(8);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(9);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;

    rbtree.delete_node(5);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(6);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(7);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(8);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(9);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;


    rbtree.delete_node(4);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(3);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(2);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(1);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
}

void test_0_31(){
    tree rbtree = tree();
    rbtree.insert_node(0);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(1);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(2);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(3);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(4);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(5);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(6);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(7);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(8);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(9);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(11);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(12);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(13);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(14);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(15);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(16);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(17);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(18);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(19);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(20);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(21);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(22);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(23);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(24);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(25);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(26);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(27);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(28);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(29);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(30);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(31);
    //=================
    rbtree.delete_node(0);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(1);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(2);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(3);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(4);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(5);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(6);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(7);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(8);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(9);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(11);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(12);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(13);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(14);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(15);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(16);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(17);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(18);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(19);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(20);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(21);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(22);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(23);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(24);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(25);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(26);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(27);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(28);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(29);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(30);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(31);
}

void test_same(){
    tree rbtree = tree();
    rbtree.insert_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    //======
    rbtree.delete_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;

}

void test_31_0(){
    tree rbtree = tree();
    rbtree.insert_node(0);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(1);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(2);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(3);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(4);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(5);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(6);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(7);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(8);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(9);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(10);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(11);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(12);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(13);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(14);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(15);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(16);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(17);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(18);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(19);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(20);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    //
    rbtree.insert_node(21);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(22);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(23);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(24);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(25);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(26);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(27);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(28);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(29);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(30);
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.insert_node(31);
    //=================
    rbtree.delete_node( 31 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 30 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 29 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 28 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 27 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 26 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 25 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 24 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 23 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 22 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 21 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    //
    rbtree.delete_node( 20 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 19 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 18 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 17 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 16 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 15 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 14 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 13 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 12 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 11 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 10 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 9 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 8 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 7 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 6 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 5 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 4 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 3 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 2 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 1 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
    rbtree.delete_node( 0 );
    rbtree.InorderPrint(rbtree.root);cout<<endl;
}

void test_rm_not_exist(){
    tree rb_tree = tree();
    rb_tree.insert_node(50);
    rb_tree.insert_node(100);
    rb_tree.insert_node(200);
    rb_tree.insert_node(150);
    rb_tree.insert_node(300);
    //rb_tree.delete_node(200);
    rb_tree.delete_node(114514);
}

void test_insert(){
    tree rb_tree = tree();
    rb_tree.insert_node(2);
    rb_tree.insert_node(1);
    rb_tree.insert_node(4);
    rb_tree.insert_node(5);
    rb_tree.insert_node(9);
    rb_tree.insert_node(3);
    rb_tree.insert_node(6);
    rb_tree.insert_node(7);
    rb_tree.insert_node(15);
}

