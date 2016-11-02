#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>

#include "Customer.h"
#include "Book.h"
#include "Review.h"
//#include "BTNode.h"
//#include "BinaryTree.h"
#include "BinarySearchTree.h"

using namespace std;

int getCustomerID(int max_ID);
char getMenuSelection();

int main()
{
	//open files
	ifstream fin_customers("customers.txt");
	ifstream fin_books("books2.txt");
	ifstream fin_ratings("ratings2.txt");

	//check to make sure files were opened
	if (!fin_customers || !fin_books || !fin_ratings)
	{
		cout << "Error opening input files!\n";
		system("pause");
		return 1;
	}

	//variables to read in customers
	int customer_ID;
	string first_name, last_name, first_line;
	char comma;
	vector<Customer> customer_vector;

	//need to clear the first line "ID, Name"
	getline(fin_customers, first_line);

	//read in customers data
	while (fin_customers >> customer_ID)
	{
		fin_customers >> comma >> first_name >> last_name;
		Customer cur_customer(customer_ID, first_name + " " + last_name);
		customer_vector.push_back(cur_customer);
	}

	//test for reading customer vector
	for (int i = 0;  i < customer_vector.size(); i++)
	{
		cout << customer_vector[i].getID() << "\t" << customer_vector[i].getName() << endl;
	}

	//read in books to binary search tree sorted by ISBN
	BinarySearchTree<Book> title_tree;
	BinarySearchTree<Book> isbn_tree;
	getline(fin_books, first_line);
	string book_ISBN;
	string book_title;
	//cout << "\n\n\n";
	while (fin_books >> book_ISBN)
	{
		//get rid of comma read in from file
		if (isdigit(book_ISBN[0]))
			book_ISBN = book_ISBN.substr(0, book_ISBN.size() - 1);
		fin_books >> book_title[0];
		fin_books.putback(book_title[0]);
		getline(fin_books, book_title);
		Book cur_book(book_ISBN, book_title);
		//cout << cur_book << endl;
		isbn_tree.insert(cur_book, 'i');
		title_tree.insert(cur_book, 't');
	}

	//////test reading in of books
	//cout << endl << endl << endl << "Output from Binary Tree\n-------------------------\n";
	//cout << book_tree.toString() << endl << endl << endl;

	// Read in reviews
	getline(fin_ratings, first_line);
	int ID, rating;
	string ISBN;
	while (fin_ratings >> ID)
	{
		char trash;
		fin_ratings >> trash >> rating >> trash >> ISBN;
		customer_vector[ID].addReview(Review(Book(ISBN, ""), rating));
	}
	customer_vector[5].printReviews();
	customer_vector[0].setSimilarities(customer_vector);
	//get customer ID entry
	customer_ID = getCustomerID(customer_vector.size() - 1);

	//display menu and get users menu selection
	char user_menu_selection = getMenuSelection();

	//perform action based on menu selection
	string user_input;
	vector<Book> book_matches, isbn_matches;
	try 
	{
		switch (user_menu_selection)
		{
		case '1':
			//read in first with >> operator to skip ws then put back and read in entire input
			cout << "Enter Book Title or ISBN: ";
			cin >> user_input[0];
			cin.putback(user_input[0]);
			getline(cin, user_input);
			cout << endl << "User Input: " << user_input << endl;
			book_matches = title_tree.startsWith(user_input, 't');
			isbn_matches = isbn_tree.startsWith(user_input, 'i');
			book_matches.insert(book_matches.end(), isbn_matches.begin(), isbn_matches.end());
			//create vector to display all "starts with" based on customer input
			//book_matches = book_tree.startsWith(user_input);
			//cout << "\n\n\n\n Book Matches: " << book_matches[0];
			break;
		case '2':
			cout << "You chose 2\n";
			break;
		case '3':
			cout << "You chose 3\n";
			break;
		case '4':
			cout << "\nExiting Program." << endl;
			system("pause");
			return 0;
			break;
		default:
			throw std::logic_error("Error in getMenuSelection: Should retrun char value between 1-4.\nExiting Program\n");
		} 
	}
	catch (logic_error e)
	{
		cerr << e.what();
		system("pause");
		return 1; 
	}

	//read in books

	//read in ratings

	//close files
	fin_customers.close();
	fin_books.close();
	fin_ratings.close();

	//end program
	cout << endl << endl << endl << "END OF PROGRAM---NO ERRORS" << endl;
	system("pause");
	return 0;
} ////////////////////END OF MAIN

