//Created by Ayon Chattopadhyay
#include <bits/stdc++.h>
#include <fstream>

//to debug use this
#define PRINT(x) printf("%s(%d): %d\n", __func__, __LINE__, (x))
using namespace std;

//  Structure of node. A tree consists of nodes.
struct node{
	node *left;     // points left child
	bool isleft;    // if the current node is not left threaded then isleft is false
	int value;      // stores the data
	bool isright;   // if the current node is not right threaded then isright is false
	node *right;    // points right child
	int count;      // keeps track the number of nodes present in the right subtree of the current node
	
    //  Default constructor
	node(){ 
		left = nullptr;
		right = nullptr;
		isleft = false;
		isright = false;
		count = 0;
		value = 0;
	}

    //  Parameterized constructor
	node(const int &x){
		left = nullptr;
		right = nullptr;
		isleft = false;
		isright = false;
		count = 0;
		value = x;
	}
};

//  listNodes are the nodes for LinkedList class. LinkedList is consisted of listNodes
struct listNode{
	int value;  // Stores the data
	listNode *next; // Points to the next Node

	//  Default constructor
	listNode(){
		value = 0;
		next = nullptr;
	}

	//  Parameterized constructor
	listNode(const int &x){
		value = x;
		next = nullptr;
	}
};


//  Consists of listNodes
//  Two methods are there. 
//  Inserting nodes into the linked list
//  Display the linked list
class linkedList{
public:
	listNode *head;
	void insert(int x);
	void display();

	//Default Constructor
	linkedList(){
		head = nullptr;
	}

	//Parameterized Constructor
	linkedList(const int &x){
		head = new listNode(x);
	}
	
};


//tree class consists of nodes 
class tree{
public:

	node *root; //to save the root of the tree

	 //Default constructor
	tree(){
		root = nullptr;
	}

	//	Parameterized constructor
	tree(const int &x){
		root = new node(x);
	}

	//  Methods Definitions
	void insert(int);
	node* search(int);
	void remove(int);
	node* successor(node*);
	node* predecessor(node*);
	linkedList* reverseInorder();
	linkedList* allElementsBetween(int, int);
	void split(int);
	void write_to_file(node*, ofstream&);
	void printTree();
	int kthElement(int);
	void getInorder(node*);

	//Destructor
	~tree(){
		delete root;
	}
};


//this function inserts at the end of the list
void linkedList::insert(int x){
	listNode *temp = head;
	listNode *ptr = new listNode(x);

	if(head == nullptr){
		head = ptr;
		return;
	}
	while(temp->next != nullptr)
		temp = temp->next;
	temp->next = ptr;
}

//function to display the entire linked list
void linkedList::display(){
	listNode *ptr = head;
	while(ptr != nullptr){
		cout << ptr->value << " ";
		ptr = ptr->next;
	}
	cout << "\n";
}

void tree::insert(int x){
	node *ptr = root; //keeps track of the current pointer
	node *par = nullptr; //keeps track of the parent pointer
	
	//iterating till we don't reach the leaf nodes
	node *f = search(x);
	if(f != nullptr)
		throw "Element already exists";
	while(ptr != nullptr){
		par = ptr;
	
		if(x < ptr->value)
			if(ptr->isleft == false)
				ptr = ptr->left;
			else
				break;
		else
			{
				(ptr->count)++ ;
				if(ptr->isright == false)
					ptr = ptr->right;
				else
					break;
			}
	}
	//declaring a new node
	node *temp = new node(x);
	temp->isleft = true;
	temp->isright = true;

	//Case 1: if the parent is null (first element is inserted)
	if(par == nullptr){
		root = temp;
		temp->left = nullptr;
		temp->right = nullptr;
	}
	//Case 2 : if the new node is inserted in the left side of parent
	else if(x < par->value){
		temp->left = par->left;
		temp->right = par;
		par->isleft = false;
		par->left = temp;
	}
	//Case 3 : if the new node is inserted in the right side of the parent
	else if(x > par->value){
		temp->left = par;
		temp->right = par->right;
		par->isright = false;
		par->right = temp;
	}
}

node* tree::search(int x){
	node *ptr = root;
	while(ptr != nullptr){
		if(x == ptr->value)
			return ptr;
		else if(x < ptr->value)
			//checking if the left pointer is point to a predecessor
			if(!ptr->isleft)
				ptr = ptr->left;
			else break;
		else if(x > ptr->value)
			//checking if the right pointer is pointing to the successor
			if(!ptr->isright)
				ptr = ptr->right;
			else break;
	}
	return nullptr;
}

