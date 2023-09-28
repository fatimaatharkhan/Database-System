// C++ program for AVL Tree  
#include<iostream> 
#include<sstream>
#include<fstream>
#include<string>
#include <filesystem>
#include<cstdio>

using namespace std;
namespace fs = std::filesystem;

// An AVL tree node  
template <class T>
class AVLNode
{
public:
    T key;
    AVLNode* left;
    AVLNode* right;
    string path;
    int lineno;
    int depth;
    AVLNode<T>* duplicate;
    AVLNode()
    {
        duplicate = NULL;
    }
    void insert_duplicate(T k, string s, int no)
    {
        //count++;
        key = k;
        path = s;
        lineno = no;
    }

};

template <class T>

class AVLTREE
{

public:
    string* total_fields{}; // stores the names of the fields in case they are needed later
    int int_total_fields;

    //get max of two integers 
    int max(int a, int b) {
        return (a > b) ? a : b;
    }

    //function to get height of the tree 
    int depth(AVLNode<T>* n)
    {
        if (n == NULL)
            return 0;
        return n->depth;
    }

    // allocate a new node with key passed
    AVLNode<T>* newNode(T key, string s, int no)
    {
        AVLNode<T>* node = new AVLNode<T>();
        node->key = key;
        node->path = s;
        node->lineno = no;
        node->left = NULL;
        node->right = NULL;
        node->depth = 1; // new node added as leaf 
        return(node);
    }

    // right rotate the sub tree rooted with y
    AVLNode<T>* rightRotate(AVLNode<T>* y)
    {
        AVLNode<T>* x = y->left;
        AVLNode<T>* T2 = x->right;

        // Perform rotation  
        x->right = y;
        y->left = T2;

        // Update heights  
        y->depth = max(depth(y->left),
            depth(y->right)) + 1;
        x->depth = max(depth(x->left),
            depth(x->right)) + 1;

        // Return new root  
        return x;
    }

    // left rotate the sub tree rooted with x 
    AVLNode<T>* leftRotate(AVLNode<T>* x)
    {
        AVLNode<T>* y = x->right;
        AVLNode<T>* T2 = y->left;

        // Perform rotation  
        y->left = x;
        x->right = T2;
        // Update heights  
        x->depth = max(depth(x->left),
            depth(x->right)) + 1;
        y->depth = max(depth(y->left),
            depth(y->right)) + 1;

        // Return new root  
        return y;
    }

    // Get Balance factor of node N  
    int getBalance(AVLNode<T>* N)
    {
        if (N == NULL)
            return 0;
        return depth(N->left) -
            depth(N->right);
    }

    //insertion operation for node in AVL tree 
    AVLNode<T>* insert(AVLNode<T>*& node, T key, string s, int no)
    {
        //normal BST rotation
        if (node == NULL)
            return(newNode(key, s, no));

        if (key < node->key)
            node->left = insert(node->left, key, s, no);
        else if (key > node->key)
            node->right = insert(node->right, key, s, no);
        else // Equal keys not allowed  
        {

            AVLNode<T>* temp = node;
            while (temp->duplicate != NULL)
            {
                temp = temp->duplicate;
            }
            temp->duplicate = new AVLNode<T>;
            temp->duplicate->insert_duplicate(key, s, no);
            return node;
        }
        //update height of ancestor node
        node->depth = 1 + max(depth(node->left), depth(node->right));

        int balance = getBalance(node);        //get balance factor

        // rotate if unbalanced 

        // Left Left Case  
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        // Right Right Case  
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);
        // Left Right Case  
        if (balance > 1 && key > node->left->key)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case  
        if (balance < -1 && key < node->right->key)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // find the node with minimum value 
    AVLNode<T>* minValueNode(AVLNode<T>* node)
    {
        AVLNode<T>* current = node;

        // find the leftmost leaf */
        while (current->left != NULL)
            current = current->left;

        return current;
    }

