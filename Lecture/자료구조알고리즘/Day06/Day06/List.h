#pragma once

namespace junios
{
	template<typename T>
	class Node
	{
	public:
		//Node() { Value = 0; Prev = NULL; Next = NULL; }

		T Value;
		Node<T>* Prev;
		Node<T>* Next;

	};

	template<typename T>
	class List
	{
	protected:
		Node<T>* Head;
		Node<T>* Tail;

		long Count;

	public:
		List();


		void PushFront(T Value);
		void PushBack(T Value);
		Node<T>* Delete(T Value);
		Node<T>* Find(T Value);
		Node<T>* IndexOf(int Index);

		Node<T>* Begin() { return Head->Next; }
		Node<T>* End() { return Tail; }

		long Length() { return Count; }

	};

	template<typename T>
	List<T>::List()
	{
		Head = new Node<T>(); //C++ //delete
							  //Head = (Node*)malloc(sizeof(Node)); // C //free
		Tail = new Node<T>();

		Head->Next = Tail;
		Tail->Prev = Head;

		Count = 0;
	}

	template<typename T>
	void List<T>::PushBack(T Value)
	{
		Node<T>* NewNode = new Node<T>();
		NewNode->Value = Value;

		// 1. ������ ����ϱ�
		Node<T>* NodePrev = Tail->Prev;

		// 2. ���� ���� ��带 ������ ����
		NewNode->Next = Tail;
		Tail->Prev = NewNode;

		// 3. ���� ���� ��带 ���� �� ���� ����
		NewNode->Prev = NodePrev;
		NodePrev->Next = NewNode;

		Count++;
	}

	template<typename T>
	void List<T>::PushFront(T Value)
	{
		Node<T>* NewNode = new Node<T>();
		NewNode->Value = Value;

		// 1. ��� ������ ����ϱ�
		Node<T>* NodeNext = Head->Next;

		// 2. ���� ���� ��带 �Ӹ��� ����
		NewNode->Prev = Head;
		Head->Next = NewNode;

		// 3. ���� ���� ��带 �Ӹ� ���� ���� ����
		NewNode->Next = NodeNext;
		NodeNext->Prev = NewNode;

		Count++;
	}


	template<typename T>
	Node<T>* List<T>::Delete(T Value)
	{
		Node<T>* DeleteNode = Find(Value);

		if (DeleteNode)
		{
			//1. ������ ����� ���� ����
			Node<T>* ReturnNode = DeleteNode->Prev;

			//2. ���� �� ��� �յ� ����
			DeleteNode->Prev->Next = DeleteNode->Next;
			DeleteNode->Next->Prev = DeleteNode->Prev;

			//3. �޸� ����
			delete DeleteNode;

			Count--;

			//4. ������ ��� �� ��带 ����
			return ReturnNode;
		}

		return nullptr;
	}

	template<typename T>
	Node<T>* List<T>::Find(T Value)
	{
		for (Node<T>* Current = Head; Current != Tail; Current = Current->Next)
		{
			if (Current->Value == Value &&
				Current != Head &&
				Current != Tail)
			{
				return Current;
			}
		}

		return nullptr;
	}


	template<typename T>
	Node<T>* List<T>::IndexOf(int Index)
	{
		long ObjectCount = 0;
		for (Node<T>* Current = Head; Current != Tail; Current = Current->Next)
		{
			if ((ObjectCount == Index) && (Index =! 0) && (Index < Count))
			{
				return Current;
			}
			ObjectCount++;
		}

		return NULL;
	}
}