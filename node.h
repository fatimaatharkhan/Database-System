#pragma once

template <class T>
class rb_node
{
public:
	T key;
	int line_no;
	string file_path;
	rb_node* left;
	rb_node* right;
	rb_node* parent;
	int color; //1 if red, 0 if black
	int size;
	bool duplicate;
	rb_node* duplicates; // in case there are more than one nodes with the same key, their nodes will be saved here.
	int no_of_duplicates;
	rb_node(T k, int l, string p, int c = 1)
	{
		key = k;
		line_no = l;
		file_path = p;
		color = c;
		left = NULL;
		right = NULL;
		parent = NULL;
		duplicate = false;
		duplicates = NULL;
		no_of_duplicates = 0;
	}
	rb_node()
	{
		line_no = -1;
		left = NULL;
		right = NULL;
		parent = NULL;
		duplicates = NULL;
		no_of_duplicates = 0;
	}
};

template <class T>
class key_node
{
public:
	T key;
	int line_no;
	string file_path;
	bool duplicate;
	key_node* duplicates; // in case there are more than one nodes with the same key, their nodes will be saved here.
	int no_of_duplicates;
	key_node(T k, int l, string p)
	{
		key = k;
		line_no = l;
		file_path = p;
		duplicate = false;
		duplicates = NULL;
		no_of_duplicates = 0;
	}
	key_node()
	{
		line_no = -1;
		duplicate = false;
		duplicates = NULL;
		no_of_duplicates = 0;
	}
};

// A BTree node
template <class T>
class BTreeNode {
    key_node<T>* keys;     // An array of keys
    int min_keys;         // Minimum degree (defines the range for number of keys)
    BTreeNode<T>** child; // An array of child pointers
    int no_keys;
    bool isLeaf;

public:
    BTreeNode(int min_keys1, bool isLeaf1) {
        min_keys = min_keys1;
        isLeaf = isLeaf1;

        keys = new key_node<T>[2 * min_keys - 1];
        child = new BTreeNode * [2 * min_keys];

        no_keys = 0;
    }
    void insertNonFull(int value) {
        int i = no_keys - 1;

        if (isLeaf == true) {
            while (i >= 0 && keys[i] > value) {
                keys[i + 1] = keys[i];
                i--;
            }

            keys[i + 1] = value;
            no_keys = no_keys + 1;
        }
        else // If this node is not isLeaf
        {
            while (i >= 0 && keys[i] > value)
                i--;

            if (child[i + 1]->no_keys == 2 * min_keys - 1) {
                splitChild(i + 1, child[i + 1]);

                if (keys[i + 1] < value)
                    i++;
            }
            child[i + 1]->insertNonFull(value);
        }
    }

    void splitChild(int i, BTreeNode* y) {
        BTreeNode* z = new BTreeNode(y->min_keys, y->isLeaf);
        z->no_keys = min_keys - 1;

        for (int j = 0; j < min_keys - 1; j++)
            z->keys[j] = y->keys[j + min_keys];

        if (y->isLeaf == false) {
            for (int j = 0; j < min_keys; j++)
                z->child[j] = y->child[j + min_keys];
        }

        y->no_keys = min_keys - 1;

        for (int j = no_keys; j >= i + 1; j--)
            child[j + 1] = child[j];

        child[i + 1] = z;

        for (int j = no_keys - 1; j >= i; j--)
            keys[j + 1] = keys[j];

        keys[i] = y->keys[min_keys - 1];

        no_keys = no_keys + 1;
    }
    void traverse() {
        int i;
        for (i = 0; i < no_keys; i++) {
            if (isLeaf == false)
                child[i]->traverse();
            cout << " " << keys[i];
        }

        if (isLeaf == false)
            child[i]->traverse();
    }

    BTreeNode* search(int k) {
        int i = 0;
        while (i < no_keys && k > keys[i])
            i++;

        if (keys[i] == k)
            return this;

        if (isLeaf == true)
            return NULL;

        return child[i]->search(k);
    }

    friend class BTree;
};