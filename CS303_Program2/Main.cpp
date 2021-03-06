#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>

#include "Customer.h"
#include "Book.h"
#include "Review.h"
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

	//variables to read in books
	BinarySearchTree<Book> title_tree;
	BinarySearchTree<Book> isbn_tree;
	string book_ISBN;
	string book_title;

	//read in un-needed first line of file
	getline(fin_books, first_line);

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

	//continue until a valid user ID is entered and they choose option to end program
	char user_menu_selection = '0';
	customer_ID = -1;
	while (user_menu_selection != '3')
	{
		//get customer ID entry
		if(customer_ID == -1)
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
				//retrieve all title matches and all isbn matches, then combine them
#pragma omp parallel sections
				{
#pragma omp section
					{
						book_matches = title_tree.startsWith(user_input, 't');
					}
#pragma omp section
					{
						isbn_matches = isbn_tree.startsWith(user_input, 'i');
					}
				}
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
					rating = getBookRating(book_matches[book_selection], 1, 5);
					cout << "Thanks for reviewing " << book_matches[book_selection] << "!" << endl;
					customer_vector[customer_ID].addReview(Review(book_matches[book_selection], rating));
				}
				else
					cout << "No book matches found.\n\n";
				break;
			case '2':
			{
				//create a vector of recommendations
				cout << "--------------------\nWeighted Ratings from Similar Users\n--------------------\n";
				recommendations = customer_vector[customer_ID].getRecommendations(customer_vector);
				int count = 0, idx = 0;
				//output 10 recommendations or as many as the customer has
				while (count < 10 && idx < recommendations.size())
				{
					if (customer_vector[customer_ID].hasRead(recommendations[idx]))
					{
						++idx;
					}
					else if (recommendations[idx].getAdjRating() < 3)
					{
						break;
					}
					else
					{
						cout << setprecision(3) << recommendations[idx].getAdjRating()  << ": " << recommendations[idx].getBook().getISBN() << ", " << isbn_tree.getTitle(recommendations[idx].getBook().getISBN()) << endl;
						++idx;
						++count;
					}
				}
				count = 0;
				idx = 0;
				vector<Review> raw_recs = customer_vector[customer_ID].getRawRecommendations();
				//output top 10 overall books
				cout << "--------------------\nTop Rated Books\n--------------------\n";
				while (count < 10 && idx < raw_recs.size())
				{
					if (customer_vector[customer_ID].hasRead(raw_recs[idx]))
					{
						++idx;
					}
					else
					{
						cout << showpoint << setprecision(3) << raw_recs[idx].getRating() << ": " << raw_recs[idx].getBook().getISBN() 
							<< ", " << isbn_tree.getTitle(raw_recs[idx].getBook().getISBN()) << endl;
						++idx;
						++count;
					}
				}
				cout << endl;
			}
				break;
			case '3':
				cout << "\nEnd of Program.\n" << endl;
				break;
			default:
				throw std::logic_error("Error in getMenuSelection: Should return char value between 1-3.\nExiting Program\n");
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
			<< "2) View book recommendations sorted by relevance.\n"
			<< "3) Exit Program.\n--------------------\n"
			<< "Choose what you would like to do. (Enter 1, 2, or 3): ";
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
		default:
			cout << "\nInvalid entry. Enter one of the menu options given.\n";
			valid_entry = false;
		}
	}
	return menu_choice;
}