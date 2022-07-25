#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 章末問題9.1(キュー)

/*
 * キューの末尾(次に要素を入れる方)を指すポインタ tail を用意しておく。tail=nil.
 * キューの先頭はnil->nextとする。
 * 
 * enqueue:tailの次に挿入する
 * dequeue:nilの次の要素を返す
 */

struct Node {
	Node *prev, *next;
	int num;

	Node(int num_) : prev(NULL), next(NULL), num(num_) {}
};

Node *nil;  // キューのheadに相当 キューからの取り出しはnil->nextを使う
Node *tail;  // キューの末尾を指すポインタ tail->nextに次の要素を入れる

void init(void)
{
	nil = new Node(-1);
	nil->prev = nil;
	nil->next = nil;
	tail = nil;  // 要素は無いので、nilを指しておく
}

// pの直後にvを挿入
// pが省略された場合、nil(先頭)の直後に挿入する
void insert(Node *v, Node *p = nil)
{
	v->next = p->next;
	v->prev = p;
	p->next->prev = v;
	p->next = v;
}

// pを削除
void erase(Node *p)
{
	if(p == nil) return;
	p->prev->next = p->next;
	p->next->prev = p->prev;
	delete p;
}

bool isEmpty(void)
{
	return(nil == tail);
}

bool isFull(void)
{
	// とりあえずは満杯という状態は考えないので、常にfalseを返しておく	
	return false;
}

void enqueue(int num)
{
	if(isFull())
	{
		cout << "error: queue is Full." << endl;
		return;
	}
	Node *node = new Node(num);
	insert(node, tail);  // tailの次に要素挿入
	tail = tail->next;
}

int dequeue(void)
{
	if(isEmpty())
	{
		cout << "error: queue is Empty." << endl;
		return -1;
	}
	// 先頭の要素を返す
	int res = nil->next->num;
	// 要素数が1のとき(tail==nil->next)、単にerase()するとtailが無効になってしまうので、
	// 先だってtailを先頭に戻しておく
	if(tail == nil->next)  tail = nil;
	erase(nil->next);

	return res;
}

int main(void)
{
	init();

	enqueue(1);
	enqueue(3);
	enqueue(5);  // {1,3,5}

	cout << dequeue() << endl;
	cout << dequeue() << endl;  // {5}

	enqueue(10);
	enqueue(100);  // {5,10,100}

	cout << dequeue() << endl;
	cout << dequeue() << endl;   
	cout << dequeue() << endl;
	cout << dequeue() << endl;  // error

	return 0;
}
