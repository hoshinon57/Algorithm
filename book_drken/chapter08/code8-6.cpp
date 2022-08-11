#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// code8.6の写経

struct Node {
	Node *prev, *next;
	string name;

	Node(string name_) : prev(NULL), next(NULL), name(name_) {}
};

Node *nil;

void init(void)
{
	nil = new Node("");
	nil->prev = nil;
	nil->next = nil;
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

void print()
{
	Node *p;
	for(p = nil->next; p != nil; p=p->next)
	{
		cout << p->name << " -> ";
	}
	cout << endl;
}

int main(void)
{
	vector<string> person = {"Yamamoto", "Watanabe", "Ito", "Takahashi", "Suzuki", "Sato"};

	init();
	Node *node_watanabe = nil;
	for(int i = 0; i < person.size(); i++)
	{
		Node *node = new Node(person[i]);
		insert(node);
		if(person[i] == "Watanabe")  // Watanabeを削除するので、ポインタを保持しておく
		{
			node_watanabe = node;
		}
	}
	print();

	erase(node_watanabe);
	print();

	return 0;
}
