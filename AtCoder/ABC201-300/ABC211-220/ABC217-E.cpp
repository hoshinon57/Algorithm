#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC217 https://atcoder.jp/contests/abc217

/*
 * multisetとqueueを用いて解く。
 * 
 * 愚直に、配列末尾に要素を追加し、クエリ3ごとに都度ソートするのは
 * 追加とソートのクエリが大量に来たときにTLEしそう。
 * 
 * よって、multisetとqueue2つのデータ構造を用意しておき、
 *   末尾に追加：queueを使う
 *   ソートする：queueの要素を全てmultisetに移動する
 *   要素出力：multiset->queryの優先度順に要素を取り出して出力する
 * とすればよい。
 * 解説にある「要素をソート済みの部分と未ソートの部分に分けて持つ」という考え方。
 */

int main(void)
{
	// ソートされたものを管理する用 set
	// 末尾に追加する用 vector
	// set->vectorの順に取り出す
	// ソートは、vectorの要素をsetに入れる
	int Q; cin >> Q;
	queue<int> list_que;
	multiset<int> list_set;
	while(Q > 0)
	{
		int query; cin >> query;
		int x;
		switch(query)
		{
		case 1:
			cin >> x;
			list_que.push(x);
			break;
		case 2:
			if(!list_set.empty())
			{
				auto itr = list_set.begin();
				cout << *itr << endl;
				list_set.erase(itr);
			}
			else
			{
				cout << list_que.front() << endl;
				list_que.pop();
			}
			break;
		case 3:
			while(!list_que.empty())
			{
				list_set.insert(list_que.front());
				list_que.pop();
			}
			break;
		}
		Q--;
	}

	return 0;
}
