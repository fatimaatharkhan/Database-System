#include <iostream>
#include "string"
#include <fstream>
#include <sstream>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;
#include "node.h"
#include "rb_tree.h"
#include "b_tree.h"
#include "avl_tree.h"

int tree_type();
int print_index_menu(string* total_fields, int& exit_choice);
void create_rb_tree(int tree_type);
int print_tasks_menu();
void rb_tasks(int tree_type, string* total_fields, int index_choice, int exit_choice);
void avl_tasks(int tree_type);
void rb_point_search(int tree_type, string* total_fields, int index_choice);
int store_fields(string*& fields, string line);
//void rb_print_entry(rb_node<int>* temp, string* total_fields, int index_choice);
void rb_update_key(int tree_type, string* total_fields, int index_choice, int exit_choice);
void rb_delete_key(int tree_type, string* total_fields, int index_choice);
bool comma_checker(string s);
void removeemptylines();
void rb_range_search(int tree_type, string* total_fields, int index_choice);
//void write_tree_to_file(int tree_type);
//int read_tree_from_file(rb_tree<int>*);
//void create_tree_from_array(rb_node<int>* r, rb_node<int>* arr, int n, int pos);
//void create_array_from_tree(rb_node<int>* r, rb_node<int>* arr, int pos);

rb_tree<int>* int_tree = new rb_tree<int>;
rb_tree<double>* double_tree = new rb_tree<double>;
str_rb_tree* string_tree = new str_rb_tree;

AVLNode<int>* int_avl_root = new AVLNode<int>;
AVLNode<double>* double_avl_root = new AVLNode<double>;
AVLNode<string>* string_avl_root = new AVLNode<string>;
AVLTREE<int>* int_avl = new AVLTREE<int>;
AVLTREE<double>* double_avl = new AVLTREE<double>;
AVLTREE_STRING* string_avl = new AVLTREE_STRING;


int main() {

	int tree_choice = 0, type_choice;
	while (tree_choice != 4) // while user doesnt enter 4 to exit
	{
		cout << "\n\n\t\t-------------------------------------------------------------------------------" << endl;
		cout << "\t\t|-----------------------DATA BASE MANAGEMENT SYSTEM---------------------------|" << endl;
		cout << "\t\t-------------------------------------------------------------------------------" << endl << endl << endl;

		cout << "\tGreetings! What tree would you like to choose for your database?" << endl << endl;
		cout << "\t       ---  " << endl;
		cout << "\tPress | 1 | for AVL TREE" << endl;
		cout << "\t       ---  " << endl << endl;
		cout << "\t       ---  " << endl;
		cout << "\tPress | 2 | for B TREE " << endl;
		cout << "\t       ---  " << endl << endl;
		cout << "\t       ---" << endl;
		cout << "\tPress | 3 | for RED BLACK TREE" << endl;
		cout << "\t       ---  " << endl << endl << endl;
		cout << "\t\t\t\t\t       ---" << endl;
		cout << "\t\t\t\t\tPress | 4 | to EXIT" << endl;
		cout << "\t\t\t\t\t       --- " << endl << endl;
		cout << "\tKey I pressed: ";
		cin >> tree_choice;
		cin.ignore();
		switch (tree_choice)
		{
		case 1:
			type_choice = tree_type();
			if (type_choice == 1)
			{
				int_avl->createAVLTree(int_avl_root, type_choice);
			}
			else if (type_choice == 2)
			{
				double_avl->createAVLTree(double_avl_root, type_choice);
			}
			else if (type_choice == 3)
			{
				string_avl->createAVLTree(string_avl_root);
			}
			avl_tasks(type_choice);
			break;
		case 2:
			type_choice = tree_type();
			break;
		case 3:
			type_choice = tree_type();
			create_rb_tree(type_choice); // go to function where a red black tree is created
			break;
		case 4:
			removeemptylines();
			break;
		}
	}
	return 0;
}

int tree_type()
{
	int type_choice;
	cout << endl << endl;
	cout << "\t\t|-----------------------------------------------------------------------------|" << endl << endl;
	cout << "\tWhat data type would you like to choose? " << endl << endl;
	cout << "\t       ---  " << endl;
	cout << "\tPress | 1 | for INT" << endl;
	cout << "\t       ---  " << endl << endl;
	cout << "\t       ---  " << endl;
	cout << "\tPress | 2 | for FLOAT" << endl;
	cout << "\t       ---  " << endl << endl;
	cout << "\t       ---" << endl;
	cout << "\tPress | 3 | for STRING" << endl;
	cout << "\t       ---  " << endl << endl << endl;
	cout << "\t\t\t\t\t       ---" << endl;
	cout << "\t\t\t\t\tPress | 4 | to EXIT" << endl;
	cout << "\t\t\t\t\t       --- " << endl << endl;
	cout << "\tKey I pressed: ";
	cin >> type_choice;
	cin.ignore();
	return type_choice;
}

