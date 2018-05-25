#pragma once
#include "DLList.h"
#include <stdio.h>
#include <stdlib.h>
//FILO (First in Last out) 
//array 는 인덱스로 빠르게 접근가능 , 링크드리스트는 개수제한이없음

namespace pdaios
{
	template<typename T>
	class LStack
	{
	protected:
		pdaios::DLList<T> Data;

	public:

		T Pop(); //꺼내는거
		T Peek(); //젤위에거
		void Push(T Value); // 넣는거
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