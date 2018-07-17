#include <iostream>
#include <string>

using namespace std;

int recursiveLength(string str)
{
	if (str[0]=='\0')
		return 0;

	str = str.substr(1);

	return 1 + recursiveLength(str);
}

int main()
{
	string str;
	cout << "문자열?";
	cin >> str;

	cout << "길이 : " << recursiveLength(str) << endl;

	return 0;
}