#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// code9.1の写経

const int MAX = 100000;  // スタックサイズ
int st[MAX];
int top;  // スタックの先頭を指す添え字

// スタック初期化
void init(void)
{
	top = 0;
}

// スタックが空かどうか
bool isEmpty(void)
{
	return (top == 0);
}

// スタックが満杯かどうか
bool isFull(void)
{
	return (top == MAX);
}

// スタックに要素を挿入する
void push(int num)
{
	if(isFull())
	{
		cout << "error: stack is Full." << endl;
		return;
	}
	st[top] = num;
	top++;
}

// スタックから要素を取り出す
int pop(void)
{
	if(isEmpty())
	{
		cout << "error: stack is Empty." << endl;
		return -1;
	}
	top--;
	return st[top];
}

int main(void)
{
	init();

	push(3);
	push(5);
	push(7);
	push(9);  // {3,5,7,9}

	cout << pop() << endl;
	cout << pop() << endl;  // {3,5}

	push(1);
	push(2);  // {3,5,1,2}

	cout << pop() << endl;
	cout << pop() << endl;
	cout << pop() << endl;
	cout << pop() << endl;
	cout << pop() << endl;  // error

	return 0;
}
