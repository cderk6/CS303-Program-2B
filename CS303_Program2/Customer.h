#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <algorithm>
#include <string>
#include <vector>
#include "BinarySearchTree.h"
#include "Review.h"
#include "omp.h"

using namespace std;

class Customer
{
public:
	//constructor
	Customer(int id, string name) 
	{
		ID = id; Name = name; reviews = {}, degrees_of_similarity = {};
	}

	//getters
	string getName() { return Name; }
	int getID() { return ID; }
	int getNumReviews() { return reviews.size(); }
	Review& getReview(int idx);
	//other functions
	//bool insert(const Review& review);
	void addReview(const Review& review);
	void printReviews();
	void setSimilarities(vector<Customer>& customers);
	int binarySearch(int lower, int upper, Review& target, vector<Review> vec);
	vector<Review> getRecommendations(vector<Customer>& customers);

private:
	//customer data
	string Name;
	int ID;
	vector<Review> reviews;
	vector<double> degrees_of_similarity;
	vector<Review> all_reviews;
	//BinarySearchTree<Review> Reviewed_Books;
};

Review& Customer::getReview(int idx)
{
	if (idx < reviews.size())
		return reviews[idx];
	// Raise error if out of bounds
}

void Customer::addReview(const Review& review) 
{
	reviews.insert(upper_bound(reviews.cbegin(), reviews.cend(), review), review);
}

void Customer::printReviews()
{
	for (int i = 0; i < reviews.size(); i++)
	{
		cout << reviews[i] << endl;
	}
}

void Customer::setSimilarities(vector<Customer>& customers)
{
	// Forumla:
	//		(5 - avg. rating diff.) / 5 * (# of books in common / total # of reviews for target customer)
	degrees_of_similarity.clear();
	omp_set_num_threads(2);
#pragma omp parallel for
	for (int i = 0; i < customers.size(); i++)
	{
		degrees_of_similarity.push_back(0);
	}
#pragma omp parallel for
	for (int i = 0; i < customers.size(); i++)
	{
		double avg_diff = 0, total_diff = 0, in_common = 0, deg_of_sim = 0;
		for (int j = 0; j < customers[i].getNumReviews(); j++)
		{
			int idx = binarySearch(0, getNumReviews() - 1, customers[i].getReview(j), reviews);
			if (idx != -1)
			{
				++in_common;
				total_diff += abs(getReview(idx).getRating() - customers[i].getReview(j).getRating());
			}
		}
		if (in_common > 0)
			avg_diff = total_diff / in_common;
		deg_of_sim = (5 - avg_diff) / 5 * (in_common / getNumReviews());
		degrees_of_similarity[i] = deg_of_sim;
	}
}

int Customer::binarySearch(int lower, int upper, Review& target, vector<Review> vec)
{
	if (lower > upper)
		return -1;
	int mid = (lower + upper) / 2;
	if (target.getBook().getISBN() == vec[mid].getBook().getISBN())
		return mid;
	else if (target > vec[mid])
		return binarySearch(mid + 1, upper, target, vec);
	else
		return binarySearch(lower, mid - 1, target, vec);
}

vector<Review> Customer::getRecommendations(vector<Customer>& customers)
{
	vector<Book> returnstuff;
	setSimilarities(customers);
	for (int i = 0; i < customers.size(); i++)
	{
		for (int j = 0; j < customers[i].getNumReviews(); j++)
		{
			//int idx = binarySearch(0, all_reviews.size() - 1, customers[i].getReview(j), all_reviews);
			int idx = -1;
			for (int k = 0; k < all_reviews.size(); k++)
			{
				if (all_reviews[k].getBook().getISBN() == customers[i].getReview(j).getBook().getISBN())
				{
					idx = k;
					break;
				}
			}
			if (idx == -1)
			{
				all_reviews.push_back(Review(Book(customers[i].getReview(j).getBook().getISBN(), ""), 0, degrees_of_similarity[i], degrees_of_similarity[i] * customers[i].getReview(j).getRating()));
			}
			else
			{
				all_reviews[idx].addToSums(degrees_of_similarity[i], customers[i].getReview(j).getRating());
			}
		}
	}
	for (int i = 0; i < all_reviews.size(); i++)
	{
		all_reviews[i].setRating((all_reviews[i].getSumOfRatings() / all_reviews[i].getSumOfDegrees()) * (.5 + .25 * (1 - (1.0 / (all_reviews[i].getNumOfRatings() + 1))) + .25 * (all_reviews[i].getSumOfDegrees() / all_reviews[i].getNumOfRatings())));
	}
	for (int i = 0; i < all_reviews.size(); i++)
	{
		int max = i;
		for (int j = i + 1; j < all_reviews.size(); j++)
		{
			if (all_reviews[j].getRating() > all_reviews[max].getRating())
				max = j;
		}
		swap(all_reviews[i], all_reviews[max]);
	}
	return all_reviews;
}
#endif