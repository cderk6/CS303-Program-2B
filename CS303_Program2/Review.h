#ifndef REVIEW_H
#define REVIEW_H

#include <string>

#include "Book.h"

using namespace std;

class Review
{
public:
	//constructors
	Review() { Reviewed_Book = Book(); Rating = 0; }
	Review(Book book, int rating) { Reviewed_Book = book, Rating = rating; }

	//getters
	int getRating() const { return Rating; }
	Book getBook() const { return Reviewed_Book; }

	//setters
	void setRating(const int& new_rating) { Rating = new_rating; }
	void setBook(const Book& book) { Reviewed_Book = book; }

private:
	//review data
	Book Reviewed_Book;
	int Rating;
};

bool operator <(const Review& lhs, const Review& rhs)
{
	if (lhs.getBook() < rhs.getBook())
		return true;
	return false;
}

bool operator >(const Review& lhs, const Review& rhs)
{
	if (lhs.getBook() > rhs.getBook())
		return true;
	return false;
}

ostream& operator<<(ostream& out, const Review& review)
{
	out << review.getBook() << ": " << review.getRating();
	return out;
}

#endif