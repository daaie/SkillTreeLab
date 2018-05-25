#include "DLList.h"
#include <list>

using namespace std;

int main()
{
	DLList<int> IntList;

	IntList.AddNode(1);
	IntList.AddNode(2);
	IntList.AddNode(3);

	for (DLNode<int> *Current = IntList.Begin(); Current != IntList.End()->Next ; Current = Current->Next)
	{
		printf("%d\n", Current->Value);
	}

	list<int> l;
	
	l.push_back(10);
	l.push_back(20);
	l.push_back(30);
	l.push_back(40);

	for (list<int>::iterator i = l.begin(); i != l.end(); i++)
	{
		printf("%d\n", *i);
	}
	return 0;
}