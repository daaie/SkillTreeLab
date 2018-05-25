#pragma once
#include "DLList.h"
#include <stdio.h>
#include <stdlib.h>
//FILO (First in Last out) 
//array �� �ε����� ������ ���ٰ��� , ��ũ�帮��Ʈ�� ���������̾���

namespace pdaios
{
	template<typename T>
	class LStack
	{
	protected:
		pdaios::DLList<T> Data;

	public:

		T Pop(); //�����°�
		T Peek(); //��������
		void Push(T Value); // �ִ°�
		long Length() { return Data.Length(); };

	};

	template<typename T>
	T LStack<T>::Pop()
	{
		T Value = 0;
		if (Length() > 0)
		{
			Value = Data.End()->Prev->Value;
			Data.Delete(Value);
		}
		return Value;
	}

	template<typename T>
	T LStack<T>::Peek()
	{
		T Value = 0;
		if (Length() > 0)
		{
			Value = Data.End()->Prev->Value;
		}
		return Value;
	}

	template<typename T>
	void LStack<T>::Push(T Value)
	{
		Data.Push_back(Value);
	}
}