#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC237 https://atcoder.jp/contests/abc237

/*
 * 双方向リストを用いて解く。
 * std::listではなく、勉強用に自前で双方向リストを作ることにした。
 * 
 * はじめ、(head) <-> 0 <-> (tail) というリストを構築しておき、
 * 0を指すポインタnowを用意する。
 * また、自分の次に要素を追加する関数insert_list()を用意する。
 * 
 * Lであれば、nowのprevに対してinsert_list()を呼び出す。
 * Rであれば、nowに対してinsert_list()を呼び出す。
 * 
 * 最後にスペース区切りで出力すればよい。
 */

// 双方向リスト
struct myList
{
	int num;
	myList *prev;
	myList *next;
};
myList head, tail;

// 値numでmyListの要素を1つ作成し、返す
myList* make_list(int num)
{
	myList *list = new(myList);
	list->num = num;
	list->prev = NULL;
	list->next = NULL;

	return list;
}

// listの次にinsertを挿入
void insert_list(myList *list, myList *insert)
{
	insert->next = list->next;
	insert->prev = list;
	list->next->prev = insert;
	list->next = insert;
}

// startの要素からnumをスペース区切りで出力
void print_list(myList *start)
{
	while(start != &tail)
	{
		cout << start->num;
		if(start->next != &tail) cout << " ";  // 次の要素があれば、スペース出力
		start = start->next;
	}
	cout << endl;
}

int main(void)
{
	int N;
	string S;
	cin >> N;
	cin >> S;

	head.prev = NULL;
	head.next = &tail;
	tail.prev = &head;
	tail.next = NULL;
	insert_list(&head, make_list(0));

	int count = 1;
	myList *now = head.next;  // 0を指す
	for(auto &e : S)
	{
		if(e == 'L')
		{
			insert_list(now->prev, make_list(count));
			now = now->prev;
		}
		else  // 'R'
		{
			insert_list(now, make_list(count));
			now = now->next;
		}
		count++;
	}

	print_list(head.next);

	return 0;
}
