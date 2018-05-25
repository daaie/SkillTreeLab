#include <stdio.h>
//#include <math.h>
#include <iostream>

using namespace std;
//2^0 = 1
//2^1 = 2
//2^2 = 4
//unsigned long long pow(unsigned long long exp)
//{
//	unsigned long long result = 1;
//
//	for (int i = 0; i < (int)exp; i++)
//	{
//		result *= 2;
//	}
//
//	return result;
//}
//

// & and 
// 00 
//
// | or
// ^ xor
// ! not

int main()
{
	int N;
	cin >> N;

	unsigned long long X;
	unsigned long long result = 0;
	for (int i = 0; i < N; i++)
	{
		cin >> X;

		for (int a = 0; a < 64; ++a)
		{
			//unsigned long long temp = pow(2,a);
			unsigned long long temp = 1 << a; 

			if (X <= temp)
			{
				if (i == 0)
				{
					result = temp;
				}
				else
				{
					result ^= temp;
				}
				break;
			}
		}
	}

	printf("%llu", result);

}