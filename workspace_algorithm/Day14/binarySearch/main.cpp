#include <iostream>

using namespace std;

template<typename T>
class BTNode
{
public:
	BTNode * left;
	BTNode * Right;
	T Data;
};

template<typename T>
class BTree
{
public:
	BTNode<T> *Root;
	BTNode<T>* MakeNode();
	void MakeRoot();
	void MakeLeftChild(BTNode<T> *Node);
	void MakeRightChild(BTNode<T> *Node);
	void SetData(BTNode<T> *Node, int Value);
	void PreorderSearch(BTNode<T> *Node);
	void InorderSearch(BTNode<T>* Node);
	void PostorderSearch(BTNode<T>* Node);
};

int main()
{
	BTree<int> TestTree;

	TestTree.MakeRoot();
	TestTree.MakeLeftChild(TestTree.Root);
	TestTree.MakeRightChild(TestTree.Root);

	TestTree.SetData(TestTree.Root,1);
	TestTree.SetData(TestTree.Root->left, 2);
	TestTree.SetData(TestTree.Root->Right, 3);

	TestTree.MakeLeftChild(TestTree.Root->left);
	TestTree.MakeRightChild(TestTree.Root->left);
	TestTree.MakeLeftChild(TestTree.Root->Right);
	TestTree.MakeRightChild(TestTree.Root->Right);

	TestTree.SetData(TestTree.Root->left->left, 4);
	TestTree.SetData(TestTree.Root->left->Right, 5);
	TestTree.SetData(TestTree.Root->Right->left, 6);
	TestTree.SetData(TestTree.Root->Right->Right, 7);


	cout << "PreOrder : ";
	TestTree.PreorderSearch(TestTree.Root);
	cout << endl;

	cout << "InOrder : ";
	TestTree.InorderSearch(TestTree.Root);
	cout << endl;

	cout << "PostOrder : ";
	TestTree.PostorderSearch(TestTree.Root);
	cout << endl;
	return 0;
}

template<typename T>
BTNode<T> *BTree<T>::MakeNode()
{
	BTNode<T> *Node = new BTNode<T>();
	return Node;
}

template<typename T>
void BTree<T>::MakeRoot()
{
	Root = new BTNode<T>();
}

template<typename T>
void BTree<T>::MakeLeftChild(BTNode<T> *Node)
{
	Node->left = new BTNode<T>();
}

template<typename T>
void BTree<T>::MakeRightChild(BTNode<T> *Node)
{
	Node->Right = new BTNode<T>();
}

template<typename T>
void BTree<T>::SetData(BTNode<T> *Node, int Value)
{
	Node->Data = Value;
}

template<typename T>
void BTree<T>::PreorderSearch(BTNode<T> * Node)
{
	if (Node == NULL) return;

	cout << Node->Data<<" ";
	PreorderSearch(Node->left);
	PreorderSearch(Node->Right);
}

template<typename T>
void BTree<T>::InorderSearch(BTNode<T> * Node)
{
	if (Node == NULL) return;

	InorderSearch(Node->left);
	cout << Node->Data << " ";
	InorderSearch(Node->Right);
}

template<typename T>
void BTree<T>::PostorderSearch(BTNode<T> * Node)
{
	if (Node == NULL) return;

	PostorderSearch(Node->left);
	PostorderSearch(Node->Right);
	cout << Node->Data << " ";
}