void tree::remove(int x){
	bool found = false;
	node *ptr = root;
	node *par = nullptr;

	//first checking if the element is present or not
	//to preseve the count in case the element is not present
	if(search(x)) found = true;

	while(found && ptr != nullptr){
		if(ptr->value == x){

			//if the value is present in a node and it has a right child
			//then after deletion the element with which swap will happen
			//will be present on the right subtree, so decreasing the count
			if(ptr->count > 0)
				ptr->count --;
			break;
		}
		par = ptr;
		if(ptr->value > x)
			if(!ptr->isleft)
				ptr = ptr->left;
			else
				break;

		else if(ptr->value < x){
			ptr->count --; //decreasing the count as we are moving to right
			if(!ptr->isright)
				ptr = ptr->right;
			else
				break;
		}
	}

	if(!found)
		throw "Element does not exist\n";
	else{

		//Case 1 : node with both children
		if(!ptr->isleft and !ptr->isright){
			node *parsuc = ptr;
			node *suc = ptr->right;

			while(suc != nullptr){
				if(!suc->isleft){
					parsuc = suc;
					suc = suc->left;
				}
				else break;
			}

			//converting it to a single children or leaf deletion
			//by swaping with the successor
			ptr->value = suc->value;

			//if the successor has no children i.e., leaf node
			if(suc->isleft and suc->isright){
				if(suc == parsuc->left){
					parsuc->isleft = true;
					parsuc->left = suc->left;
				}
				else{
					parsuc->isright = true;
					parsuc->right = suc->right;
				}
			}

			//if the successor has 1 children
			else{
				node *child;
				if(!suc->isleft)
					child = suc->left;
				else
					child = suc->right;
				if(suc == parsuc->left)
					parsuc->left = child;
				else
					parsuc->right = child;
				node *p = predecessor(suc);
				node *s = successor(suc);
				if(!suc->isleft)
					p->right = s;
				else
					s->left = p;
			}
			delete suc;
		}

		//if the node has a left child
		else if(!ptr->isleft){
			node *child = ptr->left;
			if(par == nullptr)
				root = child;
			else if(ptr == par->left)
				par->left = child;
			else
				par->right = child;
			node *p = predecessor(ptr);
			node *s = successor(ptr);
			if(!ptr->isleft)
				p->right = s;
			else
				if(!ptr->isright)
					s->left = p; 

			delete ptr;
		}

		//if the node has a right child
		else if(!ptr->isright){
			node *child = ptr->right;
			if(par == nullptr)
				root = child;
			if(ptr == par->left)
				par->left = child;
			else
				par->right = child;
			node *p = predecessor(ptr);
			node *s = successor(ptr);
			if(!ptr->isleft)
				p->right = s;
			else if(!ptr->isright)
				s->left = p;
			delete ptr;
		}

		//if the node is leaf
		else{
			if(par == nullptr)
				root = nullptr;
			else if(par->left == ptr){
				par->isleft = true;
				par->left = ptr->left;
			}
			else{
				par->isright = true;
				par->right = ptr->right;
			}
			delete ptr;
		}
	}
	
}

linkedList* tree::reverseInorder(){
	if(root == nullptr)
		throw "Tree is empty\n";

	//Reach the rightmost node
	node *ptr = root;
	linkedList *l = new linkedList();
	while(!ptr->isright)
		ptr = ptr->right;

	//One by one print the predecessor
	while(ptr != nullptr){
		// cout << ptr->value << " ";
		l->insert(ptr->value);
		ptr = predecessor(ptr);
	}
	return l;
}

//traverse to the leftmost end then print the values in between k1 and k2
linkedList* tree::allElementsBetween(int k1, int k2){
	linkedList *l = new linkedList();
	node *ptr = root;
	while(ptr->left!=NULL){
		ptr = ptr->left;
	}
    while(ptr){
        if(ptr->value <= k2 && ptr->value >= k1){
            l->insert(ptr->value);
        }
        else if(ptr->value > k2)
            break;
        ptr = successor(ptr);
    }
 
	return l;
}

