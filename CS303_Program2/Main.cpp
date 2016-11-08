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

int getBookRating(Book book, int min, int max);
int getBookSelection(int max);
int getCustomerID(int max_ID);
char getMenuSelection();

int main()
{
	//open files
	ifstream fin_customers("customers.txt");
	ifstream fin_books("books.txt");
	ifstream fin_ratings("ratings.txt");

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

	//variables to read in books
	BinarySearchTree<Book> title_tree;
	BinarySearchTree<Book> isbn_tree;
	string book_ISBN;
	string book_title;

	//read in un-needed first line of file
	getline(fin_books, first_line);
	//cout << "\n\n\n";

	//read in books to a BST sorted by ISBN and a BST sorted by title
	while (fin_books >> book_ISBN)
	{
		//get rid of comma read in from file
		if (isdigit(book_ISBN[0]))
			book_ISBN = book_ISBN.substr(0, book_ISBN.size() - 1);
		//skip all leading whitespace before title
		fin_books >> book_title[0];
		fin_books.putback(book_title[0]);
		//create a book
		getline(fin_books, book_title);
		Book cur_book(book_ISBN, book_title);
		//cout << cur_book << endl;
		//call appropriate version of insert function for both BST
		isbn_tree.insert(cur_book, 'i');
		title_tree.insert(cur_book, 't');
	}

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

	////test getRecommendations for first customer
	//vector<Review> recs = customer_vector[0].getRecommendations(customer_vector);
	//customer_vector[5].printReviews();

	// Code below calculates similarities for all users. Only used to test speed of similarity function
	//for (int i = 0; i < customer_vector.size(); i++)
	//{
	//	customer_vector[i].setSimilarities(customer_vector);
	//}

	//continue until a valid user ID is entered and they choose option to end program
	char user_menu_selection = '0';
	while (user_menu_selection != '4')
	{
		//get customer ID entry
		customer_ID = getCustomerID(customer_vector.size() - 1);

		//display menu and get users menu selection
		user_menu_selection = getMenuSelection();

		//perform action based on menu selection
		string user_input;
		vector<Book> book_matches, isbn_matches;
		vector<Review> recommendations;
		try
		{
			switch (user_menu_selection)
			{
			case '1':
				cout << "Enter Book Title or ISBN: ";
				//clear leading whitespace
				cin >> user_input[0];
				cin.putback(user_input[0]);
				//get users input
				getline(cin, user_input);
				////test to make sure user input was read in correctly
				//cout << "\nUser Input: " << user_input << endl;
				//retrieve all title matches and all isbn matches, then combine them
				book_matches = title_tree.startsWith(user_input, 't');
				isbn_matches = isbn_tree.startsWith(user_input, 'i');
				book_matches.insert(book_matches.end(), isbn_matches.begin(), isbn_matches.end());
				//output all matching books in a numbered list if any matches were found
				if (book_matches.size() > 0)
				{
					cout << "--------------------\nBook Matches\n--------------------\n";
					for (int i = 0; i < book_matches.size(); i++)
					{
						cout << i << ".) " << book_matches[i] << endl;
					}
					cout << book_matches.size() << ".) None of the Above" << endl;
					//have user select which book match they would like to rate
					int book_selection = getBookSelection(book_matches.size());
					//user didnt find book they were looking for on list, don't ask for rating
					if (book_selection == book_matches.size())
					{
						cout << "Sorry you didn't find the book you were looking for.\n\n";
						break;
					}
					//cout << "You chose the book: " << book_matches[book_selection] << endl;
					rating = getBookRating(book_matches[book_selection], 1, 5);
					cout << endl;
					customer_vector[customer_ID].addReview(Review(book_matches[book_selection], rating));
				}
				else
					cout << "No book matches found.\n\n";
				break;
			case '2':
				cout << "Enter Book Title or ISBN: ";
				//clear leading whitespace
				cin >> user_input[0];
				cin.putback(user_input[0]);
				//get users input
				getline(cin, user_input);
				////test to make sure user input was read in correctly
				//cout << "\nUser Input: " << user_input << endl;
				//retrieve all title matches and all isbn matches, then combine them
				book_matches = title_tree.startsWith(user_input, 't');
				isbn_matches = isbn_tree.startsWith(user_input, 'i');
				book_matches.insert(book_matches.end(), isbn_matches.begin(), isbn_matches.end());
				//output all matching books in a numbered list if any matches were found
				if (book_matches.size() > 0)
				{
					cout << "--------------------\nBook Matches\n--------------------\n";
					for (int i = 0; i < book_matches.size(); i++)
					{
						cout << i << ".) " << book_matches[i] << endl;
					}
					cout << book_matches.size() << ".) None of the Above" << endl;
					//have user select which book match they would like to rate
					int book_selection = getBookSelection(book_matches.size());
					//user didnt find book they were looking for on list, don't ask for rating
					if (book_selection == book_matches.size())
					{
						cout << "Sorry you didn't find the book you were looking for.\n\n";
						break;
					}
					//cout << "You chose the book: " << book_matches[book_selection] << endl;
					rating = getBookRating(book_matches[book_selection], 1, 5);
					cout << endl;
					customer_vector[customer_ID].addReview(Review(book_matches[book_selection], rating));
				}
				else
					cout << "No book matches found.\n\n";
				break;
			case '3':
				//create a vector of recommendations
				cout << "--------------------\nRecommendations\n--------------------\n";
				recommendations = customer_vector[customer_ID].getRecommendations(customer_vector);
				if (recommendations.size() == 0)
				{
					//add code to list the top 10 overall rated books
				}
				if (recommendations.size() < 10)
				{
					for (int i = 0; i < recommendations.size(); i++)
						cout << recommendations[i] << endl;
				}
				else
				{
					for (int i = 0; i < 10; i++)
						cout << recommendations[i] << endl;
				}
				cout << endl;
				//customer_vector[customer_ID].printReviews();
				//vector<Review> recs = customer_vector[0].getRecommendations(customer_vector);
				//customer_vector[5].printReviews();
				break;
			case '4':
				cout << "\nEnd of Program.\n" << endl;
				//system("pause");
				//return 0;
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
	}

	//close files
	fin_customers.close();
	fin_books.close();
	fin_ratings.close();

	//end program
	system("pause");
	return 0;
} ////////////////////END OF MAIN

int getBookRating(Book book, int min, int max)
{
	bool valid_entry = false;
	string string_input;
	int int_input;
	while (valid_entry == false)
	{
		cout << "\nEnter your rating of the book: " << book
			<< "\nRating [1-5]: ";
		cin >> string_input;
		if (string_input.find_first_not_of("0123456789") == string::npos)
		{
			int_input = atoi(string_input.c_str());
			if (int_input <= max && int_input >= min)
				valid_entry = true;
			else
				cout << "Entry Out of Range." << endl;
		}
		else
			cout << "Please enter a number in the range [1, 5]." << endl;
	}
	return int_input;
}

int getBookSelection(int max)
{
	//get customer ID entry
	bool valid_entry = false;
	string string_selection;
	int int_selection;
	//continue until user enters a valid ID
	while (valid_entry == false)
	{
		cout << "Enter list number of the book you would like to rate: ";
		cin >> string_selection;
		if (string_selection.find_first_not_of("0123456789") == string::npos)
		{
			int_selection = atoi(string_selection.c_str());
			if (int_selection <= max && int_selection >= 0)
				valid_entry = true;
			else
				cout << "Entry Out of Range.\n";
		}
		else
			cout << "Please enter a number from the list.\n";
	}
	return int_selection;
}

int getCustomerID(int max_ID)
{
	//get customer ID entry
	bool valid_entry = false;
	string string_ID;
	int int_ID;
	//continue until user enters a valid ID
	while (valid_entry == false)
	{
		cout << "Enter Your Customer ID [0, " << max_ID << "]: ";
		cin >> string_ID;
		if (string_ID.find_first_not_of("0123456789") == string::npos)
		{
			int_ID = atoi(string_ID.c_str());
			if (int_ID <= max_ID && int_ID >= 0)
				valid_entry = true;
			else
				cout << "That customer ID could not be found.\n";
		}
		else
			cout << "Non numeric customer ID entered.\n";
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
		cout << "--------------------\n"
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