    // delete a node from AVL tree with the given key  
    AVLNode<T>* deleteNode(AVLNode<T>* root, T key)
    {
        if (root == NULL)
            return root;

        //perform BST delete 
        if (key < root->key)
            root->left = deleteNode(root->left, key);

        else if (key > root->key)
            root->right = deleteNode(root->right, key);

        else
        {
            // node with only one child or no child  
            if ((root->left == NULL) ||
                (root->right == NULL))
            {
                AVLNode<T>* temp = root->left ?
                    root->left :
                    root->right;

                if (temp == NULL)
                {
                    temp = root;
                    root = NULL;
                }
                else // One child case  
                    *root = *temp;
                free(temp);
            }
            else
            {
                AVLNode<T>* temp = minValueNode(root->right);
                root->key = temp->key;
                // Delete the inorder successor  
                root->right = deleteNode(root->right,
                    temp->key);
            }
        }

        if (root == NULL)
            return root;

        // update depth  
        root->depth = 1 + max(depth(root->left),
            depth(root->right));

        // get balance factor 
        int balance = getBalance(root);

        //rotate the tree if unbalanced

        // Left Left Case  
        if (balance > 1 &&
            getBalance(root->left) >= 0)
            return rightRotate(root);

        // Left Right Case  
        if (balance > 1 && getBalance(root->left) < 0)
        {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        // Right Right Case  
        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);

        // Right Left Case  
        if (balance < -1 && getBalance(root->right) > 0)
        {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        return root;
    }

    // prints inOrder traversal of the AVL tree
    void inOrder(AVLNode<T>* root)
    {
        if (root != NULL)
        {
            inOrder(root->left);
            cout << root->key << " ";
            cout << root->lineno << " ";
            cout << root->path << endl;
            AVLNode<T>* temp = root->duplicate;
            while (temp != NULL)
            {
                cout << temp->key << " ";
                cout << temp->lineno << " ";
                cout << temp->path << endl;
                temp = temp->duplicate;
            }
            temp = NULL;
            delete temp;
            inOrder(root->right);
        }
    }

    bool comma_checker(string s)
    {
        for (int i = 0; i < s.length(); i++)
            if (s[i] == '"')
                return 1;
        return 0;
    }

    void createAVLTree(AVLNode<T>*& root, int tree_type)
    {
        int field = 0, line_no = 0, index_choice = 1, int_key, exit_choice = 0; // index_choice is the choice of index user enters 
        // and exit_choice will be the number the user enters to exit that menu
        // field is the number of field the iterator is at currently
        double double_key;
        string first_line, last_line;
        string line, key, path = "datafiles";
        bool menu_written = false;
        bool comma = 0;
        // if menu is printed, this will turn true so it isnt printed again in the loop
        for (const auto& entry : fs::directory_iterator(path)) // iterates all files in a directory
        {
            line_no = 0;
            ifstream myfile(entry.path().u8string()); // path of file given to read
            if (myfile.is_open())
            {
                while (getline(myfile, line) && index_choice != exit_choice) // while file is not empty and the user did not choose to exit
                {
                    string path_file = entry.path().u8string();
                    istringstream myline(line);
                    field = 0;
                    while (getline(myline, key, ','))
                    {
                        comma = comma_checker(key);
                        while (comma)
                        {
                            string s;
                            getline(myline, s, ',');
                            comma = !comma_checker(s);
                            key = key + ',' + s;
                        }
                        if (line_no == 0 && menu_written == false) // if the first line of the file is read and the menu has not yet been printed
                        {
                            index_choice = print_index_menu(line, exit_choice);
                            menu_written = true;
                            total_fields = new string[exit_choice]; // an array of total number of fields is printed
                            store_fields(total_fields, line); // function called to save the fields in the array total_fields
                            if (exit_choice == index_choice) // if user enters the key to exit
                                break;
                        }
                        if (field == index_choice - 1 && line_no != 0) // if the field user entered for index is found
                        {
                            if (tree_type == 1)
                            {
                                int_key = stoi(key);
                                root = insert(root, int_key, entry.path().u8string(), line_no);// insert to the tree
                            }
                            else if (tree_type == 2)
                            {
                                double_key = stod(key);
                                root = insert(root, double_key, entry.path().u8string(), line_no);// insert to the tree
                            }
                        }
                        field++;
                    }
                    line_no++;
                }
            }
            myfile.close();
        }
    }

    bool checkint(string s)
    {
        for (int i = 0; i < s.length(); i++)
            if ((s[i] < 47 || s[i]>58) && (s[i] != 46) && s[i] != 10)
                return 0;
        return 1;
    }

    void retrievedata(string path, int lineno)
    {
        string line, key;
        bool comma = 0;
        ifstream myfile(path);
        for (int i = 0; i <= lineno; i++)
            getline(myfile, line);
        istringstream file(line);
        for (int i = 0; i < int_total_fields; i++)
        {
            getline(file, key, ',');
            comma = comma_checker(key);
            while (comma)
            {
                string s;
                getline(file, s, ',');
                comma = !comma_checker(s);
                key = key + ',' + s;
            }
            cout << total_fields[i] << " : " << key << endl;
        }

    }

    void point_search_where_clause(AVLNode<T>* temp, string oldvalue, int choice, bool& found)
    {
        bool comma;
        string line;
        bool flag = 1;
        if (temp != NULL)
        {
            ifstream oldfile(temp->path);
            for (int i = 0; i < temp->lineno; i++)
            {
                getline(oldfile, line);
            }
            getline(oldfile, line);
            string str = "";
            istringstream obj(line);
            int count = 0, int_field = 1;
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
                    retrievedata(temp->path, temp->lineno);
                    found = 0;
                    flag = 0;
                    break;
                }
                int_field++;
            }
            if (flag && found)
                cout << "ERROR: Data Does Not Exist" << endl;
        }
        else
            cout << "ERROR: Data Does Not Exist" << endl;
    }

    void point_search(AVLNode<T>* root)
    {
        T key_to_be_searched;
        cout << "\tEnter the key to be searched: ";
        cin >> key_to_be_searched;
        cin.ignore();
        string s;
        cout << endl;
        cout << "\t                                         ------- " << endl;
        cout << "\tWould you like to use our bonus feature | WHERE | clause?" << endl;
        cout << "\t                                         ------- " << endl;
        cout << "\tA simple Y/N will suffice: " << endl;
        cin >> s;
        cin.ignore();

        if (s == "N" || s == "n")
        {
            AVLNode<T>* temp = searchintree(root, key_to_be_searched);
            if (temp != NULL)
            {
                retrievedata(temp->path, temp->lineno);
                temp = temp->duplicate;
                while (temp != NULL)
                {
                    retrievedata(temp->path, temp->lineno);
                    temp = temp->duplicate;
                }
            }
        }
        else if (s == "Y" || s == "y")
        {
            string value;
            int choice = 0;
            choice = print_field_menu(root);
            cout << "\tPlease enter the value: ";
            getline(cin, value, '\n');
            cin.ignore();
            bool found = 1;
            AVLNode<T>* temp = searchintree(root, key_to_be_searched);
            while (temp != NULL)
            {
                point_search_where_clause(temp, value, choice, found);
                temp = temp->duplicate;
            }
        }
    }

    AVLNode<T>* searchintree(AVLNode<T>* root, T key_to_be_searched)
    {
        if (root == NULL)
        {
            cout << "EROR: Data Does Not Exist" << endl;
            return NULL;
        }
        if (root->key == key_to_be_searched)
            return root;
        else if (key_to_be_searched < root->key)
            searchintree(root->left, key_to_be_searched);
        else if (key_to_be_searched > root->key)
            searchintree(root->right, key_to_be_searched);

    }

    void PreOrder(AVLNode<T>* node)
    {
        if (node == NULL)
            return;
        cout << node->key << " ";
        PreOrder(node->left);
        PreOrder(node->right);
    }

    int print_field_menu(AVLNode<T>* root)
    {
        string key;
        string line;
        ifstream file(root->path);
        getline(file, line);
        file.close();
        istringstream myline(line);
        int choice = 1;
        cout << endl;
        cout << "\t\t|-----------------------------------------------------------------------------|" << endl << endl;
        cout << endl << "\tGreat! And which field are you interested in?" << endl << endl;
        while (getline(myline, key, ','))
        {
            cout << "\t       ---  " << endl;
            cout << "\tPress | " << choice << " | for " << key << endl;
            cout << "\t       --- " << endl;
            choice++;
        }
        cout << "\tKey I pressed: ";
        cin >> choice;
        cin.ignore();
        return choice;
    }

    void deletefunc(AVLNode<T>* temp)
    {
        string  line;
        ifstream oldfile(temp->path);
        const char* file_to_be_updated = temp->path.c_str();
        const char updated_file_path[] = "datafiles\\updated.csv";
        ofstream newfile(updated_file_path);
        for (int i = 0; i < temp->lineno; i++)
        {
            getline(oldfile, line);
            newfile << line << '\n';
        }
        getline(oldfile, line);
        newfile << "-1" << '\n';
        while (!oldfile.eof())
        {
            getline(oldfile, line);
            newfile << line << '\n';
        }
        newfile.close();
        oldfile.close();

        cout << remove(file_to_be_updated) << endl;
        cout << rename(updated_file_path, file_to_be_updated) << endl;
    }

    void delete_where_clause(AVLNode<T>* temp, int choice, string oldvalue)
    {
        bool comma;
        string line;
        bool flag = 1;
        if (temp != NULL)
        {
            ifstream oldfile(temp->path);
            for (int i = 0; i < temp->lineno; i++)
            {
                getline(oldfile, line);
            }
            getline(oldfile, line);
            string str = "";
            istringstream obj(line);
            int count = 0, int_field = 1;
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
                    flag = 0;
                    break;
                }
                int_field++;
            }
            if (flag)
                cout << "ERROR: Data Does Not Exist" << endl;
        }
        else
            cout << "ERROR: Data Not Deleted" << endl;
    }

    void delete_key(AVLNode<T>*& root)
    {
        T key;
        string s;
        cout << "\tEnter the key to delete: ";
        cin >> key;
        cin.ignore();
        cout << endl;
        cout << "\t                                         ------- " << endl;
        cout << "\tWould you like to use our bonus feature | WHERE | clause?" << endl;
        cout << "\t                                         ------- " << endl;
        cout << "\tA simple Y/N will suffice: " << endl;
        cin >> s;
        cin.ignore();
        if (s == "N" || s == "n")
        {
            AVLNode<T>* temp = searchintree(root, key);
            if (temp != NULL)
            {
                deletefunc(temp);
                temp = temp->duplicate;
                while (temp != NULL)
                {
                    deletefunc(temp);
                    temp = temp->duplicate;
                }
                root = deleteNode(root, key);
            }
            else
                cout << "ERROR: Data Not Deleted" << endl;
        }
        else if (s == "Y" || s == "y")
        {
            string oldvalue;
            int choice = 0;
            choice = print_field_menu(root);
            cout << "\tOld Value: ";
            getline(cin, oldvalue, '\n');
            cin.ignore();
            AVLNode<T>* temp = searchintree(root, key);
            while (temp != NULL)
            {
                delete_where_clause(temp, choice, oldvalue);
                temp = temp->duplicate;
            }
        }
    }

    void update(AVLNode<T>* temp, int choice, string oldvalue, string newvalue)
    {
        bool comma = 0;
        bool bool_oldvalue = 0;
        string  line;
        ifstream oldfile(temp->path);
        const char* file_to_be_updated = temp->path.c_str();
        const char updated_file_path[] = "datafiles\\updated.csv";
        ofstream newfile(updated_file_path);
        for (int i = 0; i < temp->lineno; i++)
        {
            getline(oldfile, line);
            newfile << line << '\n';
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
        if (bool_oldvalue)
            newfile << str << '\n';
        else
        {
            newfile << str << '\n';
            cout << "\tERROR: Old Value Not Found " << endl;
        }
        while (!oldfile.eof())
        {
            getline(oldfile, line);
            newfile << line << '\n';
        }
        newfile.close();
        oldfile.close();
        cout << remove(file_to_be_updated) << endl;
        cout << rename(updated_file_path, file_to_be_updated) << endl;
        temp = NULL;
        delete temp;
    }

    void update_file(AVLNode<T>* root)
    {
        T key;
        int choice = 0;
        string  oldvalue, newvalue;
        cout << "\tEnter the key to update: ";
        cin >> key;
        cin.ignore();
        choice = print_field_menu(root);
        cout << "\tOld value: ";
        getline(cin, oldvalue, '\n');
        cin.ignore();
        cout << "\tNew value: ";
        getline(cin, newvalue, '\n');
        cin.ignore();
        AVLNode<T>* temp = searchintree(root, key);
        update(temp, choice, oldvalue, newvalue);
        temp = temp->duplicate;
        while (temp != NULL)
        {
            update(temp, choice, oldvalue, newvalue);
            temp = temp->duplicate;
        }
    }

    int print_index_menu(string line, int& exit_choice)
    {
        string key;
        istringstream myline(line);
        int choice = 1;
        cout << endl;
        cout << "\t\t|-----------------------------------------------------------------------------|" << endl << endl;
        cout << endl << "\tGreat! And which field are you interested in?" << endl << endl;
        while (getline(myline, key, ','))
        {
            cout << "\t       ---  " << endl;
            cout << "\tPress | " << choice << " | for " << key << endl;
            cout << "\t       --- " << endl;
            choice++;
        }
        cout << "\t       ---  " << endl;
        cout << "\tPress | " << choice << " | to go to MAIN MENU " << endl;
        cout << "\t       --- " << endl << endl;
        cout << "\tKey I pressed: ";
        exit_choice = choice; // exi_choice is the key to be entered to exit this menu
        cin >> choice;
        cin.ignore();
        return choice;
    }

    void store_fields(string* fields, string line)
    {
        //string key;
        istringstream myline(line);
        int i = 0;
        while (getline(myline, fields[i], ',')) // store the fields in fields[i] in a loop
        {
            i++;
        }
        int_total_fields = i;
    }

    void removeemptylines()
    {
        string path = "datafiles";
        string line;
        const char updated_file_path[] = "datafiles\\updated.csv";
        for (const auto& entry : fs::directory_iterator(path)) // iterates all files in a directory
        {
            string s = entry.path().u8string();
            const char* file_to_be_updated = s.c_str();
            ifstream oldfile(file_to_be_updated); // path of file given to read
            ofstream newfile(updated_file_path);
            if (oldfile.is_open())
            {
                while (getline(oldfile, line))
                {
                    if (line != "-1")
                        newfile << line << '\n';
                }
            }
            oldfile.close();
            newfile.close();
            cout << remove(file_to_be_updated) << endl;
            cout << rename(updated_file_path, file_to_be_updated) << endl;
            file_to_be_updated = NULL;
            delete file_to_be_updated;
        }
    }

    void range_search(AVLNode<T>* root)
    {
        int index_choice = 0;
        index_choice = print_field_menu(root);
        T to_search_low, to_search_high;
        AVLNode<T>* temp;
        cout << "\tThe Index you chose is: " << total_fields[index_choice - 1] << endl << endl;
        cout << "\tPlease enter the Lower Limit of " << total_fields[index_choice - 1] << " you want to search for: " << endl;
        cin >> to_search_low;
        cin.ignore();
        cout << "\tPlease enter the Higher Limit of " << total_fields[index_choice - 1] << " you want to search for: " << endl;
        cin >> to_search_high;
        cin.ignore();
        range_search_func(root, to_search_low, to_search_high, index_choice);
    }

    void range_search_func(AVLNode<T>* root, T key1, T key2, int index_choice)
    {
        if (root == NULL)
            return;

        if (key1 < root->key)
        {
            range_search_func(root->left, key1, key2, index_choice);
        }
        if (key1 <= root->key && key2 >= root->key)
        {
            AVLNode<T>* temp = root;
            retrievedata(temp->path, temp->lineno);// print the information if key found in the tree
            if (temp->duplicate != NULL) // if there are duplicates of that key
            {
                temp = temp->duplicate;
                while (temp != NULL)
                {
                    retrievedata(temp->path, temp->lineno); // while there are duplicates, keep printing their information as well
                    temp = temp->duplicate; // temp now points to the next duplicate
                }
            }
        }
        range_search_func(root->right, key1, key2, index_choice);
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

    int print_tasks_menu()
    {
        int task_choice = 0;
        cout << endl;
        cout << "\t\t|-----------------------------------------------------------------------------|" << endl << endl;
        cout << endl << "\tInteresting choice, what task would you like to perform?" << endl << endl;
        cout << "\t       ---  " << endl;
        cout << "\tPress | 1 | for Point Search" << endl;
        cout << "\t       ---  " << endl;
        cout << "\t       ---  " << endl;
        cout << "\tPress | 2 | for Range Search" << endl;
        cout << "\t       ---  " << endl;
        cout << "\t       ---" << endl;
        cout << "\tPress | 3 | for Updating an entry" << endl;
        cout << "\t       ---  " << endl;
        cout << "\t       ---" << endl;
        cout << "\tPress | 4 | for Deleting an entry" << endl;
        cout << "\t       ---  " << endl;
        cout << "\t       ---  " << endl;
        cout << "\tPress | 5 | to go to MAIN MENU " << endl;
        cout << "\t       --- " << endl << endl;
        cout << "\tKey I pressed: ";
        cin >> task_choice;
        cin.ignore();
        return task_choice;
    }

    void avl_tasks(AVLNode<T>* root)
    {

        int task_choice = 0;
        while (task_choice != 6)
        {
            task_choice = print_tasks_menu();
            switch (task_choice)
            {
            case 1:
                point_search(root); // calls function to search a specific key
                break;
            case 2:
                range_search(root);
                break;
            case 3:
                update_file(root);
                break;
            case 4:
                delete_key(root);
                break;
            case 5:
                break;
            }
        }
    }

};


class AVLTREE_STRING
{

public:
    string* total_fields{}; // stores the names of the fields in case they are needed later
    int int_total_fields;

    //static int count;
    //void display()
    //{
    //    cout << "Count: " << count << endl;
    //}
    //get max of two integers 
    int max(int a, int b) {
        return (a > b) ? a : b;
    }

    //function to get height of the tree 
    int depth(AVLNode<string>* n)
    {
        if (n == NULL)
            return 0;
        return n->depth;
    }
    // allocate a new node with key passed
    AVLNode<string>* newNode(string key, string s, int no)
    {
        //count++;
        AVLNode<string>* node = new AVLNode<string>();
        node->key = key;
        node->path = s;
        node->lineno = no;
        node->left = NULL;
        node->right = NULL;
        node->depth = 1; // new node added as leaf 
        return(node);
    }
    // right rotate the sub tree rooted with y
    AVLNode<string>* rightRotate(AVLNode<string>* y)
    {
        AVLNode<string>* x = y->left;
        AVLNode<string>* T2 = x->right;

        // Perform rotation  
        x->right = y;
        y->left = T2;

        // Update heights  
        y->depth = max(depth(y->left),
            depth(y->right)) + 1;
        x->depth = max(depth(x->left),
            depth(x->right)) + 1;

        // Return new root  
        return x;
    }

    // left rotate the sub tree rooted with x 
    AVLNode<string>* leftRotate(AVLNode<string>* x)
    {
        AVLNode<string>* y = x->right;
        AVLNode<string>* T2 = y->left;

        // Perform rotation  
        y->left = x;
        x->right = T2;
        // Update heights  
        x->depth = max(depth(x->left),
            depth(x->right)) + 1;
        y->depth = max(depth(y->left),
            depth(y->right)) + 1;

        // Return new root  
        return y;
    }

    // Get Balance factor of node N  
    int getBalance(AVLNode<string>* N)
    {
        if (N == NULL)
            return 0;
        return depth(N->left) -
            depth(N->right);
    }
    //insertion operation for node in AVL tree 
    AVLNode<string>* insert(AVLNode<string>*& node, string key, string s, int no)
    {
        //normal BST rotation
        if (node == NULL)
            return(newNode(key, s, no));
        int i = key.compare(node->key);
        if (i < 0)
            node->left = insert(node->left, key, s, no);
        else if (i > 0)
            node->right = insert(node->right, key, s, no);
        else // Equal keys not allowed  
        {

            AVLNode<string>* temp = node;
            while (temp->duplicate != NULL)
            {
                temp = temp->duplicate;
            }
            temp->duplicate = new AVLNode<string>;
            temp->duplicate->insert_duplicate(key, s, no);
            return node;
        }
        //update height of ancestor node
        node->depth = 1 + max(depth(node->left), depth(node->right));

        int balance = getBalance(node);        //get balance factor

        // rotate if unbalanced 

        // Left Left Case  
        if (node->left != NULL)
        {
            int j = key.compare(node->left->key);
            if (balance > 1 && j < 0)
                return rightRotate(node);
        }
        if (node->right != NULL)
        {
            int k = key.compare(node->right->key);
            // Right Right Case  
            if (balance < -1 && k > 0)
                return leftRotate(node);
        }
        // Left Right Case  
        if (node->left != NULL)
        {
            int j = key.compare(node->left->key);
            if (balance > 1 && j > 0)
            {
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }
        }
        if (node->right != NULL)
        {
            int k = key.compare(node->right->key);
            // Right Left Case  
            if (balance < -1 && k < 0)
            {
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }
        }
        return node;
    }

    // find the node with minimum value 
    AVLNode<string>* minValueNode(AVLNode<string>* node)
    {
        AVLNode<string>* current = node;

        // find the leftmost leaf */
        while (current->left != NULL)
            current = current->left;

        return current;
    }
    // delete a node from AVL tree with the given key  
    AVLNode<string>* deleteNode(AVLNode<string>* root, string key)
    {
        if (root == NULL)
            return root;
        int i = key.compare(root->key);
        //perform BST delete 
        if (i < 0)
            root->left = deleteNode(root->left, key);

        else if (i > 0)
            root->right = deleteNode(root->right, key);

        else
        {
            // node with only one child or no child  
            if ((root->left == NULL) ||
                (root->right == NULL))
            {
                AVLNode<string>* temp = root->left ?
                    root->left :
                    root->right;

                if (temp == NULL)
                {
                    temp = root;
                    root = NULL;
                }
                else // One child case  
                    *root = *temp;
                free(temp);
            }
            else
            {
                AVLNode<string>* temp = minValueNode(root->right);
                root->key = temp->key;
                // Delete the inorder successor  
                root->right = deleteNode(root->right,
                    temp->key);
            }
        }

        if (root == NULL)
            return root;

        // update depth  
        root->depth = 1 + max(depth(root->left),
            depth(root->right));

        // get balance factor 
        int balance = getBalance(root);

        //rotate the tree if unbalanced

        // Left Left Case  
        if (balance > 1 &&
            getBalance(root->left) >= 0)
            return rightRotate(root);

        // Left Right Case  
        if (balance > 1 && getBalance(root->left) < 0)
        {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        // Right Right Case  
        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);

        // Right Left Case  
        if (balance < -1 && getBalance(root->right) > 0)
        {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        return root;
    }
    // prints inOrder traversal of the AVL tree
    void inOrder(AVLNode<string>* root)
    {
        if (root != NULL)
        {
            inOrder(root->left);
            cout << root->key << " ";
            cout << root->lineno << " ";
            cout << root->path << endl;
            AVLNode<string>* temp = root->duplicate;
            while (temp != NULL)
            {
                cout << temp->key << " ";
                cout << temp->lineno << " ";
                cout << temp->path << endl;
                temp = temp->duplicate;
            }
            temp = NULL;
            delete temp;
            inOrder(root->right);
        }
    }
    bool comma_checker(string s)
    {
        for (int i = 0; i < s.length(); i++)
            if (s[i] == '"')
                return 1;
        return 0;
    }
    void createAVLTree(AVLNode<string>*& root)
    {
        int field = 0, line_no = 0, index_choice = 1, exit_choice = 0; // index_choice is the choice of index user enters 
        // and exit_choice will be the number the user enters to exit that menu
        // field is the number of field the iterator is at currently
        string first_line, last_line;
        string line, key, path = "datafiles";
        bool menu_written = false;
        bool comma = 0;
        // if menu is printed, this will turn true so it isnt printed again in the loop
        for (const auto& entry : fs::directory_iterator(path)) // iterates all files in a directory
        {
            line_no = 0;
            ifstream myfile(entry.path().u8string()); // path of file given to read
            if (myfile.is_open())
            {
                while (getline(myfile, line) && index_choice != exit_choice) // while file is not empty and the user did not choose to exit
                {
                    string path_file = entry.path().u8string();
                    istringstream myline(line);
                    field = 0;
                    while (getline(myline, key, ','))
                    {
                        comma = comma_checker(key);
                        while (comma)
                        {
                            string s;
                            getline(myline, s, ',');
                            comma = !comma_checker(s);
                            key = key + ',' + s;
                        }
                        if (line_no == 0 && menu_written == false) // if the first line of the file is read and the menu has not yet been printed
                        {
                            index_choice = print_index_menu(line, exit_choice);
                            menu_written = true;
                            total_fields = new string[exit_choice]; // an array of total number of fields is printed
                            store_fields(total_fields, line); // function called to save the fields in the array total_fields
                            if (exit_choice == index_choice) // if user enters the key to exit
                                break;
                        }
                        if (field == index_choice - 1 && line_no != 0) // if the field user entered for index is found
                        {

                            root = insert(root, key, entry.path().u8string(), line_no);// insert to the tree
                        }
                        field++;
                    }
                    line_no++;
                }
            }
            myfile.close();
        }
        //avl_tasks();
    }

    int print_tasks_menu()
    {
        int task_choice = 0;
        cout << endl;
        cout << "\t\t|-----------------------------------------------------------------------------|" << endl << endl;
        cout << endl << "\tInteresting choice, what task would you like to perform?" << endl << endl;
        cout << "\t       ---  " << endl;
        cout << "\tPress | 1 | for Point Search" << endl;
        cout << "\t       ---  " << endl;
        cout << "\t       ---  " << endl;
        cout << "\tPress | 2 | for Range Search" << endl;
        cout << "\t       ---  " << endl;
        cout << "\t       ---" << endl;
        cout << "\tPress | 3 | for Updating an entry" << endl;
        cout << "\t       ---  " << endl;
        cout << "\t       ---" << endl;
        cout << "\tPress | 4 | for Deleting an entry" << endl;
        cout << "\t       ---  " << endl;
        cout << "\t       ---  " << endl;
        cout << "\tPress | 5 | to go to MAIN MENU " << endl;
        cout << "\t       --- " << endl << endl;
        cout << "\tKey I pressed: ";
        cin >> task_choice;
        cin.ignore();
        return task_choice;
    }

    void avl_tasks(AVLNode<string>* root)
    {

        int task_choice = 0;
        while (task_choice != 5)
        {
            task_choice = print_tasks_menu();
            switch (task_choice)
            {
            case 1:
                point_search(root); // calls function to search a specific key
                break;
            case 2:
                range_search(root);
                break;
            case 3:
                update_file(root);
                break;
            case 4:
                delete_key(root);
                break;
            case 5:

                break;
      
            }
        }
        return;
    }

    bool checkint(string s)
    {
        for (int i = 0; i < s.length(); i++)
            if ((s[i] < 47 || s[i]>58) && (s[i] != 46) && s[i] != 10)
                return 0;
        return 1;
    }
    void retrievedata(string path, int lineno)
    {
        string line, key;
        bool comma = 0;
        ifstream myfile(path);
        for (int i = 0; i <= lineno; i++)
            getline(myfile, line);
        istringstream file(line);
        for (int i = 0; i < int_total_fields; i++)
        {
            getline(file, key, ',');
            comma = comma_checker(key);
            while (comma)
            {
                string s;
                getline(file, s, ',');
                comma = !comma_checker(s);
                key = key + ',' + s;
            }
            cout << total_fields[i] << " : " << key << endl;
        }

    }
    void point_search_where_clause(AVLNode<string>* temp, string oldvalue, int choice, bool& found)
    {
        bool comma;
        string line;
        bool flag = 1;
        if (temp != NULL)
        {
            ifstream oldfile(temp->path);
            for (int i = 0; i < temp->lineno; i++)
            {
                getline(oldfile, line);
            }
            getline(oldfile, line);
            string str = "";
            istringstream obj(line);
            int count = 0, int_field = 1;
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
                    retrievedata(temp->path, temp->lineno);
                    flag = 0;
                    found = 0;
                    break;
                }
                int_field++;
            }
            if (flag && found)
                cout << "ERROR: Data Not Found" << endl;
        }
        else
            cout << "ERROR: Data Not Found" << endl;
    }
    AVLNode<string>* search(string index, AVLNode<string>* root)
    {
        AVLNode<string>* current = root;
        while (current != NULL)
        {
            int i = 0;
            i = index.compare(current->key);
            if (i == 0)
            {
                //cout << "Found!" << endl;
                return current;
            }
            else if (i < 0)
            {
                current = current->left;
            }
            else if (i > 0)
            {
                current = current->right;
            }
        }

        //cout << "Not Found, sorry!" << endl;
        return NULL;
    }
    void point_search(AVLNode<string>* root)
    {
        string key_to_be_searched;
        cout << "\tEnter the key to be searched: ";
        getline(cin, key_to_be_searched, '\n');
        cin.ignore();
        string s;
        cout << endl;
        cout << "\t                                         ------- " << endl;
        cout << "\tWould you like to use our bonus feature | WHERE | clause?" << endl;
        cout << "\t                                         ------- " << endl;
        cout << "\tA simple Y/N will suffice: " << endl;
        cin >> s;
        cin.ignore();
        if (s == "N" || s == "n")
        {
            AVLNode<string>* temp = search(key_to_be_searched, root);
            if (temp != NULL)
            {
                retrievedata(temp->path, temp->lineno);
                temp = temp->duplicate;
                while (temp != NULL)
                {
                    retrievedata(temp->path, temp->lineno);
                    temp = temp->duplicate;
                }
            }
        }
        else if (s == "Y" || s == "y")
        {
            string value;
            int choice = 0;
            choice = print_field_menu(root);
            cout << "Please enter the value : ";
            getline(cin, value, '\n');
            cin.ignore();
            bool found = 1;
            AVLNode<string>* temp = search(key_to_be_searched, root);
            while (temp != NULL)
            {
                point_search_where_clause(temp, value, choice, found);
                temp = temp->duplicate;
            }
        }
    }

    void PreOrder(AVLNode<string>* node)
    {
        if (node == NULL)
            return;
        cout << node->key << " ";
        PreOrder(node->left);
        PreOrder(node->right);
    }
    int print_field_menu(AVLNode<string>* root)
    {
        string key;
        string line;
        ifstream file(root->path);
        getline(file, line);
        file.close();
        istringstream myline(line);
        int choice = 1;
        cout << endl << "CHOOSE THE FIELD " << endl << endl;
        while (getline(myline, key, ','))
        {
            cout << "Press " << choice << " to choose the " << key << " as field" << endl;
            choice++;
        }
        cout << "Press the desired option : " << endl;
        cin >> choice;
        cin.ignore();
        return choice;
    }
    void deletefunc(AVLNode<string>* temp)
    {
        string  line;
        ifstream oldfile(temp->path);
        const char* file_to_be_updated = temp->path.c_str();
        const char updated_file_path[] = "datafiles\\updated.csv";
        ofstream newfile(updated_file_path);
        for (int i = 0; i < temp->lineno; i++)
        {
            getline(oldfile, line);
            newfile << line << '\n';
        }
        getline(oldfile, line);
        newfile << "-1" << '\n';
        while (!oldfile.eof())
        {
            getline(oldfile, line);
            newfile << line << '\n';
        }
        newfile.close();
        oldfile.close();

        cout << remove(file_to_be_updated) << endl;
        cout << rename(updated_file_path, file_to_be_updated) << endl;
    }
    void delete_where_clause(AVLNode<string>* temp, int choice, string oldvalue)
    {
        bool comma;
        string line;
        bool flag = 1;
        if (temp != NULL)
        {
            ifstream oldfile(temp->path);
            for (int i = 0; i < temp->lineno; i++)
            {
                getline(oldfile, line);
            }
            getline(oldfile, line);
            string str = "";
            istringstream obj(line);
            int count = 0, int_field = 1;
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
                    flag = 0;
                    break;
                }
                int_field++;
            }
            if (flag)
                cout << "\tERROR: Data not found" << endl;
        }
        else
            cout << "\tERROR: Data Not Deleted" << endl;
    }
    void delete_key(AVLNode<string>*& root)
    {
        string key;
        string s;
        cout << "\tEnter the key to delete : ";
        getline(cin, key, '\n');
        cin.ignore();
        cout << endl;
        cout << "\t                                         ------- " << endl;
        cout << "\tWould you like to use our bonus feature | WHERE | clause?" << endl;
        cout << "\t                                         ------- " << endl;
        cout << "\tA simple Y/N will suffice: " << endl;
        cin >> s;
        cin.ignore();
        if (s == "N" || s == "n")
        {
            AVLNode<string>* temp = search(key, root);
            if (temp != NULL)
            {
                deletefunc(temp);
                temp = temp->duplicate;
                while (temp != NULL)
                {
                    deletefunc(temp);
                    temp = temp->duplicate;
                }
                root = deleteNode(root, key);
            }
            else
                cout << "Data Not Deleted" << endl;
        }
        else if (s == "Y" || s == "y")
        {
            string oldvalue;
            int choice = 0;
            choice = print_field_menu(root);
            cout << "Old value : ";
            getline(cin, oldvalue, '\n');
            cin.ignore();
            AVLNode<string>* temp = search(key, root);
            while (temp != NULL)
            {
                delete_where_clause(temp, choice, oldvalue);
                temp = temp->duplicate;
            }
        }
    }
    void update(AVLNode<string>* temp, int choice, string oldvalue, string newvalue)
    {
        bool comma = 0;
        bool bool_oldvalue = 0;
        string  line;
        ifstream oldfile(temp->path);
        const char* file_to_be_updated = temp->path.c_str();
        const char updated_file_path[] = "datafiles\\updated.csv";
        ofstream newfile(updated_file_path);
        for (int i = 0; i < temp->lineno; i++)
        {
            getline(oldfile, line);
            newfile << line << '\n';
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
        if (bool_oldvalue)
            newfile << str << '\n';
        else
        {
            newfile << str << '\n';
        }
        while (!oldfile.eof())
        {
            getline(oldfile, line);
            newfile << line << '\n';
        }
        newfile.close();
        oldfile.close();
        cout << remove(file_to_be_updated) << endl;
        cout << rename(updated_file_path, file_to_be_updated) << endl;
        temp = NULL;
        delete temp;
    }
    void update_file(AVLNode<string>* root)
    {
        string key;
        int choice = 0;
        string  oldvalue, newvalue;
        cout << "\tEnter the key to update: ";
        getline(cin, key, '\n');
        cin.ignore();
        choice = print_field_menu(root);
        cout << "\tOld value: ";
        getline(cin, oldvalue, '\n');
        cin.ignore();
        cout << "\tNew value: ";
        getline(cin, newvalue, '\n');
        cin.ignore();
        AVLNode<string>* temp = search(key, root);
        update(temp, choice, oldvalue, newvalue);
        temp = temp->duplicate;
        while (temp != NULL)
        {
            update(temp, choice, oldvalue, newvalue);
            temp = temp->duplicate;
        }
    }
    int print_index_menu(string line, int& exit_choice)
    {
        string key;
        istringstream myline(line);
        int choice = 1;
        cout << endl;
        cout << "\t\t|-----------------------------------------------------------------------------|" << endl << endl;
        cout << endl << "\tGreat! And which field are you interested in?" << endl << endl;
        while (getline(myline, key, ','))
        {
            cout << "\t       ---  " << endl;
            cout << "\tPress | " << choice << " | for " << key << endl;
            cout << "\t       --- " << endl;
            choice++;
        }
        cout << "Press " << choice << " to go to MENU 1 (choosing tree) " << endl;
        exit_choice = choice; // exi_choice is the key to be entered to exit this menu
        cin >> choice;
        cin.ignore();
        return choice;
    }
    void store_fields(string* fields, string line)
    {
        //string key;
        istringstream myline(line);
        int i = 0;
        while (getline(myline, fields[i], ',')) // store the fields in fields[i] in a loop
        {
            i++;
        }
        int_total_fields = i;
    }
    void removeemptylines()
    {
        string path = "datafiles";
        string line;
        const char updated_file_path[] = "datafiles\\updated.csv";
        for (const auto& entry : fs::directory_iterator(path)) // iterates all files in a directory
        {
            string s = entry.path().u8string();
            const char* file_to_be_updated = s.c_str();
            ifstream oldfile(file_to_be_updated); // path of file given to read
            ofstream newfile(updated_file_path);
            if (oldfile.is_open())
            {
                while (getline(oldfile, line))
                {
                    if (line != "-1")
                        newfile << line << '\n';
                }
            }
            oldfile.close();
            newfile.close();
            cout << remove(file_to_be_updated) << endl;
            cout << rename(updated_file_path, file_to_be_updated) << endl;
            file_to_be_updated = NULL;
            delete file_to_be_updated;
        }
    }
    void range_search(AVLNode<string>* root)
    {
        int index_choice = 0;
        index_choice = print_field_menu(root);
        string to_search_low, to_search_high;
        AVLNode<string>* temp;
        cout << "\tThe Index you chose is: " << total_fields[index_choice - 1] << endl << endl;
        cout << "\tPlease enter the Lower Limit of " << total_fields[index_choice - 1] << " you want to search for: " << endl;
        getline(cin, to_search_low, '\n');
        cin.ignore();
        cout << "\tPlease enter the Higher Limit of " << total_fields[index_choice - 1] << " you want to search for: " << endl;
        //cin.ignore();
        getline(cin, to_search_high, '\n');
        cin.ignore();
        range_search_func(root, to_search_low, to_search_high, index_choice);
    }
    void range_search_func(AVLNode<string>* root, string key1, string key2, int index_choice)
    {
        if (root == NULL)
            return;

        int i = key1.compare(root->key);
        int j = key2.compare(root->key);
        if (i < 0)
        {
            range_search_func(root->left, key1, key2, index_choice);
        }
        i = key1.compare(root->key);
        j = key2.compare(root->key);
        if (i <= 0 && j >= 0)
        {
            AVLNode<string>* temp = root;
            retrievedata(temp->path, temp->lineno); // print the information if key found in the tree
            if (temp->duplicate != NULL) // if there are duplicates of that key
            {
                temp = temp->duplicate;
                while (temp != NULL)
                {
                    retrievedata(temp->path, temp->lineno); // while there are duplicates, keep printing their information as well
                    temp = temp->duplicate; // temp now points to the next duplicate
                }
            }
        }
        range_search_func(root->right, key1, key2, index_choice);
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
};
//int main()
//{
//    AVLNode<string>* root = NULL;
//    AVLTREE_STRING tree;
//    tree.createAVLTree(root);
//    tree.update_file(root);
//    tree.point_search(root);
//    //tree.delete_key(root);
//    //tree.point_search(root);
//    //tree.range_search(root);
//  //  tree.inOrder(root);
//    cout << endl;
//    //root->display();
//    //tree.display();
//    tree.removeemptylines();
//    // tree. PreOrder(root);
//}