//utility function to write to the file the output tree
void tree::write_to_file(node *root, ofstream &myfile) {

	if(root == NULL) {
		return;
	}
	if(root->left == NULL){
		myfile << "nullleft" << root->value << "[shape=point];" << "\n";
		myfile << root->value << " -> " << "nullleft" << root->value << ";" << "\n";
	}
	if(root->right == NULL){
		myfile << "nullright" << root->value << "[shape=point];" << "\n";
		myfile << root->value << " -> " << "nullright" << root->value << ";" << "\n";
	}
    if(root->left && root->isleft == false) {
        myfile << root->value << " -> " << root->left->value << ";\n";
    }
	if(root->left && root->isleft == true) {
        myfile << root->value << " -> " << root->left->value << "[color=blue];\n";
    }
    if(root->right && root->isright == false) {
        myfile << root->value << " -> " << root->right->value << ";\n";
    }
	if(root->right && root->isright == true) {
        myfile << root->value << " -> " << root->right->value << "[color=blue];\n";
    }
 
	if(root->left && root->isleft == false)
    	write_to_file(root->left, myfile);
	if(root->right && root->isright == false)
    	write_to_file(root->right, myfile);
}

//prints the tree as a png using graphviz
void tree::printTree(){
  
    ofstream myfile;
    myfile.open ("graph.gv");
    myfile << "digraph {\n";
    node *ptr = root;
    write_to_file(ptr, myfile);

    myfile << "}\n";
    myfile.close();

    string cmd = "dot.exe -Tpng graph.gv -o file.png";
    system((const char*)cmd.c_str());
}

//to find the inorder successor of the node
node* tree::successor(node *ptr){
	if(ptr == nullptr)
		throw "Successor does not exist";
	else if(!ptr->right)
		return nullptr;
	if(ptr->isright)
		return ptr->right;

	ptr = ptr->right;
	while(ptr != nullptr)
		if(!ptr->isleft)
			ptr = ptr->left;
		else
			break;
	return ptr;
}

//to find the inorder predecessor of the node
node* tree::predecessor(node *ptr){
	if(ptr == nullptr)
		throw "Predecessor does not exists";
	else if(!ptr->left)
		return nullptr;
	if(ptr->isleft)
		return ptr->left;
	ptr = ptr->left;
	while(ptr != nullptr)
		if(!ptr->isright)
			ptr = ptr->right;
		else
			break;
	return ptr;
}

void tree::split(int x){
	node *ptr = root;
    vector<node*> afterSplit(2, new node(0));
 
    node* currFirst = afterSplit[0];
    node* currSecond = afterSplit[1];
 
    while (ptr && (ptr->isleft == false || ptr->isright == false)){
 		if(ptr->value <= x){
            currFirst->right = new node(ptr->value);
            currFirst = currFirst->right;
			if(ptr->isleft == false)
            	currFirst->left  = ptr->left;
			if(ptr->isright == false)
            	ptr = ptr->right;
			else break;
        }
 
        else{
            currSecond->left = new node(ptr->value);
            currSecond = currSecond->left;
			if(ptr->isright == false)
            	currSecond->right = ptr->right;
			if(ptr->isleft == false)
            	ptr  = ptr->left;
			else break;
        }
    }
	if(ptr && ptr->isleft == true && ptr->isright == true){
		 if(ptr->value <= x){
            currFirst->right = new node(ptr->value);
        }
 
        else{
            currSecond->left = new node(ptr->value);
        }
	}
    afterSplit[0] = afterSplit[0]->right;
    afterSplit[1]= afterSplit[1]->left;

	cout << "\nFirst BST after split: ";
	getInorder(afterSplit[0]);
	cout << "\n\nSecond BST after split: ";
	getInorder(afterSplit[1]);
	cout << endl;
}

void tree::getInorder(node *ptr) {
	if(ptr == NULL) {
		return;
	}
 
	if(ptr->left && ptr->isleft == false)
    	getInorder(ptr->left);
	cout << ptr->value << " ";
	if(ptr->right && ptr->isright == false)
    	getInorder(ptr->right);
}