void create_rb_tree(int tree_type)
{
	int field = 0, line_no = 0, index_choice = 1, int_key, exit_choice = 0; // index_choice is the choice of index user enters 
																			// and exit_choice will be the number the user enters to exit that menu
																			// field is the number of field the iterator is at currently
	double double_key;
	string first_line, last_line;
	string line, key, path = "datafiles";
	bool comma;
	bool menu_written = false; // if menu is printed, this will turn true so it isnt printed again in the loop
	string* total_fields{}; // stores the names of the fields in case they are needed later
	for (const auto& entry : fs::directory_iterator(path)) // iterates all files in a directory
	{
		line_no = 1;
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
					if (line_no == 1 && menu_written == false) // if the first line of the file is read and the menu has not yet been printed
					{
						exit_choice = store_fields(total_fields, line); // function called to save the fields in the array total_fields
						index_choice = print_index_menu(total_fields, exit_choice);
						menu_written = true;
						//line_no++;
						if (exit_choice == index_choice) // if user enters the key to exit
							break;
					}
					if (field == index_choice - 1 && line_no != 1) // if the field user entered for index is found
					{
						if (tree_type == 1)
						{
							int_key = stoi(key);
							int_tree->insert(int_key, line_no, entry.path().u8string()); // insert to the tree;
						}
						else if (tree_type == 2)
						{
							double_key = stof(key);
							double_tree->insert(double_key, line_no, entry.path().u8string()); // insert to the tree;
						}
						else
						{
							//str_rb_tree* tree = new str_rb_tree;
							string_tree->insert(key, line_no, entry.path().u8string()); // insert to the tree;
						}

					}
					field++;
				}
				line_no++;
			}

		}
		myfile.close();
	}
	rb_tasks(tree_type, total_fields, index_choice, exit_choice); // prints and calls all the tasks that can be performed

	return;
}

bool comma_checker(string s)
{
	for (int i = 0; i < s.length(); i++)
		if (s[i] == '"')
			return 1;
	return 0;
}

int print_index_menu(string* total_fields, int& exit_choice)
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
	exit_choice = choice; // exit_choice is the key to be entered to exit this menu
	cin >> choice;
	cin.ignore();
	return choice;
}

