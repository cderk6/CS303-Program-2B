#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <algorithm>
#include <string>
#include <vector>
#include "BinarySearchTree.h"
#include "Review.h"

using namespace std;

class Customer
{
public:
	//constructor
	Customer(int id, string name) 
	{
		ID = id; Name = name; reviews = {};
	}

	//getters
	string getName() { return Name; }
	int getID() { return ID; }
	
	//other functions
	//bool insert(const Review& review);
	void addReview(const Review& review);
	void printReviews();

private:
	//customer data
	string Name;
	int ID;
	vector<Review> reviews;
	//BinarySearchTree<Review> Reviewed_Books;
};

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

#endif