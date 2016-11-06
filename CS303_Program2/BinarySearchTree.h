#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <vector>
#include "Book.h"
#include "BinaryTree.h"

template<typename Item_Type>
class BinarySearchTree : public BinaryTree<Item_Type>
{
public:
	//constructor
	BinarySearchTree<Item_Type>() { BinaryTree<Item_Type>(); }

	//destructor
	~BinarySearchTree<Item_Type>() {}

	//other functions
	virtual bool insert(const Item_Type& item);
	virtual bool insert(const Item_Type& item, const char& type);
	const vector<Item_Type> startsWith(const Item_Type& target) const;
	const vector<Book> startsWith(string& target, const char& type) const;
	//const Item_Type* find(const Item_Type& item) const;

protected:

private:
	//other functions
	virtual bool insert(BTNode<Item_Type>*& local_root, const Item_Type& item);
	virtual bool insert(BTNode<Item_Type>*& local_root, const Item_Type& item, const char& type);
	void startsWith(BTNode<Item_Type>* local_root, const Item_Type& target, vector<Item_Type>& matches) const;
	void startsWith(BTNode<Item_Type>* local_root, string& target, const char& type, vector<Item_Type>& matches) const;
	//const Item_Type* find(BTNode<Item_Type>* local_root, const Item_Type& target) const;

}; ////////////////////END OF BinarySearchTree

//wrapper insert function
template<typename Item_Type>
bool BinarySearchTree<Item_Type>::insert(const Item_Type& item)
{
	return insert(this->Root, item);
}

template<typename Item_Type>
bool BinarySearchTree<Item_Type>::insert(const Item_Type& item, const char& type) 
{
	return insert(this->Root, item, type);
}

//insert item into binary search tree
template<typename Item_Type>
bool BinarySearchTree<Item_Type>::insert(BTNode<Item_Type>*& local_root, const Item_Type& item)
{
	if (local_root == nullptr)
	{
		local_root = new BTNode<Item_Type>(item);
		return true;
	}
	else
	{
		if (item < local_root->Data)
			return insert(local_root->Left, item);
		else if (item > local_root->Data)
			return insert(local_root->Right, item);
		else
			return false;
	}
}

template<typename Item_Type>
bool BinarySearchTree<Item_Type>::insert(BTNode<Item_Type>*& local_root, const Item_Type& item, const char& type)
{
	if (local_root == nullptr)
	{
		local_root = new BTNode<Item_Type>(item);
		return true;
	}
	else
	{
		string root_text, item_text;
		switch (type)
		{
		case 'I':
		case 'i':
			root_text = local_root->Data.getISBN();
			item_text = item.getISBN();
			break;
		case 'T':
		case 't':
			root_text = local_root->Data.getTitle();
			item_text = item.getTitle();
			break;
		default:
			break;
		}
		if (item_text < root_text)
			return insert(local_root->Left, item, type);
		else if (item_text > root_text)
			return insert(local_root->Right, item, type);
		else
			return false;
	}
}

template<typename Item_Type>
const vector<Item_Type> BinarySearchTree<Item_Type>::startsWith(const Item_Type& target) const
{ 
	vector<int> matches; 
	startsWith(Root, target, matches);
	return matches;
}

template<typename Item_Type>
void BinarySearchTree<Item_Type>::startsWith(BTNode<Item_Type>* local_root, const Item_Type& target, vector<Item_Type>& matches) const
{
	if (local_root == nullptr)
		return;
	if (target == local_root->Data.substr(0, target.length()))
	{
		matches.push_back(local_root->Data);
		startsWith(local_root->Left, target, matches);
		startsWith(local_root->Right, target, matches);
	}
	else if (local_root->Data < target)
		return startsWith(local_root->Right, target, matches);
	else if (local_root->Data > target)
		return startsWith(local_root->Left, target, matches);
}

const vector<Book> BinarySearchTree<Book>::startsWith(string& target, const char& type) const
{
	vector<Book> matches;
	startsWith(Root, target, type, matches);
	return matches;
}

template<typename Item_Type>
void BinarySearchTree<Item_Type>::startsWith(BTNode<Item_Type>* local_root, string& target, const char& type, vector<Item_Type>& matches) const
{
	if (local_root == nullptr)
		return;
	string text;
	switch (type)
	{
	case 'I':
	case 'i':
		text = local_root->Data.getISBN();
		break;
	case 'T':
	case 't':
		text = local_root->Data.getTitle();
		break;
	default:
		break;
	}
	transform(target.begin(), target.end(), target.begin(), ::tolower);
	transform(text.begin(), text.end(), text.begin(), ::tolower);
	if (target == text.substr(0, target.length()))
	{
		matches.push_back(local_root->Data);
		startsWith(local_root->Left, target, type, matches);
		startsWith(local_root->Right, target, type, matches);
	}
	else if (text < target)
		return startsWith(local_root->Right, target, type, matches);
	else if (text > target)
		return startsWith(local_root->Left, target, type, matches);
}
////wrapper function to find an item in binary search tree
//template<typename Item_Type>
//const Item_Type* BinarySearchTree<Item_Type>::find(const Item_Type& item) const
//{
//	return find(this->Root, item);
//}
//
////find an item in binary search tree
//template<typename Item_Type>
//const Item_Type* BinarySearchTree<Item_Type>::find(BTNode<Item_Type>* local_root, const Item_Type& item) const
//{
//	if (local_root == nullptr)
//		return nullptr;
//	if (item < local_root->Data)
//		return find(local_root->Left, item);
//	else if (local_root->Data < item)
//		return find(local_root->Right, item);
//	else
//		return &(local_root->Data);
//}

#endif