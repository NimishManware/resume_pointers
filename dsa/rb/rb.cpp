#include "rb.h"
using ptr = RedBlackTree::ptr;

RedBlackTree::RedBlackTree(){}

const ptr RedBlackTree::getRoot() const
{ 
	return root; 
}

ptr RedBlackTree::insert(int data)
{
	ptr newnodePtr = new node(data);
	if (!root) {
		root = newnodePtr;
		root->color = 0; // set root color as black
		return newnodePtr;
	}
	insert(root, newnodePtr);
	return newnodePtr;
}

// auxiliary function to perform RBT insertion of a node
// you may assume start is not nullptr
void RedBlackTree::insert(ptr start, ptr newnodePtr)
{
	// choose direction
	
	// recurse down the tree

	return;
}

// Credits to Adrian Schneider
void RedBlackTree::printRBT(ptr start, const std::string& prefix, bool isLeftChild) const
{
	if (!start) return;

	std::cout << prefix;
	std::cout << (isLeftChild ? "|--" : "|__" );
	// print the value of the node
	std::cout << start->data << "(" << start->color << ")" << std::endl;
	// enter the next tree level - left and right branch
	printRBT(start->left, prefix + (isLeftChild ? "│   " : "    "), true);
	printRBT(start->right, prefix + (isLeftChild ? "│   " : "    "), false);
}

//Auxiliary function to perform RBT insertion of a node
// You may assume start is not nullptr
void RedBlackTree::insert(ptr start, ptr newnodePtr) {
    // Choose direction
    if (newnodePtr->data < start->data) {
        if (start->left == nullptr) {
            start->left = newnodePtr;
            newnodePtr->parent = start;
        } else {
            insert(start->left, newnodePtr);
        }
    } else {
        if (start->right == nullptr) {
            start->right = newnodePtr;
            newnodePtr->parent = start;
        } else {
            insert(start->right, newnodePtr);
        }
    }
}

// Credits to Adrian Schneider
void RedBlackTree::printRBT(ptr start, const std::string& prefix, bool isLeftChild) const {
    if (!start) return;

    std::cout << prefix;
    std::cout << (isLeftChild ? "|--" : "|__" );
    // print the value of the node
    std::cout << start->data << "(" << start->color << ")" << std::endl;
    // enter the next tree level - left and right branch
    printRBT(start->left, prefix + (isLeftChild ? "│   " : "    "), true);
    printRBT(start->right, prefix + (isLeftChild ? "│   " : "    "), false);
}

// Function performing right rotation
// of the passed node
void RedBlackTree::rightrotate(ptr loc) {
    ptr leftChild = loc->left;
    loc->left = leftChild->right;
    if (leftChild->right != nullptr) {
        leftChild->right->parent = loc;
    }
    leftChild->parent = loc->parent;
    if (loc->parent == nullptr) {
        root = leftChild;
    } else if (loc == loc->parent->left) {
        loc->parent->left = leftChild;
    } else {
        loc->parent->right = leftChild;
    }
    leftChild->right = loc;
    loc->parent = leftChild;
}

// Function performing left rotation
// of the passed node
void RedBlackTree::leftrotate(ptr loc) {
    ptr rightChild = loc->right;
    loc->right = rightChild->left;
    if (rightChild->left != nullptr) {
        rightChild->left->parent = loc;
    }
    rightChild->parent = loc->parent;
    if (loc->parent == nullptr) {
        root = rightChild;
    } else if (loc == loc->parent->left) {
        loc->parent->left = rightChild;
    } else {
        loc->parent->right = rightChild;
    }
    rightChild->left = loc;
    loc->parent = rightChild;
}

// This function fixes violations
// caused by RBT insertion
void RedBlackTree::fixup(ptr loc) {
    while (loc != root && loc->parent->color == 1) {
        if (loc->parent == loc->parent->parent->left) {
            ptr uncle = loc->parent->parent->right;
            if (uncle && uncle->color == 1) {
                loc->parent->color = 0;
                uncle->color = 0;
                loc->parent->parent->color = 1;
                loc = loc->parent->parent;
            } else {
                if (loc == loc->parent->right) {
                    loc = loc->parent;
                    leftrotate(loc);
                }
                loc->parent->color = 0;
                loc->parent->parent->color = 1;
                rightrotate(loc->parent->parent);
            }
        } else {
            ptr uncle = loc->parent->parent->left;
            if (uncle && uncle->color == 1) {
                loc->parent->color = 0;
                uncle->color = 0;
                loc->parent->parent->color = 1;
                loc = loc->parent->parent;
            } else {
                if (loc == loc->parent->left) {
                    loc = loc->parent;
                    rightrotate(loc);
                }
                loc->parent->color = 0;
                loc->parent->parent->color = 1;
                leftrotate(loc->parent->parent);
            }
        }
    }
    root->color = 0;
}

// Function to print inorder traversal
// of the fixated tree
void RedBlackTree::inorder(ptr start) const {
    if (!start) return;

    inorder(start->left);
    std::cout << start->data << " ";
    inorder(start->right);
}

// driver code
int main()
{
	int n; std :: cin >> n;
	assert (n < 10000 && n >= 0);
	int a[10000];
	RedBlackTree tree;

	for (int i = 0; i < n; i++) {
		std::cin >> a[i];

		// allocating memory to the node and initializing:
		// 1. color as red
		// 2. parent, left and right pointers as NULL
		// 3. data as i-th value in the array

		// calling function that performs rbt insertion of
		// this newly created node
		auto newnodePtr = tree.insert(a[i]);

		// calling function to preserve properties of rb
		// tree
		tree.fixup(newnodePtr);
	}
	tree.printRBT(tree.getRoot());

	return 0;
}
