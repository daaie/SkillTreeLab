#include <iostream>

using namespace std;

int main()
{
	unsigned long long number;
	unsigned long long result;
	int n;

	cout << "�??";
	cin >> n;
	
	for (int i=0;i<n;i++)
	{
		cin >> number;
		for (int a = 0; a< 64 ; a++)
		{
			unsigned long long temp = 1 << a;
			if (number <= temp)
			{
				if (i == 0)
					result = temp;
				else
					result = result^temp;
			}
		}

	}
	cout << "��� = " << result << endl;
	
	return 0;
}
