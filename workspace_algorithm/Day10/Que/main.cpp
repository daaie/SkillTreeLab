#include "AQueue.h"
#include <iostream>

using namespace std;

//------��ũ�帮��Ʈ ������ ť�� ���δ� �߾Ⱦ���. ���� �迭�� ��Ƽ� ����ų�  Queue.h
/*
int main()
{
	queue<int> q;

	q.enqueue(1);
	q.enqueue(2);
	q.enqueue(3);
	q.enqueue(4);
	q.enqueue(5);


	cout << q.isEmpty() << endl;
	cout << q.length() << endl;

	int temp;
	q.dequeue(temp);
	cout << temp << endl;

	q.dequeue(temp);
	cout << temp << endl;

	q.dequeue(temp);
	cout << temp << endl;

	q.dequeue(temp);
	cout << temp << endl;

	q.dequeue(temp);
	cout << temp << endl;

	cout << q.isEmpty() << endl;
	cout << q.length() << endl;
	return 0;
}
*/

int main()
{
	Aqueue<int> q;
	q.init(3);
	q.enqueue(1);
	q.enqueue(2);
	q.enqueue(3);

	int temp;
	q.dequeue(temp);
	q.dequeue(temp);
	q.dequeue(temp);
	q.dequeue(temp);

	q.enqueue(4);
	q.enqueue(5);
	q.enqueue(6);
	q.enqueue(7);

	return 0;
}
//flat buffers , protocol buffers 
// message