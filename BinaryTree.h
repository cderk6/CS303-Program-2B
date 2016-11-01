#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <stdexcept>
#include <string>

#include "BTNode.h"

template<typename Item_Type>
class BinaryTree
{
public:
	//constructor
	BinaryTree() { Root = nullptr; }

	//destructor
	virtual ~BinaryTree() {}

	//access subtrees
	BinaryTree<Item_Type> getLeftSubtree() const;
	BinaryTree<Item_Type> getRightSubtree() const;

	//getters
	BTNode<Item_Type>* getRoot() { return Root; }
	const Item_Type& getData() const;

	//other functions
	bool isLeaf() const;
	bool isNull() const { return Root == nullptr; }
	virtual string toString() const;

protected:
	//binary tree data
	BTNode<Item_Type>* Root;

	//constructor for getLeftSubtree() and getRightSubtree()
	BinaryTree(BTNode<Item_Type>* new_root) : Root(new_root) {}

private:

}; ////////////////////END OF BinaryTree

//get left subtree
template<typename Item_Type>
BinaryTree<Item_Type> BinaryTree<Item_Type>::getLeftSubtree() const 
{
	if (Root == nullptr)
		throw std::invalid_argument("Tried to retrieve empty left subtree");
	return BinaryTree<Item_Type>(Root->Left);
}

//get right subtree
template<typename Item_Type>
BinaryTree<Item_Type> BinaryTree<Item_Type>::getRightSubtree() const
{
	if (Root == nullptr)
		throw std::invalid_argument("Tried to retrieve empty right subtree");
	return BinaryTree<Item_Type>(Root->Right);
}

//get data of current node
template<typename Item_Type>
const Item_Type& BinaryTree<Item_Type>::getData() const
{
	if (Root == nullptr)
		throw std::invalid_argument("get_data on null tree");
	return Root->Data;
}

//check if your at a leaf
template<typename Item_Type>
bool BinaryTree<Item_Type>::isLeaf() const 
{
	if (Root != nullptr)
		return Root->Left == nullptr && Root->Right == nullptr;
	else
		return true;
}

//output tree as a string
template<typename Item_Type>
string BinaryTree<Item_Type>::toString() const 
{
	ostringstream os;
	if (isNull())
		os << "NULL\n";
	else {
		os << *Root << '\n';
		os << "Left of: " << *Root << "\n";
		os << getLeftSubtree().toString();
		os << "Right of: " << *Root << "\n";
		os << getRightSubtree().toString();
	}
	return os.str();
}

#endif