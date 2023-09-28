#pragma once
#include "node.h"
#include <iostream>
using namespace std;

template <class T>
class rb_tree
{
public:
    rb_node<T>* root;

    rb_tree()
    {
        root = NULL;
    }

    void fix_insert(rb_node<T>* temp_node) {
        if (temp_node->parent == NULL) {// if no parent, it means it is the root
            return;
        }
        for (; temp_node->parent != NULL && temp_node->parent->color == 1;)
        {    // while parent of temp_node is red, this loop will work
             // or else it means that the parent is black and the tree is also balanced
            rb_node<T>* gp_node = temp_node->parent->parent; // gp_node is the grandparent node
            rb_node<T>* u_node = root; // u_node is the uncle node (sibling of parent)
            if (temp_node->parent == gp_node->left) { // if the parent is the left child of a grandparent node
                if (gp_node->right != NULL) {
                    u_node = gp_node->right; //uncle node goes to the right of grandparent node if it isn't null
                }

                if (u_node->color == 1) { // possibility 1: if uncle node is red
                    temp_node->parent->color = 0; // make the temp node's parent's color black
                    u_node->color = 0; // make the uncle node black
                    temp_node->parent->parent->color = 1; // make the grangparent node red
                    temp_node = temp_node->parent->parent; // change the temp node to its grandparent
                }
                else if (temp_node == temp_node->parent->right) { // possibility 2: if uncle node is black and temp node is right child
                    temp_node = temp_node->parent; // change the temp node to its parent
                    rotate_left(temp_node); // rotation applied to balance the tree
                }
                else {
                    temp_node->parent->color = 0;  // possibility 3: uncle node is black and temp node is left child
                    temp_node->parent->parent->color = 1; // make the grandparent node red
                    rotate_right(temp_node->parent->parent); // rotation applied to balance the tree
                    temp_node = gp_node; // change the temp node to the grandparent node
                }


            }
            else {
                if (gp_node->left) // if grandparent has a left child
                {
                    u_node = gp_node->left; // uncle node becomes the left child of grandparent node
                }
                if (temp_node == gp_node->right->left) // if temo node is the grandparent's right child's left child
                {
                    temp_node = temp_node->parent; // change temp node to its parent
                    rotate_right(temp_node); // rotation applied to balance the tree
                }
                else if (u_node->color == 1) // if uncle node is red
                {
                    temp_node->parent->color = 0; // make temp node's parent black
                    u_node->color = 0; // change uncle node to black
                    gp_node->color = 1; // change grandparent node to red
                    temp_node = gp_node; // change temp node to grandparent node
                }
                else
                {
                    temp_node->parent->color = 0; // make temp node's parent to black
                    gp_node->color = 1;
                    rotate_left(gp_node);
                    temp_node = gp_node;
                }

            }
        }
        root->color = 0; // change color of root to black
    }

    void rotate_left(rb_node<T>* curr_node) {
        rb_node<T>* temp_node = curr_node->right;
        curr_node->right = temp_node->left;
        if (temp_node->left != NULL) {
            temp_node->left->parent = curr_node;
        }
        temp_node->parent = curr_node->parent;
        if (curr_node->parent == NULL) {
            root = temp_node;
        }
        else if (curr_node == curr_node->parent->left) {
            curr_node->parent->left = temp_node;
        }
        else {
            curr_node->parent->right = temp_node;
        }
        temp_node->left = curr_node;
        curr_node->parent = temp_node;

        // update size field
        temp_node->size = curr_node->size;
        if (curr_node->right != NULL && curr_node->left != NULL) {
            curr_node->size = curr_node->right->size + curr_node->left->size + 1;
        }
        else if (curr_node->right != NULL) {
            curr_node->size = curr_node->right->size + 1;
        }
        else if (curr_node->left != NULL) {
            curr_node->size = curr_node->left->size + 1;
        }
        else {
            curr_node->size = 1;
        }
    }

    void rotate_right(rb_node<T>* curr_node) {
        rb_node<T>* temp_node = curr_node->left;
        curr_node->left = temp_node->right;
        if (temp_node->right != NULL) {
            temp_node->right->parent = curr_node;
        }
        temp_node->parent = curr_node->parent;
        if (curr_node->parent == NULL) {
            root = temp_node;
        }
        else if (curr_node == curr_node->parent->left) {
            curr_node->parent->left = temp_node;
        }
        else {
            curr_node->parent->right = temp_node;
        }
        temp_node->right = curr_node;
        curr_node->parent = temp_node;

        //update size field
        temp_node->size = curr_node->size;

        if (curr_node->right != NULL && curr_node->left != NULL) {
            curr_node->size = curr_node->right->size + curr_node->left->size + 1;
        }
        else if (curr_node->right != NULL) {
            curr_node->size = curr_node->right->size + 1;
        }
        else if (curr_node->left != NULL) {
            curr_node->size = curr_node->left->size + 1;
        }
        else {
            curr_node->size = 1;
        }
    }

    void insert(T key, int line, string path) {
        if (root != NULL)
        {
            rb_node<T>* temp = search(key, root); // search the tree if a node with same key is already inserted
            if (temp == NULL) // if null, it means there was no node with the same key so insert a new node
            {
                //initialize Node y to be parent of curr_node and curr_node = root
                rb_node<T>* prev_node = NULL;
                rb_node<T>* curr_node = this->root;

                //dynamically create new_node Node to be inserted
                rb_node<T>* new_node = new rb_node<T>(key, line, path, 1);

                // go down the tree up to its leaf
                // same as in normal binary search tree
                for (; curr_node != NULL;) {
                    prev_node = curr_node;
                    // if new_node's key is less than key of node curr_node go to curr_node left child
                    //int i = 0;
                    //i = key.compare(curr_node->key);
                    if (key < curr_node->key) {
                        curr_node = curr_node->left;
                    }
                    else {// otherwise go to curr_node right child
                        curr_node = curr_node->right;
                    }

                    prev_node->size++;// increment size of prev_node
                }
                new_node->parent = prev_node;
                //int i = 0;
                //i = key.compare(prev_node->key);
                if (prev_node == NULL) {
                    root = new_node;
                }
                else if (key < prev_node->key) {
                    prev_node->left = new_node;
                }
                else {
                    prev_node->right = new_node;
                }

                // assign NULL to new_node children
                new_node->left = NULL;
                new_node->right = NULL;
                new_node->color = 1;

                // size of z is 1
                new_node->size = 1;

                // bring back tree colouring property
                fix_insert(new_node);
            }
            else // a node with same key found
            {
                temp->duplicate = true;
                temp->no_of_duplicates++;
                rb_node<T>* x = new rb_node<T>(key, line, path); // the node to be inserted
                if (temp->duplicates == NULL) // if a duplicate does not already exists
                {
                    temp->duplicates = x;
                }
                else // if a duplicate exists, insert it in the next
                {
                    rb_node<T>* y = temp->duplicates;
                    while (y->right != NULL)
                    {
                        y = y->right;
                    }
                    y->right = x;
                }
            }
        }
        else
        {
            root = new rb_node<T>(key, line, path, 0);
            root->size = 1;
        }
    }

