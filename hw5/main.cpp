//HW5  AVL Tree
//Due: Friday (Nov. 8) at 11:59PM
//55 points
//This homework requires more efforts. You should get started as soon as possible.

#define RR 1
#define RL 2
#define LL 3
#define LR 4

#define DIREC_L 0
#define DIREC_R 1

#define SUCC 0
#define PRED 1

#include <iostream> //to use cout
#include <algorithm> //to use max function such as  i = max(a, b);

using namespace std;

//You need to use the following node class for each node of the AVL tree
class node {
public:
	int value;
	int height;//this is tree height. Leaf node is 1; empty node (i.e., NIL) is 0
	node* parent;
	node* l_child;
	node* r_child;
	node() {}
	node(int i) { value = i; height = 1; parent = l_child = r_child = nullptr; }
};


class avl_tree {
public:
	node* root;
	avl_tree() {
		root = nullptr;
	}

	//************************************************************************************
	//Implement the following member functions
	void add_node(int i);
	//if case of a tie, add i to the last of existing nodes with value i in the in-order sequence

	void delete_node(int i);
	//Delete the node with value i.  in case of multiple nodes with value i, delete the first node with value i in the in-order traveral sequence


	void in_order_traversal(node* p); //such as 2 5 9 11 11 14 15 15 ...

	pair<node*, int> height_update(node* p);
	/*
	This function will be invoked by add_node and delete_node.
	p points to the first node that we need to check for possible height update.  We then need to check possible height update toward root.
	All nodes whose heights need to be updated will be performed in this function.
	The function returns a pair.  If no imbalance is detected, the first of the pair will be nullptr; otherwise it will be the address of the action node.
	The second of the pair will be 0 if no imbalance is detected; otherwise its value is 1,2,3,4 for RR RL, LL, and LR pattern, respectively.
	*/

	void L_Rotate(node* p);
	//p points to the node at which the rotation will be performed.

	void R_Rotate(node* p);
	//p points to the node at which the rotation will be performed.
};

void avl_tree::in_order_traversal(node* p) {
	if (p == nullptr) {
		return;
	}
	this->in_order_traversal(p->l_child);
	cout << "(" << p->value << ", " << p->height << ") ";
	this->in_order_traversal(p->r_child);
}

void avl_tree::R_Rotate(node* p) {
	node* parent = p->parent;
	int direction;
	if (parent != nullptr) {
		if (p == parent->l_child) {
			direction = DIREC_L;
		}
		else {
			direction = DIREC_R;
		}
	}
	node* pivot = p->l_child;
	p->l_child = nullptr;

	if (pivot->r_child != nullptr) {
		node* temp = pivot->r_child;
		temp->parent = p;
		p->l_child = temp;
	}

	pivot->r_child = p;
	p->parent = pivot;
	if (parent != nullptr) {
		//parent->r_child = pivot;
		//parent_direction = pivot;
		if (direction == DIREC_L) {
			parent->l_child = pivot;
		}
		else if (direction == DIREC_R) {
			parent->r_child = pivot;
		}
	}
	pivot->parent = parent;
	return;
}

void avl_tree::L_Rotate(node* p) {
	node* parent = p->parent;
	int direction;
	if (parent != nullptr) {
		if (p == parent->l_child) {
			direction = DIREC_L;
		}
		else {
			direction = DIREC_R;
		}
	}
	node* pivot = p->r_child;
	p->r_child = nullptr;

	if (pivot->l_child != nullptr) {
		node* temp = pivot->l_child;
		temp->parent = p;
		p->r_child = temp;
	}

	pivot->l_child = p;
	p->parent = pivot;
	if (parent != nullptr) {
		//parent_direction = pivot;
		if (direction == DIREC_L) {
			parent->l_child = pivot;
		}
		else if (direction == DIREC_R) {
			parent->r_child = pivot;
		}
	}
	pivot->parent = parent;
	return;
}


