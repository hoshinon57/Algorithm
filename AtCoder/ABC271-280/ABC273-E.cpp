#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ABC273 https://atcoder.jp/contests/abc273

/*
 * コンテスト中に解けず、解説を見て実装した。
 * 
 * 永続データ構造…というデータ構造を用いて解く。
 * 参考：
 *   https://atcoder.jp/contests/abc273/editorial/5059
 *   https://noshi91.hatenablog.com/entry/2019/02/04/175100
 *   https://noshi91.hatenablog.com/entry/2019/02/04/175100
 * 
 * 値と次の要素を持つNodeと、先頭の要素を指すheadを用意する。
 * すると4種の操作はそれぞれ以下のように実装できる。
 *   ADD:新たなNodeを作成し、現headの手前に追加する。新たにheadはここを指す。
 *   DELETE:headが指す要素を次へ動かす。
 *   SAVE:ページ番号とheadのアドレスの組を保存する。
 *   LOAD:ページ番号を元にheadのアドレスを再設定する。
 * 各操作ごとにheadが指す要素の値を出力すればよい。
 */

struct Node {
	int num;
	Node *next;
	Node(int num_ = -1, Node *next_ = nullptr) : num(num_), next(next_) {}
};
Node *head;

// num=nであるNodeを作成し、これをheadに設定する
void Push(int n)
{
	Node *node = new Node(n, head);  // 作成したNodeは先頭へ、つまりnextをheadに設定する
	head = node;
}

// headが指す要素を、headの次の要素に変更する
void Pop()
{
	if(head->num == -1) return;
	head = head->next;
}

int main(void)
{
	int Q;
	cin >> Q;
	map<int, Node*> notebook;

	head = new Node(-1, nullptr);
	for(int i = 0; i < Q; i++)
	{
		string que;
		cin >> que;
		if(que == "ADD")
		{
			int x;
			cin >> x;
			Push(x);
		}
		if(que == "DELETE")
		{
			Pop();
		}
		if(que == "SAVE")
		{
			int y;
			cin >> y;
			notebook[y] = head;
		}
		if(que == "LOAD")
		{
			int z;
			cin >> z;
			if(notebook.count(z) > 0)
			{
				head = notebook[z];
			}
			else
			{
				head = new Node(-1, nullptr);  // 空ページ
			}
		}

		cout << head->num;
		if(i != Q-1) cout << " ";
	}
	cout << endl;

	return 0;
}