    void fix_delete(rb_node<T>* x) {
        rb_node<T>* s;
        while (x != root && x->color == 0) {
            if (x == x->parent->left) {
                s = x->parent->right;
                if (s->color == 1) {
                    s->color = 0;
                    x->parent->color = 1;
                    rotate_left(x->parent);
                    s = x->parent->right;
                }

                if (s->left->color == 0 && s->right->color == 0) {
                    s->color = 1;
                    x = x->parent;
                }
                else {
                    if (s->right->color == 0) {
                        s->left->color = 0;
                        s->color = 1;
                        rotate_right(s);
                        s = x->parent->right;
                    }

                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->right->color = 0;
                    rotate_right(x->parent);
                    x = root;
                }
            }
            else {
                s = x->parent->left;
                if (s->color == 1) {
                    s->color = 0;
                    x->parent->color = 1;
                    rotate_right(x->parent);
                    s = x->parent->left;
                }

                if (s->right->color == 0 && s->right->color == 0) {
                    s->color = 1;
                    x = x->parent;
                }
                else {
                    if (s->left->color == 0) {
                        s->right->color = 0;
                        s->color = 1;
                        rotate_left(s);
                        s = x->parent->left;
                    }

                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->left->color = 0;
                    rotate_right(x->parent);
                    x = root;
                }
            }
        }
        x->color = 0;
    }

    void transplanting_rb(rb_node<T>* u, rb_node<T>* v) {
        if (u->parent == NULL) {
            root = v;
        }
        else if (u == u->parent->left) {
            u->parent->left = v;
        }
        else {
            u->parent->right = v;
        }
        if (v != NULL && u != NULL)
            v->parent = u->parent;
    }

    rb_node<T>* minimum(rb_node<T>* node) {
        while (node->left != NULL) {
            node = node->left;
        }
        return node;
    }

    void deleteNodeHelper(T key) {
        rb_node<T>* z = root;
        rb_node<T>* x = NULL;
        rb_node<T>* y = root;
        rb_node<T>* node = root;
        while (node != NULL) {
            if (node->key == key) {
                z = node;
                break;
            }

            if (node->key <= key) {
                node = node->right;
            }
            else {
                node = node->left;
            }
        }

        if (z == NULL) {
            cout << "\tSorry! Seems like this key doesn't exist." << endl;
            return;
        }
        if (z != NULL)
        {
            /*y.color = z->color;
            y.duplicate = z->duplicate;
            y.duplicates = z->duplicates;
            y.file_path = z->file_path;
            y.key = z->key;
            y.left = z->left;
            y.line_no = z->line_no;
            y.no_of_duplicates = z->no_of_duplicates;
            y.parent = z->parent;
            y.right = z->right;
            y.size = z->size;*/
            y = z;
            int y_original_color = y->color;
            if (!(z->right == NULL && z->left == NULL))
            {
                if (z->left == NULL) {
                    x = z->right;
                    if (z != NULL && z->right != NULL)
                        transplanting_rb(z, z->right);
                }
                else if (z->right == NULL) {
                    x = z->left;
                    //                    if (z != NULL && z->left != NULL)
                    transplanting_rb(z, z->left);
                }
                else {
                    y = minimum(z->right);
                    y_original_color = y->color;
                    x = y->right;
                    if (y->parent == z) {
                        x->parent = y;
                    }
                    else {
                        transplanting_rb(y, y->right);
                        y->right = z->right;
                        y->right->parent = y;
                    }

                    transplanting_rb(z, y);
                    y->left = z->left;
                    y->left->parent = y;
                    y->color = z->color;
                }
            }
            else
            {
                if (z->parent->left == z)
                {
                    z->parent->left = NULL;
                }
                else
                    z->parent->right = NULL;
            }
            delete z;
            z = NULL;
            if (x != NULL)
            {
                if (y_original_color == 0) {
                    fix_delete(x);
                }
            }
        }
        else
        {
            cout << "\tSorry! Seems like this key doesn't exist." << endl;
            return;
        }
    }

    rb_node<T>* search(T index, rb_node<T>* root)
    {
        rb_node<T>* current = root;

        while (current != NULL) {
            if (index >= current->key && index <= current->key) {
                //cout << "Found!" << endl;
                return current;
            }

            else if (index < current->key) {
                current = current->left;
            }

            else if (index > current->key) {
                current = current->right;
            }
        }

        //cout << "Not Found, sorry!" << endl;
        return NULL;
    }

    //void print(rb_node<T>* root, string indent, bool last) {
    //    // print the tree structure on the screen
    //    if (root != NULL) {
    //        cout << indent;
    //        if (last) {
    //            cout << "R----";
    //            indent += "     ";
    //        }
    //        else {
    //            cout << "L----";
    //            indent += "|    ";
    //        }

    //        string sColor = root->color == 1 ? "RED" : "BLACK";
    //        cout << root->key << " " << /* root->line_no <<*/ " " << "(" << sColor << ")" << " " /* << root->no_of_duplicates */ << endl;
    //        print(root->left, indent, false);
    //        print(root->right, indent, true);
    //    }
    //}

    int tree_height(rb_node<T>* root)
    {
        if (root == nullptr) {
            return 0;
        }
        return 1 + (height(root->left) > height(root->right) ? height(root->left) : height(root->right));
    }

    int total_nodes(rb_node<T>* root, int& nodes)
    {
        if (root == NULL)
        {
            return nodes;
        }
        nodes += root->no_of_duplicates + 1;
        if (root->left != NULL) {
            total_nodes(root->left, nodes);
        }
        if (root->right != NULL) {
            total_nodes(root->right, nodes);
        }
    }