void avl_tree::add_node(int i) {
	node* new_node = new node(i);
	node* current = this->root;
	if (!this->root) {
		this->root = new_node;
		this->root->height = 1;
		return;
	}
	while (true) {
		if (new_node->value < current->value) {
			if (current->l_child != nullptr) {
				current = current->l_child;
			}
			else {
				current->l_child = new_node;
				new_node->parent = current;
				//update height
				break;
			}
		}
		else if (new_node->value >= current->value) {
			if (current->r_child != nullptr) {
				current = current->r_child;
			}
			else {
				current->r_child = new_node;
				new_node->parent = current;
				//update height
				break;
			}
		}
	}
	pair<node*, int> ret = this->height_update(root);
	if (ret.second == LL) {
		this->R_Rotate(ret.first);
		if (ret.first == this->root) {
			this->root = this->root->parent;
		}
	}
	else if (ret.second == RR) {
		this->L_Rotate(ret.first);
		if (ret.first == this->root) {
			this->root = this->root->parent;
		}
	}
	else if (ret.second == RL) {
		this->R_Rotate(ret.first->r_child);
		this->L_Rotate(ret.first);
		if (ret.first == this->root) {
			this->root = this->root->parent;
		}
	}
	else if (ret.second == LR) {
		this->L_Rotate(ret.first->l_child);
		this->R_Rotate(ret.first);
		if (ret.first == this->root) {
			this->root = this->root->parent;
		}
	}
	//cout << "invoke" <<endl;
	//cout << ret.first <<endl;
	//cout << ret.second<<endl;
	//cout << "=======" <<endl;
	pair<node*, int> ret1 = this->height_update(root);//update height again

}

pair<node*, int> avl_tree::height_update(node* p) {
	auto get_balance_factor = [](node* p) -> int {
		int l_height, r_height;
		if (p->l_child == nullptr) {
			l_height = 0;
		}
		else {
			l_height = p->l_child->height;
		}
		if (p->r_child == nullptr) {
			r_height = 0;
		}
		else {
			r_height = p->r_child->height;
		}
		return l_height - r_height;
	};
	if (p == nullptr) {
		return { nullptr,0 };
	}
	else {
		pair<node*, int> l = this->height_update(p->l_child);
		pair<node*, int> r = this->height_update(p->r_child);
		if (l.first != nullptr) {
			return l;
		}
		else if (r.first != nullptr) {
			return r;
		}

		//update height
		int l_height, r_height;
		if (p->l_child == nullptr) {
			l_height = 0;
		}
		else {
			l_height = p->l_child->height;
		}

		if (p->r_child == nullptr) {
			r_height = 0;
		}
		else {
			r_height = p->r_child->height;
		}

		p->height = max(l_height, r_height) + 1;

		int current_balance = get_balance_factor(p);

		//cout << p->value <<endl;
		if (current_balance > 1) {
			if (get_balance_factor(p->l_child) > 0) {
				//ret.imbalance = {p,LL};
				//cout << "LL";
				return { p,LL };
			}
			else if (get_balance_factor(p->l_child) <= 0) {
				//ret.imbalance = {p,LR};
				//cout << "LR";
				return { p,LR };
			}
		}
		else if (current_balance < -1) {
			if (get_balance_factor(p->r_child) <= 0) {
				//ret.imbalance = {p,RR};
				//cout << "RR";
				return { p,RR };
			}
			else if (get_balance_factor(p->r_child) > 0) {
				//ret.imbalance = {p,RL};
				//cout << "RL";
				return { p,RL };
			}
		}
		else {
			return { nullptr,0 };
		}
	}
}




