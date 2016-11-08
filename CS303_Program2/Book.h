#ifndef BOOK_H
#define BOOK_H

#include <string>

using namespace std;

class Book
{
public:
	//constructor
	Book(string isbn = "", string title = "") { Title = title; ISBN = isbn; }

	//getters
	string getTitle() const { return Title; }
	string getISBN() const { return ISBN; }

private:
	string ISBN;
	string Title;
};

//comparison operators based off of ISBN
bool operator <(const Book& lhs, const Book& rhs)
{
	if (lhs.getISBN() < rhs.getISBN())
		return true;
	return false;
}

bool operator >(const Book& lhs, const Book& rhs)
{
	if (lhs.getISBN() > rhs.getISBN())
		return true;
	return false;
}

bool operator ==(const Book& lhs, const Book&rhs)
{
	if (lhs.getISBN() == rhs.getISBN())
		return true;
	return false;
}

ostream& operator<<(ostream& out, const Book& book)
{
	out << book.getISBN() << ", " << book.getTitle();
	return out;
}

#endif