    int transplanting_rb_index_menu(string* total_fields/*, int& exit_choice*/)
    {
        int choice = 1;
        cout << endl;
        cout << "\t\t|-----------------------------------------------------------------------------|" << endl << endl;
        cout << endl << "\tGreat! And which field are you interested in?" << endl << endl;
        while (total_fields[choice - 1] != "")
        {
            cout << "\t       ---  " << endl;
            cout << "\tPress | " << choice << " | for " << total_fields[choice - 1] << endl;
            cout << "\t       --- " << endl;
            choice++;
        }
        cout << "\t       ---  " << endl;
        cout << "\tPress | " << choice << " | to go to MAIN MENU " << endl;
        cout << "\t       --- " << endl << endl;
        cout << "\tKey I pressed: ";
        //exit_choice = choice; // exit_choice is the key to be entered to exit this menu
        cin >> choice;
        cin.ignore();
        return choice;
    }

    void delete_key(string* total_fields, int index_choice)
    {
        T to_search;
        char choice;
        rb_node<T>* temp;
        cout << "\tThe Index you chose is: " << total_fields[index_choice - 1] << endl << endl;
        cout << "\tPlease enter the " << total_fields[index_choice - 1] << " you want to delete: " << endl;
        cin >> to_search;
        cin.ignore();
        cout << endl;
        cout << "\t                                         ------- " << endl;
        cout << "\tWould you like to use our bonus feature | WHERE | clause?" << endl;
        cout << "\t                                         ------- " << endl;
        cout << "\tA simple Y/N will suffice: " << endl;
        cin >> choice;
        cin.ignore();

        if (choice == 'N' || choice == 'n')
        {
            temp = search(to_search, root); // search the tree for the key user entered
            if (temp != NULL)
            {
                //print(root, "", 0);
                deletefunc(temp);
                if (temp->duplicate == true)
                {
                    rb_node<T>* temp2 = temp->duplicates;
                    //temp = temp->duplicates;
                    while (temp2 != NULL)
                    {
                        deletefunc(temp2);
                        temp2 = temp2->right;
                    }
                }
                deleteNodeHelper(temp->key);
            }
            else
                cout << endl;
                cout << "\tError: Data Not Deleted" << endl;
        }
        else if (choice == 'Y' || choice == 'y')
        {
            rb_node<T>* deletenode = NULL;
            string key;
            int choice = 0;
            choice = print_index_menu(total_fields);
            cout << "Old value : ";
            getline(cin, key, '\n');
            cin.ignore();
            //cin >> key;
            rb_node<T>* temp = search(to_search, root);
            //string str_key = to_string(key);
            delete_where_clause(temp, choice, key);
            if (temp->duplicate == true)
            {
                temp = temp->duplicates;
                while (temp != NULL)
                {
                    delete_where_clause(temp, choice, key);
                    temp = temp->right;
                }
            }
        }
    }

    void delete_where_clause(rb_node<T>* temp, int choice, string oldvalue)
    {
        bool comma;
        string line;
        if (temp != NULL)
        {
            ifstream oldfile(temp->file_path);
            for (int i = 1; i < temp->line_no; i++)
            {
                getline(oldfile, line);
            }
            getline(oldfile, line);
            string str = "";
            istringstream obj(line);
            int count = 0, int_field = 1;
            bool flag = 0;
            while (getline(obj, line, ','))
            {
                comma = comma_checker(line);
                while (comma)
                {
                    string s;
                    getline(obj, s, ',');
                    comma = !comma_checker(s);
                    line = line + ',' + s;
                }
                if (equalsIgnoreCase(line, oldvalue) && int_field == choice)
                {
                    oldfile.close();
                    deletefunc(temp);
                    break;
                }
                int_field++;
            }
        }
        else
            cout << endl;
            cout << "\tError: Data Not Deleted" << endl;
    }

    void deletefunc(rb_node<T>* temp)
    {
        ifstream oldfile(temp->file_path);
        const char* file_to_be_updated = temp->file_path.c_str();
        const char updated_file_path[] = "updated.csv";
        ofstream newfile(updated_file_path);
        int line_no = 1;
        string entry;
        while (getline(oldfile, entry))
        {
            if (line_no != temp->line_no)
            {
                newfile << entry << '\n';
            }
            else
            {
                newfile << "-1" << '\n';
            }
            line_no++;
        }
        newfile.close();
        oldfile.close();
        remove(file_to_be_updated);
        rename(updated_file_path, file_to_be_updated);
        //cout << remove(file_to_be_updated) << endl;
        //cout << rename(updated_file_path, file_to_be_updated) << endl;
        //print(root, "", 0);
    }
    
    int print_index_menu(string* total_fields/*, int& exit_choice*/)
    {
        int choice = 1;
        cout << endl;
        cout << "\t\t|-----------------------------------------------------------------------------|" << endl << endl;
        cout << endl << "\tGreat! And which field are you interested in?" << endl << endl;        while (total_fields[choice - 1] != "")
        {
            cout << "\t       ---  " << endl;
            cout << "\tPress | " << choice << " | for " << total_fields[choice - 1] << endl;
            cout << "\t       --- " << endl;
            choice++;
        }
        cout << "\t       ---  " << endl;
        cout << "\tPress | " << choice << " | to go to MAIN MENU " << endl;
        cout << "\t       --- " << endl << endl;
        cout << "\tKey I pressed: ";        //exit_choice = choice; // exit_choice is the key to be entered to exit this menu
        cin >> choice;
        cin.ignore();
        return choice;
    }

    void point_search(string* total_fields, int index_choice)
    {
        T to_search;
        rb_node<T>* temp;
        char choice;
        cout << endl;
        cout << "\tThe Index you chose is: " << total_fields[index_choice - 1] << endl << endl;
        cout << "\tPlease enter the " << total_fields[index_choice - 1] << " you want to search for: " << endl;
        cin >> to_search;
        cin.ignore();
        cout << endl;
        cout << "\t                                         ------- " << endl;
        cout << "\tWould you like to use our bonus feature | WHERE | clause?" << endl;
        cout << "\t                                         ------- " << endl;
        cout << "\tA simple Y/N will suffice: " << endl;
        cin >> choice;
        cin.ignore();
        if (choice == 'N' || choice == 'n')
        {
            temp = search(to_search, root); // search the tree for the key user entered
            if (temp != NULL)
            {
                print_entry(temp, total_fields, index_choice); // print the information if key found in the tree
                if (temp->duplicate == true) // if there are duplicates of that key
                {
                    temp = temp->duplicates;
                    while (temp != NULL)
                    {
                        print_entry(temp, total_fields, index_choice); // while there are duplicates, keep printing their information as well
                        temp = temp->right; // temp now points to the next duplicate
                    }
                }
            }
        }
        else if (choice == 'Y' || choice == 'y')
        {
            string value;
            int choice = 0;
            choice = print_index_menu(total_fields);
            cout << endl;
            cout << "\tPlease enter the value: ";
            getline(cin, value, '\n');
            //cin >> value;
            rb_node<T>* temp = search(to_search, root);
            point_search_where_clause(temp, value, choice, total_fields, index_choice);
            if (temp->duplicate == true)
            {
                temp = temp->duplicates;
                while (temp != NULL)
                {
                    point_search_where_clause(temp, value, choice, total_fields, index_choice);
                    temp = temp->right;
                }
            }
        }
    }

