#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
	Node* left;
	Node* right;

	int data;
}TagNode;

TagNode* Create(int data)
{
	TagNode *newNode = new TagNode();
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->data = data;

	return newNode;
}

void Destroy(TagNode *node)
{
	delete node;
}

void DestroyTree(TagNode *tree)
{
	if (tree->right != NULL)
	{
		DestroyTree(tree->right);
	}

	if (tree->left != NULL)
	{
		DestroyTree(tree->left);
	}

	tree->left = NULL;
	tree->right = NULL;

	Destroy(tree);
}


TagNode *Search(TagNode *tree, int target)
{
	if (tree == NULL)
		return NULL;

	if (tree->data == target)
	{
		return tree;
	}
	else if (tree->data > target)
	{
		return Search(tree->left, target);
	}
	else
		return Search(tree->right, target);
}

TagNode *SearchMin(TagNode *tree)
{
	if (tree == NULL)
	{
		return NULL;
	}

	if (tree->left == NULL)
	{
		return tree;
	}
	else
	{
		return SearchMin(tree->left);
	}
}

void Insert(TagNode *tree,TagNode *child)
{
	if (tree->data < child->data)
	{
		if (tree->right == NULL)
			tree->right = child;
		else
			Insert(tree->right, child);
	}
	else if (tree->data > child->data)
	{
		if (tree->left == NULL)
		{
			tree->left = child;
		}
		else
			Insert(tree->left, child);
	}
}

TagNode *Remove(TagNode *tree, TagNode *parent, int target)
{
	TagNode * removed = NULL;

	if (tree == NULL)
		return NULL;

	if (tree->data > target)
	{
		removed = Remove(tree->left, tree, target);
	}
	else if (tree->data < target)
	{
		removed = Remove(tree->right, tree, target);
	}
	else
	{
		removed = tree;
		if (tree->left == NULL && tree->right == NULL)
		{
			if (parent->left == tree)
				parent->left = NULL;
			else
				parent->right = NULL;
		}
		else
		{
			if (tree->left != NULL && tree->right != NULL)
			{
				TagNode *minNode = SearchMin(tree->right);
				minNode = Remove(tree, NULL, minNode->data);
				tree->data = minNode->data;
			}
			else
			{
				TagNode *temp = NULL;
				if (tree->left != NULL)
				{
					temp = tree->left;
				}
				else
					temp = tree->right;

				if (parent->left == tree)
					parent->left = temp;
				else
					parent->right = temp;
			}
		}
	}
	return removed;
}

void Print(TagNode *Node)
{
	if (Node == NULL)
	{
		return;
	}

	Print(Node->left);
	printf("%d ", Node->data);

	Print(Node->right);
}

int main()
{
	TagNode * tree = Create(123);
	TagNode * Node = NULL;

	Insert(tree, Create(22));
	Insert(tree, Create(1111));
	Insert(tree, Create(444));
	Insert(tree, Create(11));
	Insert(tree, Create(6));
	Insert(tree, Create(777));
	Insert(tree, Create(290));
	Insert(tree, Create(333));
	Insert(tree, Create(1024));

	Print(tree);
	printf("\n");

	printf("remove 22\n");
	Node = Remove(tree, NULL, 22);
	Destroy(Node);
	Print(tree);
	printf("\n");

	printf("insert 22\n");
	Insert(tree, Create(22));
	Print(tree);
	printf("\n");

	DestroyTree(tree);
	return 0;
}