//returns the kth largest element
int tree::kthElement(int k){
	if(root == NULL){
		return -1;
	}
	else{
        int m = k;
		node* tempTraverse=root;
		int f = 0;
		while(tempTraverse && (tempTraverse->isright == false || tempTraverse->isleft == false)){
			//if the count of right subtree is same as k then we return it as the kth largest
			if(tempTraverse->count+1==k){
				return tempTraverse->value;
				f = 1;
				break;
			}
			//if the count of the right subtree is less than k the we move to it's left
			//and decrease k by the number of elements already traversed i.e., the right
			//subtree count + 1 for that node
			else if(k>tempTraverse->count+1){
				k = k - (tempTraverse->count+1);
				tempTraverse = tempTraverse->left;
			}
			//else we move to right and check if we get the value k
			else
				tempTraverse = tempTraverse->right;
		}

		//if we reach leaf node then we simply check if the value is reached
		if(tempTraverse && (tempTraverse->isright == true) && (tempTraverse->isleft == true)){
			if(tempTraverse->count+1==k)
			{
				return tempTraverse->value;
				f = 1;
			}
		}
		return -1;
	}
}

//driving function
int main(){
	cout << "This program is authored by Ayon Chattopadhyay (214101012)\n";
	cout << "------------------------------\n";
	tree *t = new tree(); // nullptr --> root
	cout << "Choice 1: Insert\n" << "Choice 2: Search\n" << "Choice 3: Delete\n";
	cout << "Choice 4: Get reverse inorder traversal\n" << "Choice 5: Get inorder successor\n";
	cout << "Choice 6: Split tree\n" << "Choice 7: Get elements between 2 value\n";
	cout << "Choice 8: Find kth largest element\n" << "Choice 9: Print tree\n";
	cout << "Choice 10: Get a sample BST\n";
	cout << "------------------------------\n";

	char ch;
	do{
		cout << "Enter your choice: ";
		int choice, x, k, k1, k2; cin >> choice;
		linkedList *l;
		node *ptr;
		switch(choice){
			case 1:
				cout << "Enter the key value: ";
				cin >> x;
				try{
					t->insert(x);
				}
				catch(const char *e){
					cerr << e << "\n";
				}
				break;
			case 2:
				cout << "Enter the value to search: ";
				cin >> x;
				if(t->search(x)) cout << "Found " << x << "\n";
				else cout << "Not found " << x << "\n";
				break;
			case 3:
				cout << "Enter the key to be deleted: ";
				cin >> x;
				try{
					t->remove(x);
				}
				catch(const char *e){
					cerr << e << "\n";
				}
				break;
			case 4:
				cout << "Reverse inorder traversal is: ";
				l = t->reverseInorder();
				l->display();
				break;
			case 5:
				cout << "Enter the key for inorder successor: ";
				cin >> x;
				try{
					ptr = t->search(x);
					ptr = t->successor(ptr); 
					if(ptr)
						cout << "Successor of " << x << " is: " << ptr->value << "\n";
					else cout << "It is the last element\n";
				}	
				catch(const char *e){
					cerr << e << "\n";
				}
				break;
			case 6:
				cout << "Enter the value to split: ";
				cin >> x;
				t->split(x);
				break;
			case 7:
				cout << "Enter first value: ";
				cin >> k1;
				cout << "Enter second value: ";
				cin >> k2;
				l = t->allElementsBetween(k1, k2);
				l->display();
				break;
			case 8:
				cout << "Enter the value of k: ";
				cin >> k;
				if(t->kthElement(k) != -1)
					cout << k << "th largest element is: " << t->kthElement(k) << "\n";
				else cout << "Invalid value entered!\n";
				break;
			case 9:
				t->printTree();
				break;
			case 10:
				t->insert(50);
				t->insert(30);
				t->insert(40);
				t->insert(20);
				t->insert(36);
				t->insert(76);
				t->insert(3);
				cout << "Open the file to see the sample BST diagram :)\n";
				t->printTree();
				break;
			default:
				cout << "Invalid option entered!\n";
				cout << "Choice 1: Insert\n" << "Choice 2: Search\n" << "Choice 3: Delete\n";
				cout << "Choice 4: Get reverse inorder traversal\n" << "Choice 5: Get inorder successor\n";
				cout << "Choice 6: Split tree\n" << "Choice 7: Get elements between 2 value\n";
				cout << "Choice 8: Find kth largest element\n" << "Choice 9: Print tree\n";
				cout << "Choice 10: Get a sample BST\n";
				cout << "------------------------------\n";
		}
		cout << "Do you want to continue (Y/N)? ";
		cin >> ch;
		cout << "\n";
	}while(ch == 'y' || ch == 'Y');
	return 0;
}