    void point_search_where_clause(rb_node<T>* temp, string oldvalue, int choice, string* total_fields, int index_choice)
    {
        bool comma;
        string line;
        if (temp != NULL)
        {
            ifstream oldfile(temp->file_path);
            for (int i = 1; i < temp->line_no; i++)
            {
                getline(oldfile, line);
            }
            getline(oldfile, line);
            string str = "";
            istringstream obj(line);
            int count = 0, int_field = 1;
            bool flag = 0;
            while (getline(obj, line, ','))
            {
                comma = comma_checker(line);
                while (comma)
                {
                    string s;
                    getline(obj, s, ',');
                    comma = !comma_checker(s);
                    line = line + ',' + s;
                }
                if (line == oldvalue && int_field == choice)
                {
                    oldfile.close();
                    print_entry(temp, total_fields, index_choice);
                    break;
                }
                int_field++;
            }
        }
        else
            cout << "\tERROR: Data Does Not Exist" << endl;
    }

    void print_entry(rb_node<T>* temp, string* total_fields, int index_choice)
    {
        bool comma;
        string line;
        ifstream myfile(temp->file_path);
        if (myfile.is_open())
        {
            int line_no = 0;
            while (line_no != temp->line_no)
            {
                getline(myfile, line);
                line_no++;
            }
            cout << "Found!" << endl;
            istringstream myline(line);
            int i = 0;
            string info;
            while (getline(myline, info, ',')) // break lines into different fields
            {
                comma = comma_checker(info);
                while (comma)
                {
                    string s;
                    getline(myline, s, ',');
                    comma = !comma_checker(s);
                    info = info + ',' + s;
                }
                cout << total_fields[i] << ": " << info << endl;
                i++;
            }

        }
        myfile.close();
    }

    bool comma_checker(string s)
    {
        for (int i = 0; i < s.length(); i++)
            if (s[i] == '"')
                return 1;
        return 0;
    }

    int findSize(string s)
    {
        for (int i = 0; ; i++)
            if (s[i] == 0)
                return i;
        return 0;
    }

    bool equalsIgnoreCase(string a, string b)
    {
        if (findSize(a) == findSize(b))
        {
            for (int i = 0; a[i] != NULL; i++)
            {
                if (tolower(a[i]) != tolower(b[i]))
                {
                    return false;
                    break;
                }
            }
        }
        else
        {
            return false;
        }
        return true;
    }

    void update(rb_node<T>* temp, int choice, string oldvalue, string newvalue)
    {
        bool comma = 0;
        bool bool_oldvalue = 0;
        string  line;
        ifstream oldfile(temp->file_path);
        const char* file_to_be_updated = temp->file_path.c_str();
        const char updated_file_path[] = "updated.csv";
        ofstream newfile(updated_file_path);
        int line_no = 1;
        while (getline(oldfile, line))
        {
            if (line_no == temp->line_no)
            {
                string str = "";
                istringstream obj(line);
                int count = 0, int_field = 1;
                bool flag = 0;
                while (getline(obj, line, ','))
                {
                    comma = comma_checker(line);
                    while (comma)
                    {
                        string s;
                        getline(obj, s, ',');
                        comma = !comma_checker(s);
                        line = line + ',' + s;
                    }
                    if (equalsIgnoreCase(line, oldvalue) && int_field == choice)
                    {
                        if (choice != 1)
                        {
                            str = str + ',' + newvalue;
                            bool_oldvalue = 1;
                            count++;
                        }
                        else
                        {
                            bool_oldvalue = 1;
                            str = newvalue;
                            count++;
                        }
                        continue;
                    }
                    if (count != 0)
                        str = str + ',' + line;
                    else
                    {
                        str = line;
                        count++;
                    }
                    int_field++;
                }
                newfile << str << '\n';
                if (!bool_oldvalue)
                {
                    cout << "\tERROR: Old Value Does Not Exist" << endl;
                }
            }
            else
            {
                newfile << line << '\n';
            }
            line_no++;
        }
        newfile.close();
        oldfile.close();
        cout << remove(file_to_be_updated) << endl;
        cout << rename(updated_file_path, file_to_be_updated) << endl;
        temp = NULL;
        delete temp;
    }

    void update_key(int choice, string* total_fields, int index_choice)
    {
        T to_search;
        string oldvalue, newvalue;
        rb_node<T>* temp;
        cout << endl;
        cout << "\tThe Index you chose is: " << total_fields[index_choice - 1] << endl << endl;
        cout << "\tPlease enter the " << total_fields[index_choice - 1] << " you want to update: " << endl;
        cin >> to_search;
        cin.ignore();
        temp = search(to_search, root); // search the tree for the key user entered
        if (temp != NULL)
        {
            cout << "\tOld Value: ";
            getline(cin, oldvalue, '\n');
            cin.ignore();
            //cin >> oldvalue;
            cout << "\tNew Value: ";
            getline(cin, newvalue, '\n');
            //cin >> newvalue;
            update(temp, choice, oldvalue, newvalue);
            if (temp->duplicate == true)
            {
                temp = temp->duplicates;
                while (temp != NULL)
                {
                    update(temp, choice, oldvalue, newvalue);
                    temp = temp->right;
                }
            }
        }
    }

    void range_search(string* total_fields, int index_choice)
    {
        T to_search_low, to_search_high;
        rb_node<T>* temp;
        cout << "\tThe Index you chose is: " << total_fields[index_choice - 1] << endl << endl;
        cout << "\tPlease enter the Lower Limit of " << total_fields[index_choice - 1] << " you want to search for: " << endl;
        cin >> to_search_low;
        cin.ignore();
        cout << "\tPlease enter the Higher Limit of " << total_fields[index_choice - 1] << " you want to search for: " << endl;
        cin >> to_search_high;
        cin.ignore();
        range_search_func(root, to_search_low, to_search_high, total_fields, index_choice);
    }

