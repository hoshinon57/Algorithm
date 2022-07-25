#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// code9.2の写経

const int MAX = 100000;  // キューのサイズ
int qu[MAX];
int head, tail;  // それぞれキューの先頭、末尾を指す添え字

// キュー初期化
void init(void)
{
	head = tail = 0;
}

// キューが空かどうか
bool isEmpty(void)
{
	return (head == tail);
}

// キューが満杯かどうか
bool isFull(void)
{
	// 次に要素を入れたらqu[]の全てに要素が入る、という状態が満杯(1つは空が存在する)
	return (head == (tail+1)%MAX);
}

// キューに要素を挿入する
void enqueue(int num)
{
	if(isFull())
	{
		cout << "error: queue is Full." << endl;
		return;
	}
	qu[tail] = num;
	tail++;
	if(tail == MAX) tail = 0;  // リングバッファの終端に来た場合
}

// キューから要素を取り出す
int dequeue(void)
{
	if(isEmpty())
	{
		cout << "error: queue is Empty." << endl;
		return -1;
	}
	int res = qu[head];
	head++;
	if(head == MAX) head = 0;  // リングバッファの終端に来た場合
	return res;
}

int main(void)
{
	init();

	enqueue(3);
	enqueue(5);
	enqueue(7);
	enqueue(9);  // {3,5,7,9}

	cout << dequeue() << endl;
	cout << dequeue() << endl;  // {7,9}

	enqueue(1);
	enqueue(2);  // {7,9,1,2}

	cout << dequeue() << endl;
	cout << dequeue() << endl;
	cout << dequeue() << endl;
	cout << dequeue() << endl;
	cout << dequeue() << endl;  // error

	return 0;
}
