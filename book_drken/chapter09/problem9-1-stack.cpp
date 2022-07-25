#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 章末問題9.1(スタック)

/*
 * スタックの先頭を指すポインタ head を用意しておく。初期値はhead=nil.
 * push:headの次に挿入する
 * pop:headの要素を返す
 */

struct Node {
	Node *prev, *next;
	int num;

	Node(int num_) : prev(NULL), next(NULL), num(num_) {}
};

Node *nil;
Node *head;  // スタックの先頭を指すポインタ head->nextに次の要素を入れる

void init(void)
{
	nil = new Node(-1);
	nil->prev = nil;
	nil->next = nil;
	head = nil;  // 要素は無いので、nilを指しておく
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
	return(nil == head);
}

bool isFull(void)
{
	// とりあえずは満杯という状態は考えないので、常にfalseを返しておく	
	return false;
}

void push(int num)
{
	if(isFull())
	{
		cout << "error: stack is Full." << endl;
		return;
	}
	Node *node = new Node(num);
	insert(node, head);  // headの次に要素挿入
	head = head->next;
}

int pop(void)
{
	if(isEmpty())
	{
		cout << "error: stack is Empty." << endl;
		return -1;
	}
	int res = head->num;
	// 単にheadをerase()するとheadが無効になってしまうので、
	// 1つ前に戻してnextを消す、という処理にする
	head = head->prev;
	erase(head->next);
	return res;
}

int main(void)
{
	init();

	push(1);
	push(3);
	push(5);  // {1,3,5}

	cout << pop() << endl;
	cout << pop() << endl;  // {1}

	push(10);
	push(100);  // {1,10,100}

	cout << pop() << endl;
	cout << pop() << endl;   
	cout << pop() << endl;
	cout << pop() << endl;  // error

	return 0;
}
