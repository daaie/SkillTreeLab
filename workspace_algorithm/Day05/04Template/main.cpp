#include <stdio.h>

template<typename T> // �����Ϸ��� �׳� �������Ͽ� �޿���� Template���¾ֵ� �� ã�Ƽ� �Լ��پ�..������
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