    void range_search_func(rb_node<T>* root, T key1, T key2, string* total_fields, int index_choice)
    {
        if (root == NULL)
            return;

        //int i = key1.compare(root->key);
        if (key1 < root->key)
        {
            range_search_func(root->left, key1, key2, total_fields, index_choice);
        }
        if (key1 <= root->key && key2 >= root->key)
        {
            rb_node<T>* temp = root;
            print_entry(temp, total_fields, index_choice); // print the information if key found in the tree
            if (temp->duplicate == true) // if there are duplicates of that key
            {
                temp = temp->duplicates;
                while (temp != NULL)
                {
                    print_entry(temp, total_fields, index_choice); // while there are duplicates, keep printing their information as well
                    temp = temp->right; // temp now points to the next duplicate
                }
            }
        }
        range_search_func(root->right, key1, key2, total_fields, index_choice);
    }


};

class str_rb_tree
{
public:
    rb_node<string>* root;

    str_rb_tree()
    {
        root = NULL;
    }

    void fix_insert(rb_node<string>* temp_node) {
        if (temp_node->parent == NULL) {// if no parent, it means it is the root
            return;
        }
        for (; temp_node->parent != NULL && temp_node->parent->color == 1;)
        {    // while parent of temp_node is red, this loop will work
             // or else it means that the parent is black and the tree is also balanced
            rb_node<string>* gp_node = temp_node->parent->parent; // gp_node is the grandparent node
            rb_node<string>* u_node = root; // u_node is the uncle node (sibling of parent)
            if (temp_node->parent == gp_node->left) { // if the parent is the left child of a grandparent node
                if (gp_node->right != NULL) {
                    u_node = gp_node->right; //uncle node goes to the right of grandparent node if it isn't null
                }

                if (u_node->color == 1) { // possibility 1: if uncle node is red
                    temp_node->parent->color = 0; // make the temp node's parent's color black
                    u_node->color = 0; // make the uncle node black
                    temp_node->parent->parent->color = 1; // make the grangparent node red
                    temp_node = temp_node->parent->parent; // change the temp node to its grandparent
                }
                else if (temp_node == temp_node->parent->right) { // possibility 2: if uncle node is black and temp node is right child
                    temp_node = temp_node->parent; // change the temp node to its parent
                    rotate_left(temp_node); // rotation applied to balance the tree
                }
                else {
                    temp_node->parent->color = 0;  // possibility 3: uncle node is black and temp node is left child
                    temp_node->parent->parent->color = 1; // make the grandparent node red
                    rotate_right(temp_node->parent->parent); // rotation applied to balance the tree
                    temp_node = gp_node; // change the temp node to the grandparent node
                }


            }
            else {
                if (gp_node->left) // if grandparent has a left child
                {
                    u_node = gp_node->left; // uncle node becomes the left child of grandparent node
                }
                if (temp_node == gp_node->right->left) // if temo node is the grandparent's right child's left child
                {
                    temp_node = temp_node->parent; // change temp node to its parent
                    rotate_right(temp_node); // rotation applied to balance the tree
                }
                else if (u_node->color == 1) // if uncle node is red
                {
                    temp_node->parent->color = 0; // make temp node's parent black
                    u_node->color = 0; // change uncle node to black
                    gp_node->color = 1; // change grandparent node to red
                    temp_node = gp_node; // change temp node to grandparent node
                }
                else
                {
                    temp_node->parent->color = 0; // make temp node's parent to black
                    gp_node->color = 1;
                    rotate_left(gp_node);
                    temp_node = gp_node;
                }

            }
        }
        root->color = 0; // change color of root to black
    }

    void rotate_left(rb_node<string>* curr_node) {
        rb_node<string>* temp_node = curr_node->right;
        curr_node->right = temp_node->left;
        if (temp_node->left != NULL) {
            temp_node->left->parent = curr_node;
        }
        temp_node->parent = curr_node->parent;
        if (curr_node->parent == NULL) {
            root = temp_node;
        }
        else if (curr_node == curr_node->parent->left) {
            curr_node->parent->left = temp_node;
        }
        else {
            curr_node->parent->right = temp_node;
        }
        temp_node->left = curr_node;
        curr_node->parent = temp_node;

        // update size field
        temp_node->size = curr_node->size;
        if (curr_node->right != NULL && curr_node->left != NULL) {
            curr_node->size = curr_node->right->size + curr_node->left->size + 1;
        }
        else if (curr_node->right != NULL) {
            curr_node->size = curr_node->right->size + 1;
        }
        else if (curr_node->left != NULL) {
            curr_node->size = curr_node->left->size + 1;
        }
        else {
            curr_node->size = 1;
        }
    }

    void rotate_right(rb_node<string>* curr_node) {
        rb_node<string>* temp_node = curr_node->left;
        curr_node->left = temp_node->right;
        if (temp_node->right != NULL) {
            temp_node->right->parent = curr_node;
        }
        temp_node->parent = curr_node->parent;
        if (curr_node->parent == NULL) {
            root = temp_node;
        }
        else if (curr_node == curr_node->parent->left) {
            curr_node->parent->left = temp_node;
        }
        else {
            curr_node->parent->right = temp_node;
        }
        temp_node->right = curr_node;
        curr_node->parent = temp_node;

        //update size field
        temp_node->size = curr_node->size;

        if (curr_node->right != NULL && curr_node->left != NULL) {
            curr_node->size = curr_node->right->size + curr_node->left->size + 1;
        }
        else if (curr_node->right != NULL) {
            curr_node->size = curr_node->right->size + 1;
        }
        else if (curr_node->left != NULL) {
            curr_node->size = curr_node->left->size + 1;
        }
        else {
            curr_node->size = 1;
        }
    }

