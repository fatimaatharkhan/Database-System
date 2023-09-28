#pragma once
#include "node.h"
#include <iostream>
using namespace std;


// A BTree
template <class T>
class b_tree {
    BTreeNode<T>* root; // Pointer to root node
    int min_deg;           // Minimum degree
public:
    b_tree(int _min_deg)
    {
        root = NULL;
        min_deg = _min_deg;
    }

    void traverse() {
        if (root != NULL)
            root->traverse();
    }

    BTreeNode<T>* search(T k) { return (root == NULL) ? NULL : root->search(k); }

    void insert(T value, int line_no, string file_path) {
        if (root == NULL) {
            root = new BTreeNode(min_deg, true);
            root->keys[0] = value; // Insert key
            root->no_keys = 1;       // Update number of keys in root
        }
        else               // If tree is not empty
        {
            if (root->no_keys == 2 * min_deg - 1) {
                // Allocate memory for new root
                BTreeNode* newRoot = new BTreeNode(min_deg, false);

                newRoot->child[0] = root;

                newRoot->splitChild(0, root);

                int indexToInsert = newRoot->keys[0] < value ? 1 : 0;
                newRoot->child[indexToInsert]->insertNonFull(value);

                root = newRoot;
            }
            else // If root is not full, call insertNonFull for root
                root->insertNonFull(value);
        }
    }
};