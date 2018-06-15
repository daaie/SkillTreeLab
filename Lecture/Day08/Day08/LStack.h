#pragma once
#include "List.h"

namespace junios
{
	template<typename T>
	class LStack
	{
	protected:
		junios::List<T> Data;

	public:
		T Pop();
		void Push(T Value);
		T Peek();
		long Length() { return Data.Length(); }
	};

	template<typename T>
	T LStack<T>::Pop()
	{
		T Value = 0;

		if (Data.Length() > 0)
		{
			Value = Data.End()->Prev->Value;
			Data.Delete(Value);
		}

		return Value;
	}

	template<typename T>
	void LStack<T>::Push(T Value)
	{
		Data.PushBack(Value);
	}

	template<typename T>
	T LStack<T>::Peek()
	{
		T Value = 0;

		if (Data.Length() > 0)
		{
			Value = Data.End()->Prev->Value;
		}

		return Value;
	}
}