    void insert(string key, int line, string path) {
        if (root != NULL)
        {
            rb_node<string>* temp = search(key, root); // search the tree if a node with same key is already inserted
            if (temp == NULL) // if null, it means there was no node with the same key so insert a new node
            {
                //initialize Node y to be parent of curr_node and curr_node = root
                rb_node<string>* prev_node = NULL;
                rb_node<string>* curr_node = this->root;

                //dynamically create new_node Node to be inserted
                rb_node<string>* new_node = new rb_node<string>(key, line, path, 1);

                // go down the tree up to its leaf
                // same as in normal binary search tree
                for (; curr_node != NULL;) {
                    prev_node = curr_node;
                    // if new_node's key is less than key of node curr_node go to curr_node left child
                    int i = 0;
                    i = key.compare(curr_node->key);
                    if (i < 0) {
                        curr_node = curr_node->left;
                    }
                    else {// otherwise go to curr_node right child
                        curr_node = curr_node->right;
                    }

                    prev_node->size++;// increment size of prev_node
                }
                new_node->parent = prev_node;
                int i = 0;
                i = key.compare(prev_node->key);
                if (prev_node == NULL) {
                    root = new_node;
                }
                else if (i < 0) {
                    prev_node->left = new_node;
                }
                else {
                    prev_node->right = new_node;
                }

                // assign NULL to new_node children
                new_node->left = NULL;
                new_node->right = NULL;
                new_node->color = 1;

                // size of z is 1
                new_node->size = 1;

                // bring back tree colouring property
                fix_insert(new_node);
            }
            else // a node with same key found
            {
                temp->duplicate = true;
                temp->no_of_duplicates++;
                rb_node<string>* x = new rb_node<string>(key, line, path); // the node to be inserted
                if (temp->duplicates == NULL) // if a duplicate does not already exists
                {
                    temp->duplicates = x;
                }
                else // if a duplicate exists, insert it in the next
                {
                    rb_node<string>* y = temp->duplicates;
                    while (y->right != NULL)
                    {
                        y = y->right;
                    }
                    y->right = x;
                }
            }
        }
        else
        {
            root = new rb_node<string>(key, line, path, 0);
            root->size = 1;
        }
    }

    void fix_delete(rb_node<string>* x) {
        rb_node<string>* s;
        while (x != root && x->color == 0) {
            if (x == x->parent->left) {
                s = x->parent->right;
                if (s->color == 1) {
                    s->color = 0;
                    x->parent->color = 1;
                    rotate_left(x->parent);
                    s = x->parent->right;
                }

                if (s->left->color == 0 && s->right->color == 0) {
                    s->color = 1;
                    x = x->parent;
                }
                else {
                    if (s->right->color == 0) {
                        s->left->color = 0;
                        s->color = 1;
                        rotate_right(s);
                        s = x->parent->right;
                    }

                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->right->color = 0;
                    rotate_right(x->parent);
                    x = root;
                }
            }
            else {
                s = x->parent->left;
                if (s->color == 1) {
                    s->color = 0;
                    x->parent->color = 1;
                    rotate_right(x->parent);
                    s = x->parent->left;
                }

                if (s->right->color == 0 && s->right->color == 0) {
                    s->color = 1;
                    x = x->parent;
                }
                else {
                    if (s->left->color == 0) {
                        s->right->color = 0;
                        s->color = 1;
                        rotate_left(s);
                        s = x->parent->left;
                    }

                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->left->color = 0;
                    rotate_right(x->parent);
                    x = root;
                }
            }
        }
        x->color = 0;
    }

