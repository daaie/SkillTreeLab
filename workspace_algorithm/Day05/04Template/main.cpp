#include <stdio.h>

template<typename T> // 컴파일러가 그냥 실행파일에 쭈욱써줌 Template쓰는애들 다 찾아서 함수다씀..ㅋㅋㅋ
void swap(T *a, T*b)
{
	T temp = *a;
	*a = *b;
	*b = temp;
}

int main()
{
	int a = 1;
	int b = 2;
	swap<int>(&a, &b);

	float c = 1.1;
	float d = 2.2;
	swap<float>(&c, &d);

	printf("%d\t%d\n",a, b);
	printf("%f\t%f\n",c, d);

	return 0;
}