#ifndef REVIEW_H
#define REVIEW_H

#include <string>

#include "Book.h"

using namespace std;

class Review
{
public:
	//constructors
	Review() { Reviewed_Book = Book(); Rating = 0; sum_of_degrees = 0; sum_of_ratings = 0; num_of_ratings = 0; }
	Review(Book book, double rating, double sum_of_deg = 0, double sum_of_rat = 0) 
	{ 
		Reviewed_Book = book; Rating = rating; sum_of_degrees = sum_of_deg; sum_of_ratings = sum_of_rat; num_of_ratings = 1;
	}

	//getters
	double getRating() const { return Rating; }
	Book getBook() const { return Reviewed_Book; }
	double getSumOfDegrees() const { return sum_of_degrees; }
	double getSumOfRatings() const { return sum_of_ratings; }
	int getNumOfRatings() const { return num_of_ratings; }

	//setters
	void setRating(const double& new_rating) { Rating = new_rating; }
	void setBook(const Book& book) { Reviewed_Book = book; }

	void addToSums(const double& deg, const double& rat) { sum_of_degrees += deg; sum_of_ratings += deg * rat; num_of_ratings++; }

private:
	//review data
	Book Reviewed_Book;
	double Rating;
	double sum_of_degrees;
	double sum_of_ratings;
	int num_of_ratings;
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