#include "DLList.h"

int main()
{
	DLList<int> dList;
	
	for (int i = 1; i <= 10; i++)
	{
		dList.AddNode(i);
	}
	
	DLNode<int> *Last = dList.End();

	for (int i = 1; i <= 10; i++)
	{
		cout << Last->Value << "\t";
		Last = Last->Prev;
	}

	return 0;
}
