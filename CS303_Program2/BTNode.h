#ifndef BTNODE_H
#define BTNODE_H

#include <string>
#include <sstream>

using namespace std;

template<typename Item_Type>
struct BTNode
{
	//constructor
	BTNode(const Item_Type& data, BTNode<Item_Type>* left = nullptr, 
		BTNode<Item_Type>* right = nullptr)
	{
		Data = data; Left = left; Right = right;
	}

	//destructor
	virtual ~BTNode() {}

	//convert data to string
	virtual string to_string() const 
	{
		ostringstream out;
		out << Data;
		return out.str();
	}

	//node data
	Item_Type Data;
	BTNode<Item_Type>* Left;
	BTNode<Item_Type>* Right;
}; ////////////////////END OF BTNode

//ouptut operator
template<typename Item_Type>
ostream& operator<<(std::ostream& out, const BTNode<Item_Type>& node)
{
	return out << node.to_string();
}

#endif