int getCustomerID(int max_ID)
{
	//get customer ID entry
	bool valid_entry = false;
	string string_ID;
	int int_ID;
	while (valid_entry == false)
	{
		cout << "Enter Your Customer ID: ";
		cin >> string_ID;
		if (string_ID.find_first_not_of("0123456789") == string::npos)
		{
			int_ID = atoi(string_ID.c_str());
			if (int_ID <= max_ID && int_ID >= 0)
				valid_entry = true;
			else
				cout << "That customer ID could not be found.\n\n";
		}
		else
			cout << "Non numeric customer ID entered.\n\n";
	}
	return int_ID;
}

char getMenuSelection()
{
	//create menu for user, and get and validate input
	bool valid_entry = false;
	string user_input;
	char menu_choice;
	while (valid_entry == false)
	{
		cout << "\n--------------------\n"
			<< "1) Search for a book and probably rate it.\n"
			<< "2) Rate a book they had not rated before.\n"
			<< "3) View book recommendations sorted by relevance.\n"
			<< "4) Exit Program.\n--------------------\n"
			<< "Choose what you would like to do. (Enter 1, 2, 3, or 4): ";
		cin >> user_input;
		//if they entered more than one character, go ahead set it to
		//data that isn't valid to avoid accepting false input
		if (user_input.size() > 1)
			user_input = "abc";
		switch (user_input[0])
		{
		case '1':
			menu_choice = user_input[0];
			valid_entry = true; break;
		case '2':
			menu_choice = user_input[0];
			valid_entry = true; break;
		case '3':
			menu_choice = user_input[0];
			valid_entry = true; break;
		case '4':
			menu_choice = user_input[0];
			valid_entry = true; break;
		default:
			cout << "\nInvalid entry. Enter one of the menu options given.\n";
			valid_entry = false;
		}
	}
	return menu_choice;
}

//////////////////////////////////////////////////////////////////////////////////changed Book class ISBN to string
////Binary Search Tree Test
//BinarySearchTree<Book> book_tree;
//getline(fin_books, first_line);
////int book_ISBN;
//string string_ISBN;
//string book_title;
//while (fin_books >> string_ISBN)
//{
//	if (isdigit(string_ISBN[0]))
//		string_ISBN = string_ISBN.substr(0, string_ISBN.size() - 1);
//	getline(fin_books, book_title);
//	Book cur_book(string_ISBN, book_title);
//	cout << cur_book << endl;
//	book_tree.insert(cur_book);
//}
//cout << endl << endl << endl << book_tree.toString() << endl << endl << endl;

////code to create menu for user without function
//bool valid_entry = false;
//string user_input;
//while (valid_entry == false)
//{
//	cout << "\n1) Search for a book and probably rate it.\n"
//		<< "2) Rate a book they had not rated before.\n"
//		<< "3) View book recommendations sorted by relevance.\n\n"
//		<< "Choose what you would like to do: ";
//	cin >> user_input;
//	switch (user_input[0])
//	{
//	case '1':
//		cout << "You chose 1\n"; 
//		valid_entry = true; break;
//	case '2':
//		cout << "You chose 2\n"; 
//		valid_entry = true; break;
//	case '3':
//		cout << "You chose 3\n"; 
//		valid_entry = true; break;
//	default:
//		cout << "\nInvalid entry. Enter one of the menu options given.\n";
//		valid_entry = false;
//	}
//}

////code to get customer ID w/o function
//bool valid_entry = false;
//string ID;
//while (valid_entry == false)
//{
//	cout << "Enter Your Customer ID: ";
//	cin >> ID;
//	if (ID.find_first_not_of("0123456789") == string::npos)
//	{
//		customer_ID = atoi(ID.c_str());
//		if (customer_ID < customer_vector.size() && customer_ID >= 0)
//			valid_entry = true;
//		else
//			cout << "That customer ID could not be found.\n\n";
//	}
//	/*if (customer_ID < customer_vector.size() && customer_ID >= 0)
//		valid_entry = true;*/
//	else
//		cout << "Non numeric customer ID entered.\n\n";
//}