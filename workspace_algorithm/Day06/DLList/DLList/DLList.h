#pragma once

#include <iostream>

using namespace std;

namespace PdaDLList
{
	//template <typename T, typename Y >이런식으로 두개받을수도있다.
	template<typename T>
	class DLLNode
	{
	public:
		//DLLNode() { Value = 0; Prev = NULL; Next = NULL; };
		//선언을 안해주면 알아서 복사생성자 만들어준다..ㅋㅋ컴파일러 짱짱인듯
		T Value;
		DLLNode* Prev;
		DLLNode* Next;
	};

	template<typename T>
	class DLList
	{
	private:
		DLLNode<T> *Head;
		DLLNode<T> *Tail;

		DLLNode<T>* MakeNode(T value)
		{
			//DLLNode<T>* TmpNode = (DLLNode<T>*)malloc(sizeof(DLLNode<T>));
			DLLNode<T>* TmpNode = new DLLNode<T>();

			TmpNode->Value = value;
			//TmpNode->Prev = NULL;
			//TmpNode->Next = NULL;

			return TmpNode;
		}
		DLLNode<T>* SearchNode(T value);

		long count;
		//컨트롤 점쩐다..
	public:
		DLList();
		~DLList() {}; //특별히 하는일이없댱

		DLLNode<T> *Delete(T value);		
		DLLNode<T> *IndexOf(int Index);

		void Push_back(T value);
		void Push_front(T value);
		void Print();

		long Length() { return count; };

		DLLNode<T> *Begin() { return Head->Next; };
		DLLNode<T> *End() { return Tail; };
	};

	template<typename T>
	DLList<T>::DLList()
	{
		//Head = MakeNode(NULL);
		Head = new DLLNode<T>();
		//Tail = MakeNode(NULL);
		Tail= new DLLNode<T>();

		Head->Next = Tail;
		Tail->Prev = Head;

		count = 0;
	};

	template<typename T>
	DLLNode<T>* DLList<T>::SearchNode(T value)
	{
		for (DLLNode<T>* TmpNode = Head->Next; TmpNode != Tail; TmpNode = TmpNode->Next)
		{
			if (TmpNode->Value == value)
			{
				return TmpNode;
			}
		}
		return nullptr;
	}

	template<typename T>
	void DLList<T>::Push_back(T value)
	{
		DLLNode<T> *TmpNode = MakeNode(value);

		TmpNode->Prev = Tail->Prev;
		TmpNode->Next = Tail;
		Tail->Prev->Next = TmpNode;
		Tail->Prev = TmpNode;

		count++;
	}

	template<typename T>
	void DLList<T>::Push_front(T value)
	{
		DLLNode<T> *TmpNode = MakeNode(value);

		TmpNode->Prev = Head;
		TmpNode->Next = Head->Next;
		Head->Next->Prev = TmpNode;
		Head->Next = TmpNode;

		count++;
	}

	template<typename T>
	DLLNode<T> * DLList<T>::Delete(T value)
	{
		DLLNode<T> * TmpNode = SearchNode(value);
		DLLNode<T> * PrevNode = NULL;

		//if (TmpNode -> Value == Value , TmpNode != Head, TmpNode->Tail)
		if(TmpNode)
		{
			PrevNode = TmpNode->Prev;
			TmpNode->Prev->Next = TmpNode->Next;
			TmpNode->Next->Prev = TmpNode->Prev;
			delete TmpNode;		
			count--;
		}

		return PrevNode;
	}

	template<typename T>
	void DLList<T>::Print()
	{
		for (DLLNode<T>* TmpNode = Head->Next; TmpNode != Tail; TmpNode = TmpNode->Next)
		{
			cout << TmpNode->Value <<endl;
		}

		printf("----------------------------------------------\n");
	}

	template<typename T>
	inline DLLNode<T>* DLList<T>::IndexOf(int Index)
	{
		long ObjectCount = 0;
		for (DLLNode<T>* Current = Head; Current != Tail; Current = Current->Next)
		{
			if ((ObjectCount == Index) && (Index != 0) && Index <= count)
			{
				return Current;
			}
			ObjectCount++;
		}

		return NULL;
	}


	}