    void transplanting_rb(rb_node<string>* u, rb_node<string>* v) {
        if (u->parent == NULL) {
            root = v;
        }
        else if (u == u->parent->left) {
            u->parent->left = v;
        }
        else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    rb_node<string>* minimum(rb_node<string>* node) {
        while (node->left != NULL) {
            node = node->left;
        }
        return node;
    }

    void deleteNodeHelper(string key) {
        rb_node<string>* z = root;
        rb_node<string>* x = NULL;
        rb_node<string>* y = root;
        rb_node<string>* node = root;
        z = search(key, root);

        if (z == NULL) {
            return;
        }
        if (z != NULL)
        {
            y = z;
            int y_original_color = y->color;
            if (!(z->right == NULL && z->left == NULL))
            {
                if (z->left == NULL) {
                    x = z->right;
                    transplanting_rb(z, z->right);
                }
                else if (z->right == NULL) {
                    x = z->left;
                    transplanting_rb(z, z->left);
                }
                else {
                    y = minimum(z->right);
                    y_original_color = y->color;
                    x = y->right;
                    if (y->parent == z) {
                        x->parent = y;
                    }
                    else {
                        transplanting_rb(y, y->right);
                        y->right = z->right;
                        y->right->parent = y;
                    }

                    transplanting_rb(z, y);
                    y->left = z->left;
                    y->left->parent = y;
                    y->color = z->color;
                }
            }
            else
            {
                if (z->parent->left == z)
                {
                    z->parent->left = NULL;
                }
                else
                    z->parent->right = NULL;
            }
            delete z;
            z = NULL;
            if (x != NULL)
            {
                if (y_original_color == 0) {
                    fix_delete(x);
                }
            }
        }
        else
        {
            cout << "\tERROR: Data Does Not Exist" << endl;
            return;
        }
    }

    rb_node<string>* search(string index, rb_node<string>* root)
    {
        rb_node<string>* current = root;

        while (current != NULL) {
            int i = 0;
            i = index.compare(current->key);
            if (i == 0) {
                //cout << "Found!" << endl;
                return current;
            }

            else if (i < 0) {
                current = current->left;
            }

            else if (i > 0) {
                current = current->right;
            }
        }

        //cout << "Not Found, sorry!" << endl;
        return NULL;
    }

    //void print(rb_node<string>* root, string indent, bool last) {
    //    // print the tree structure on the screen
    //    if (root != NULL) {
    //        cout << indent;
    //        if (last) {
    //            cout << "R----";
    //            indent += "     ";
    //        }
    //        else {
    //            cout << "L----";
    //            indent += "|    ";
    //        }

    //        string sColor = root->color == 1 ? "RED" : "BLACK";
    //        cout << root->key << " " << /* root->line_no <<*/ " " << "(" << sColor << ")" << " " /* << root->no_of_duplicates */ << endl;
    //        print(root->left, indent, false);
    //        print(root->right, indent, true);
    //    }
    //}

    int tree_height(rb_node<string>* root)
    {
        if (root == nullptr) {
            return 0;
        }
        return 1 + (tree_height(root->left) > tree_height(root->right) ? tree_height(root->left) : tree_height(root->right));
    }

    int total_nodes(rb_node<string>* root, int& nodes)
    {
        if (root == NULL)
        {
            return nodes;
        }
        nodes += root->no_of_duplicates + 1;
        if (root->left != NULL) {
            total_nodes(root->left, nodes);
        }
        if (root->right != NULL) {
            total_nodes(root->right, nodes);
        }
    }

    void delete_key(string* total_fields, int index_choice)
    {
        string to_search;
        char choice;
        rb_node<string>* temp;
        cout << endl;
        cout << "\tThe Index you chose is: " << total_fields[index_choice - 1] << endl << endl;
        cout << "\tPlease enter the " << total_fields[index_choice - 1] << " you want to delete: " << endl;
        getline(cin, to_search, '\n');
        cin.ignore();
        cout << endl;
        cout << "\t                                         ------- " << endl;
        cout << "\tWould you like to use our bonus feature | WHERE | clause?" << endl;
        cout << "\t                                         ------- " << endl;
        cout << "\tA simple Y/N will suffice: " << endl;
        cin >> choice;
        cin.ignore();

        if (choice == 'N' || choice == 'n')
        {
            temp = search(to_search, root); // search the tree for the key user entered
            if (temp != NULL)
            {
                //print(root, "", 0);
                deletefunc(temp);
                if (temp->duplicate == true)
                {
                    rb_node<string>* temp2 = temp->duplicates;
                    //temp = temp->duplicates;
                    while (temp2 != NULL)
                    {
                        deletefunc(temp2);
                        temp2 = temp2->right;
                    }
                }
                deleteNodeHelper(temp->key);
            }
            else
                cout << "\tERROR: Data Not Deleted" << endl;
        }
        else if (choice == 'Y' || choice == 'y')
        {
            rb_node<string>* deletenode = NULL;
            string key;
            int choice = 0;
            choice = print_index_menu(total_fields);
            cout << "Old value : ";
            getline(cin, key, '\n');
            cin.ignore();
            rb_node<string>* temp = search(to_search, root);
            //string str_key = to_string(key);
            delete_where_clause(temp, choice, key);
            if (temp->duplicate == true)
            {
                temp = temp->duplicates;
                while (temp != NULL)
                {
                    delete_where_clause(temp, choice, key);
                    temp = temp->right;
                }
            }
        }
    }

    int print_index_menu(string* total_fields/*, int& exit_choice*/)
    {
        int choice = 1;
        cout << endl;
        cout << "\t\t|-----------------------------------------------------------------------------|" << endl << endl;
        cout << endl << "\tGreat! And which field are you interested in?" << endl << endl;
        while (total_fields[choice - 1] != "")
        {
            cout << "\t       ---  " << endl;
            cout << "\tPress | " << choice << " | for " << total_fields[choice - 1] << endl;
            cout << "\t       --- " << endl;
            choice++;
        }
        cout << "\t       ---  " << endl;
        cout << "\tPress | " << choice << " | to go to MAIN MENU " << endl;
        cout << "\t       --- " << endl << endl;
        cout << "\tKey I pressed: ";
        //exit_choice = choice; // exit_choice is the key to be entered to exit this menu
        cin >> choice;
        cin.ignore();
        return choice;
    }

    void delete_where_clause(rb_node<string>* temp, int choice, string oldvalue)
    {
        bool comma;
        string line;
        if (temp != NULL)
        {
            ifstream oldfile(temp->file_path);
            for (int i = 1; i < temp->line_no; i++)
            {
                getline(oldfile, line);
            }
            getline(oldfile, line);
            string str = "";
            istringstream obj(line);
            int count = 0, int_field = 1;
            bool flag = 0;
            while (getline(obj, line, ','))
            {
                comma = comma_checker(line);
                while (comma)
                {
                    string s;
                    getline(obj, s, ',');
                    comma = !comma_checker(s);
                    line = line + ',' + s;
                }
                if (equalsIgnoreCase(line, oldvalue) && int_field == choice)
                {
                    oldfile.close();
                    deletefunc(temp);
                    break;
                }
                int_field++;
            }
        }
        else
            cout << "\tERROR: Data Not Deleted" << endl;
    }

    void deletefunc(rb_node<string>* temp)
    {
        ifstream oldfile(temp->file_path);
        const char* file_to_be_updated = temp->file_path.c_str();
        const char updated_file_path[] = "updated.csv";
        ofstream newfile(updated_file_path);
        int line_no = 1;
        string entry;
        while (getline(oldfile, entry))
        {
            if (line_no != temp->line_no)
            {
                newfile << entry << '\n';
            }
            else
            {
                newfile << "-1" << '\n';
            }
            line_no++;
        }
        newfile.close();
        oldfile.close();
        remove(file_to_be_updated);
        rename(updated_file_path, file_to_be_updated);
        //cout << remove(file_to_be_updated) << endl;
        //cout << rename(updated_file_path, file_to_be_updated) << endl;
        //print(root, "", 0);
    }

    void point_search(string* total_fields, int index_choice)
    {
        string to_search;
        rb_node<string>* temp;
        char choice;
        cout << "\tThe Index you chose is: " << total_fields[index_choice - 1] << endl << endl;
        cout << "\tPlease enter the " << total_fields[index_choice - 1] << " you want to search for: " << endl;
        getline(cin, to_search, '\n');
        cin.ignore();
        cout << endl;
        cout << "\t                                         ------- " << endl;
        cout << "\tWould you like to use our bonus feature | WHERE | clause?" << endl;
        cout << "\t                                         ------- " << endl;
        cout << "\tA simple Y/N will suffice: " << endl;
        cin >> choice;
        cin.ignore();
        if (choice == 'N' || choice == 'n')
        {
            temp = search(to_search, root); // search the tree for the key user entered
            if (temp != NULL)
            {
                print_entry(temp, total_fields, index_choice); // print the information if key found in the tree
                if (temp->duplicate == true) // if there are duplicates of that key
                {
                    temp = temp->duplicates;
                    while (temp != NULL)
                    {
                        print_entry(temp, total_fields, index_choice); // while there are duplicates, keep printing their information as well
                        temp = temp->right; // temp now points to the next duplicate
                    }
                }
            }
        }
        else if (choice == 'Y' || choice == 'y')
        {
            string value;
            int choice = 0;
            choice = print_index_menu(total_fields);
            cout << "\tPlease enter the value: ";
            getline(cin, value, '\n');
            rb_node<string>* temp = search(to_search, root);
            point_search_where_clause(temp, value, choice, total_fields, index_choice);
            if (temp->duplicate == true)
            {
                temp = temp->duplicates;
                while (temp != NULL)
                {
                    point_search_where_clause(temp, value, choice, total_fields, index_choice);
                    temp = temp->right;
                }
            }
        }
    }

    void point_search_where_clause(rb_node<string>* temp, string oldvalue, int choice, string* total_fields, int index_choice)
    {
        bool comma;
        string line;
        if (temp != NULL)
        {
            ifstream oldfile(temp->file_path);
            for (int i = 1; i < temp->line_no; i++)
            {
                getline(oldfile, line);
            }
            getline(oldfile, line);
            string str = "";
            istringstream obj(line);
            int count = 0, int_field = 1;
            bool flag = 0;
            while (getline(obj, line, ','))
            {
                comma = comma_checker(line);
                while (comma)
                {
                    string s;
                    getline(obj, s, ',');
                    comma = !comma_checker(s);
                    line = line + ',' + s;
                }
                if (line == oldvalue && int_field == choice)
                {
                    oldfile.close();
                    print_entry(temp, total_fields, index_choice);
                    break;
                }
                int_field++;
            }
        }
        else
            cout << "ERROR: Data Does Not Exist" << endl;
    }

    void print_entry(rb_node<string>* temp, string* total_fields, int index_choice)
    {
        bool comma;
        string line;
        ifstream myfile(temp->file_path);
        if (myfile.is_open())
        {
            int line_no = 0;
            while (line_no != temp->line_no)
            {
                getline(myfile, line);
                line_no++;
            }
            cout << "Found!" << endl;
            istringstream myline(line);
            int i = 0;
            string info;
            while (getline(myline, info, ',')) // break lines into different fields
            {
                comma = comma_checker(info);
                while (comma)
                {
                    string s;
                    getline(myline, s, ',');
                    comma = !comma_checker(s);
                    info = info + ',' + s;
                }
                cout << total_fields[i] << ": " << info << endl;
                i++;
            }

        }
        myfile.close();
    }

    void update(rb_node<string>* temp, int choice, string oldvalue, string newvalue)
    {
        bool comma = 0;
        bool bool_oldvalue = 0;
        string  line;
        ifstream oldfile(temp->file_path);
        const char* file_to_be_updated = temp->file_path.c_str();
        const char updated_file_path[] = "updated.csv";
        ofstream newfile(updated_file_path);
        int line_no = 1;
        while (getline(oldfile, line))
        {
            if (line_no == temp->line_no)
            {
                string str = "";
                istringstream obj(line);
                int count = 0, int_field = 1;
                bool flag = 0;
                while (getline(obj, line, ','))
                {
                    comma = comma_checker(line);
                    while (comma)
                    {
                        string s;
                        getline(obj, s, ',');
                        comma = !comma_checker(s);
                        line = line + ',' + s;
                    }
                    if (equalsIgnoreCase(line, oldvalue) && int_field == choice)
                    {
                        if (choice != 1)
                        {
                            str = str + ',' + newvalue;
                            bool_oldvalue = 1;
                            count++;
                        }
                        else
                        {
                            bool_oldvalue = 1;
                            str = newvalue;
                            count++;
                        }
                        continue;
                    }
                    if (count != 0)
                        str = str + ',' + line;
                    else
                    {
                        str = line;
                        count++;
                    }
                    int_field++;
                }
                newfile << str << '\n';
                if (!bool_oldvalue)
                {
                    cout << "\tERROR: Old Value Does Not Exist" << endl;
                }
            }
            else
            {
                newfile << line << '\n';
            }
            line_no++;
        }
        newfile.close();
        oldfile.close();
        cout << remove(file_to_be_updated) << endl;
        cout << rename(updated_file_path, file_to_be_updated) << endl;
        temp = NULL;
        delete temp;
    }

    void update_key(int choice, string* total_fields, int index_choice)
    {
        string to_search;
        string oldvalue, newvalue;
        rb_node<string>* temp;
        cout << "\tThe Index you chose is: " << total_fields[index_choice - 1] << endl << endl;
        cout << "\tPlease enter the " << total_fields[index_choice - 1] << " you want to update: " << endl;
        getline(cin, to_search, '\n');
        cin.ignore();
        temp = search(to_search, root); // search the tree for the key user entered
        if (temp != NULL)
        {
            cout << "\tOld Value: ";
            getline(cin, oldvalue, '\n');
            cin.ignore();
            cout << "\tNew Value: ";
            //cin.ignore();
            getline(cin, newvalue, '\n');
            cin.ignore();
            update(temp, choice, oldvalue, newvalue);
            if (temp->duplicate == true)
            {
                temp = temp->duplicates;
                while (temp != NULL)
                {
                    update(temp, choice, oldvalue, newvalue);
                    temp = temp->right;
                }
            }
        }
    }

    int findSize(string s)
    {
        for (int i = 0; ; i++)
            if (s[i] == 0)
                return i;
        return 0;
    }

    bool equalsIgnoreCase(string a, string b)
    {
        if (findSize(a) == findSize(b))
        {
            for (int i = 0; a[i] != NULL; i++)
            {
                if (tolower(a[i]) != tolower(b[i]))
                {
                    return false;
                    break;
                }
            }
        }
        else
        {
            return false;
        }
        return true;
    }

    bool comma_checker(string s)
    {
        for (int i = 0; i < s.length(); i++)
            if (s[i] == '"')
                return 1;
        return 0;
    }

    void range_search(string* total_fields, int index_choice)
    {
        //print(root, "", 0);
        string to_search_low, to_search_high;
        rb_node<string>* temp;
        cout << "\tThe Index you chose is: " << total_fields[index_choice - 1] << endl << endl;
        cout << "\tPlease enter the Lower Limit of " << total_fields[index_choice - 1] << " you want to search for: " << endl;
        getline(cin, to_search_low, '\n');
        cin.ignore();
        cout << "\tPlease enter the Higher Limit of " << total_fields[index_choice - 1] << " you want to search for: " << endl;
        //cin.ignore();
        getline(cin, to_search_high, '\n');
        cin.ignore();
        range_search_func(root, to_search_low, to_search_high, total_fields, index_choice);
    }

    void range_search_func(rb_node<string>* root, string key1, string key2, string* total_fields, int index_choice)
    {
        if (root == NULL)
            return;

        int i = key1.compare(root->key);
        int j = key2.compare(root->key);
        if (i < 0)
        {
            range_search_func(root->left, key1, key2, total_fields, index_choice);
        }
        i = key1.compare(root->key);
        j = key2.compare(root->key);
        if (i <= 0 && j >= 0)
        {
            rb_node<string>* temp = root;
            print_entry(temp, total_fields, index_choice); // print the information if key found in the tree
            if (temp->duplicate == true) // if there are duplicates of that key
            {
                temp = temp->duplicates;
                while (temp != NULL)
                {
                    print_entry(temp, total_fields, index_choice); // while there are duplicates, keep printing their information as well
                    temp = temp->right; // temp now points to the next duplicate
                }
            }
        }
        range_search_func(root->right, key1, key2, total_fields, index_choice);
    }

};