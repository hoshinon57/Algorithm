#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// code8.4の写経
// 加えて、リストの末尾から追加していくケース(本で提示しているコード)と
// リストの先頭から追加していくケースをそれぞれ実装した。

struct Node {
	Node *next;
	string name;

	Node(string name_) : next(NULL), name(name_) {}
};

Node *nil;

void init(void)
{
	nil = new Node("");
	nil->next = nil;
}

// pの直後にvを挿入
// pが省略された場合、nil(先頭)の直後に挿入する
void insert(Node *v, Node *p = nil)
{
	v->next = p->next;
	p->next = v;
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

// 末尾から追加していくケース
void case1(void)
{
	vector<string> person = {"Yamamoto", "Watanabe", "Ito", "Takahashi", "Suzuki", "Sato"};
	for(int i = 0; i < person.size(); i++)
	{
		Node *node = new Node(person[i]);
		insert(node);
		cout << "step" << i+1 << ": ";
		print();
	}
}

// 先頭から追加していくケース
void case2(void)
{
	vector<string> person = {"Sato", "Suzuki", "Takahashi", "Ito", "Watanabe", "Yamamoto"};
	Node *p = nil;
	for(int i = 0; i < person.size(); i++)
	{
		Node *node = new Node(person[i]);
		insert(node, p);
		p = node;  // 次はここの直後に挿入
		cout << "step" << i+1 << ": ";
		print();
	}
}

int main(void)
{
	init();
	case1();  // 末尾から追加していくケース
	cout << "-------" << endl;
	// メモリ開放は気にせず、initやり直し
	init();
	case2();  // 先頭から追加していくケース

	return 0;
}
