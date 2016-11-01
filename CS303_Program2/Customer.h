#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

#include "BinarySearchTree.h"
#include "Review.h"

using namespace std;

class Customer
{
public:
	//constructor
	Customer(int id, string name) 
	{
		ID = id; Name = name; 
	}

	//getters
	string getName() { return Name; }
	int getID() { return ID; }
	
	//other functions
	//bool insert(const Review& review);

private:
	//customer data
	string Name;
	int ID;
	//BinarySearchTree<Review> Reviewed_Books;
};

//bool Customer::insert(const Review& review) 
//{
//	return Reviewed_Books.insert(review);
//}

#endif