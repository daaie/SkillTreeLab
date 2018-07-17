#include <iostream>
#include <stack>

using namespace std;

int main()
{
	stack<int> binary;
	unsigned long long n = 0;
	cout << "10진수 : ";
	cin >> n;

	while (n != 0)
	{
		binary.push(n%2);
		n = n / 2;
	}

	cout << "2진수 : ";
	while (!binary.empty()) {
		cout<<binary.top();
		binary.pop();
	}
	cout << endl;

	return 0;
}