int store_fields(string*& fields, string line)
{
	string key;
	istringstream myline(line);
	int i = 0;
	while (getline(myline, key, ',')) // count how many fields
	{
		i++;
	}
	istringstream myline2(line);
	fields = new string[i + 1]; // an array of total number of fields is printed
	i = 0;
	while (getline(myline2, fields[i], ',')) // store the fields in fields[i] in a loop
	{
		i++;
	}
	return i;
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

void rb_tasks(int tree_type, string* total_fields, int index_choice, int exit_choice)
{

	int task_choice = 0;
	while (task_choice != 5)
	{
		task_choice = print_tasks_menu();
		switch (task_choice)
		{
		case 1:
			rb_point_search(tree_type, total_fields, index_choice); // calls function to search a specific key
			break;
		case 2:
			rb_range_search(tree_type, total_fields, index_choice);
			break;
		case 3:
			rb_update_key(tree_type, total_fields, index_choice, exit_choice);
			break;
		case 4:
			rb_delete_key(tree_type, total_fields, index_choice);
			break;
		case 5:

			break;
		}
	}
}

void avl_tasks(int tree_type)
{
	if (tree_type == 1)
	{
		int_avl->avl_tasks(int_avl_root);
	}
	else if (tree_type == 2)
	{
		double_avl->avl_tasks(double_avl_root);
	}
	else if (tree_type == 3)
	{
		string_avl->avl_tasks(string_avl_root);
	}
}

void rb_range_search(int tree_type, string* total_fields, int index_choice)
{
	if (tree_type == 1)
	{
		int_tree->range_search(total_fields, index_choice);
	}
	else if (tree_type == 2)
	{
		double_tree->range_search(total_fields, index_choice);
	}
	else if (tree_type == 3)
	{
		string_tree->range_search(total_fields, index_choice);
	}
}

void rb_point_search(int tree_type, string* total_fields, int index_choice)
{
	if (tree_type == 1)
	{
		int_tree->point_search(total_fields, index_choice);
	}
	else if (tree_type == 2)
	{
		double_tree->point_search(total_fields, index_choice);
	}
	else if (tree_type == 3)
	{
		string_tree->point_search(total_fields, index_choice);
	}
}

void rb_update_key(int tree_type, string* total_fields, int index_choice, int exit_choice)
{
	cout << "\tField to update: " << endl;
	int choice = print_index_menu(total_fields, exit_choice);
	if (tree_type == 1)
	{
		int_tree->update_key(choice, total_fields, index_choice);
	}
	else if (tree_type == 2)
	{
		double_tree->update_key(choice, total_fields, index_choice);
	}
	else if (tree_type == 3)
	{
		string_tree->update_key(choice, total_fields, index_choice);
	}
}

void rb_delete_key(int tree_type, string* total_fields, int index_choice)
{
	if (tree_type == 1)
	{
		int_tree->delete_key(total_fields, index_choice);
	}
	else if (tree_type == 2)
	{
		double_tree->delete_key(total_fields, index_choice);
	}
	else if (tree_type == 3)
	{
		string_tree->delete_key(total_fields, index_choice);
	}
}

void removeemptylines()
{
	string path = "datafiles";
	string line;
	const char updated_file_path[] = "updated.csv";
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


//		create an array of tree height
//		write tree from array in a file

//void create_tree_from_array(rb_node<int>* r, rb_node<int>* arr, int n, int pos)
//{
//	if (r == NULL || arr == NULL || n == 0) { return; }
//	// Setting the left subtree of root
//	int newPos = 2 * pos + 1;
//	if (newPos < n && arr[newPos].line_no != -1)
//	{
//		r->left = new rb_node<int>(arr[newPos]);
//		create_tree_from_array(r->left, arr, n, newPos);
//	}
//	// Setting the Right subtree of root
//	newPos = 2 * pos + 2;
//	if (newPos < n && arr[newPos].line_no != -1)
//	{
//		r->right = new rb_node<int>(arr[newPos]);
//		create_tree_from_array(r->right, arr, n, newPos);
//	}
//}
//
//rb_tree<int>* read_tree_from_file(rb_tree<int>* tree)
//{
//	rb_node<int>* temp;
//	int i = 0;
//	ifstream my_file("rb_tree.txt", ios::binary);
//	if (!my_file.fail())
//	{
//		i = 0;
//		while (my_file.read((char*)&temp, sizeof(temp)))
//		{
//			i++; // to count how many objects are there in the file so we can create an array of that size
//		}
//		my_file.close();
//
//		rb_node<int>* arr = new rb_node<int>[i];
//		rb_node<int> temp2;
//
//		my_file.open("rb_tree.txt");
//		i = 0;
//		while (my_file.read((char*)&temp2, sizeof(temp2)))
//		{
//			arr[i] = temp2; // adding all objects from file to an array
//			i++;
//		}
//		my_file.close();
//		if (i == 0)
//		{
//			return tree;
//		}
//		create_tree_from_array(tree->root, arr, i, 0);
//	}
//	return tree;
//}

//		find number of objects in that file
//		create an array of that number
//		create tree from that array
//		write file from tree

//void create_array_from_tree(rb_node<int>* r, rb_node<int>* arr, int pos)
//{
//	if (r->key == NULL)
//	{
//		return;
//	}
//	arr[pos].color = r->color;
//	arr[pos].duplicate = r->duplicate;
//	arr[pos].duplicates = r->duplicates;
//	arr[pos].file_path = r->file_path;
//	arr[pos].key = r->key;
//	arr[pos].left = r->left;
//	arr[pos].line_no = r->line_no;
//	arr[pos].parent = r->parent;
//	arr[pos].right = r->right;
//	if (r->duplicate == true)
//	{
//		//while
//	}
//	if (r->left != NULL) {
//		create_array_from_tree(r->left, arr, 2 * pos + 1);
//	}
//	if (r->right != NULL) {
//		create_array_from_tree(r->right, arr, 2 * pos + 2);
//	}
//}
//
//void write_tree_to_file(rb_tree<int>* tree)
//{
//	//int i = 0;
//	//tree->total_nodes(tree->root, i);
//	int i = tree->tree_height(tree->root);
//	rb_node<int>* arr = new rb_node<int>[i];
//	create_array_from_tree(tree->root, arr, 0);
//}

//
// while (getline(myfile, line)
// {
//	field = 0;
//	string key;
//	istrinstream myline(line)
//	while (getline(myline, key, ','))
//	{
//		if (field==0)
//		{
//			field++;
//		}
//		else
//		{
//			
//		}
//	}
// }
//