void avl_tree::delete_node(int i) {
	auto is_leaf = [](node* p) -> bool {
		return p->l_child == nullptr && p->r_child == nullptr;
	};
	node* current = this->root;
	while (current != nullptr) {
		if (i == current->value) {
			if (current->l_child != nullptr && current->l_child->value == i) {
				current = current->l_child;
				continue;
			}
			//do delete
			node* to_delete = current;
			if (is_leaf(to_delete)) {
				//is leaf node
				if (to_delete == this->root) {
					//if to_delete is root and only root
					delete(to_delete);
					this->root = nullptr;
					break;
				}
				node* parent = to_delete->parent;
				if (parent->l_child == to_delete) {
					parent->l_child = nullptr;
				}
				else if (parent->r_child == to_delete) {
					parent->r_child = nullptr;
				}
				delete(to_delete);
				//chk for violation
				while (true) {
					pair<node*, int> ret = this->height_update(root);
					if (ret.first == nullptr) {
						break;
					}
					else {
						if (ret.second == LL) {
							this->R_Rotate(ret.first);
							if (ret.first == this->root) {
								this->root = this->root->parent;
							}
						}
						else if (ret.second == RR) {
							this->L_Rotate(ret.first);
							if (ret.first == this->root) {
								this->root = this->root->parent;
							}
						}
						else if (ret.second == RL) {
							this->R_Rotate(ret.first->r_child);
							this->L_Rotate(ret.first);
							if (ret.first == this->root) {
								this->root = this->root->parent;
							}
						}
						else if (ret.second == LR) {
							this->L_Rotate(ret.first->l_child);
							this->R_Rotate(ret.first);
							if (ret.first == this->root) {
								this->root = this->root->parent;
							}
						}
					}
				}
			}
			else if (!(
				(to_delete->l_child != nullptr && !is_leaf(to_delete->l_child))
				||
				(to_delete->r_child != nullptr && !is_leaf(to_delete->r_child))
				)) {
				//is parent of leaf
				if (to_delete->l_child != nullptr) {
					to_delete->value = to_delete->l_child->value;
					to_delete->l_child = nullptr;
				}
				else {
					to_delete->value = to_delete->r_child->value;
					to_delete->r_child = nullptr;
				}
			}
			else {
				//or other cases
				node* pred_current = to_delete->l_child;
				node* predecessor = nullptr;
				while (true) {
					//find predecessor
					if (pred_current->r_child != nullptr) {
						pred_current = pred_current->r_child;
					}
					else {
						predecessor = pred_current;
						break;
					}
				}
				node* succ_current = to_delete->r_child;
				node* successor;
				while (true) {
					//find successor
					if (succ_current->l_child != nullptr) {
						succ_current = succ_current->l_child;
					}
					else {
						successor = succ_current;
						break;
					}
				}
				node* selected = nullptr;
				int selected_type;
				if (current->l_child->height > current->r_child->height) {
					selected = predecessor;
					selected_type = PRED;
				}
				else if (current->r_child->height > current->l_child->height) {
					selected = successor;
					selected_type = SUCC;
				}
				else {
					selected = predecessor;
					selected_type = PRED;
				}

				int temp = current->value;
				current->value = selected->value;
				selected->value = temp;

				if (selected->l_child != nullptr || selected->r_child != nullptr) {
					//have child
					node* child_of_selected;
					int child_of_selected_direction;
					if (selected->l_child != nullptr) {
						child_of_selected = selected->l_child;
						child_of_selected_direction = DIREC_L;
					}
					else /*if (selected->r_child != nullptr)*/ {
						child_of_selected = selected->r_child;
						child_of_selected_direction = DIREC_R;
					}

					//swap with child and selected
					temp = selected->value;
					selected->value = child_of_selected->value;
					child_of_selected->value = temp;

					//remove child of selected
					if (child_of_selected_direction == DIREC_L) {
						child_of_selected->parent->l_child = nullptr;
						delete(child_of_selected);
					}
					else {
						child_of_selected->parent->r_child = nullptr;
						delete(child_of_selected);
					}

				}
				else {
					//node to del is leaf
					if (selected_type == PRED) {
						selected->parent->r_child = nullptr;
						delete(selected);
					}
					else {
						selected->parent->l_child = nullptr;
						delete(selected);
					}
				}
			}
			break;//break after deletion
		}
		else if (i < current->value) {
			current = current->l_child;
		}
		else if (i > current->value) {
			current = current->r_child;
		}
	}
	pair<node*, int> ret = this->height_update(root);
	while (ret.first != nullptr) {
		if (ret.second == LL) {
			this->R_Rotate(ret.first);
			if (ret.first == this->root) {
				this->root = this->root->parent;
			}
		}
		else if (ret.second == RR) {
			this->L_Rotate(ret.first);
			if (ret.first == this->root) {
				this->root = this->root->parent;
			}
		}
		else if (ret.second == RL) {
			this->R_Rotate(ret.first->r_child);
			this->L_Rotate(ret.first);
			if (ret.first == this->root) {
				this->root = this->root->parent;
			}
		}
		else if (ret.second == LR) {
			this->L_Rotate(ret.first->l_child);
			this->R_Rotate(ret.first);
			if (ret.first == this->root) {
				this->root = this->root->parent;
			}
		}
		ret = this->height_update(root);
	}
	return;
}



int main() {
	//Different test cases will be used during grading.
	avl_tree t1;

	t1.in_order_traversal(t1.root);
	t1.delete_node(45);
	t1.in_order_traversal(t1.root);
	t1.add_node(50);
	t1.add_node(46);
	t1.add_node(30);
	t1.add_node(34);
	t1.in_order_traversal(t1.root);
	t1.delete_node(48);
	t1.in_order_traversal(t1.root);
	getchar();
	getchar();
	return 0;
}

//The following is an example showing how to return a pair.
/*
#include <iostream>
using namespace std;

pair<int *, int> f1(){
	return { new int {10}, 10 };
}
int main() {
	cout << *f1().first << " " << f1().second << endl;
	getchar();
	getchar();
	return 0;
}
*/
