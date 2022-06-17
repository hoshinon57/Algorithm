#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC234 https://atcoder.jp/contests/abc234

/*
 * 自分は当初setを用いて解いたが、公式解説にあるpriority_queueを用いた解法も実装してみる。
 * https://atcoder.jp/contests/abc234/editorial/3222
 * 
 * キューには常に要素がK個となるようにすることで、
 * top(),pop()により、K番目に大きい値を取得できる。
 * あとはsetを用いた解法(ABC234-D.cpp)と方針は同じ。
 */
int main(void)
{
	// 1-indexed
	int i;
	int N, K;
	cin >> N >> K;
	vector<int> P(N+1);
	for(i = 1; i <= N; i++)
	{
		cin >> P[i];
	}
	priority_queue<int, vector<int>, greater<int>> p_queue;
	for(i = 1; i <= K; i++)  // まず、Pの先頭K個を入れる
	{
		p_queue.push(P[i]);
	}

	// i=Kのみ先だって処理する
	// キューにK個入っている状態で、キューの先頭＝先頭K個のうち最小のもの
	cout << p_queue.top() << endl;

	// i=K+1, K+2, ... , Nについて処理していく
	for(i = K+1; i <= N; i++)
	{
		// キューには常に要素がK個入っているようにする。
		// するとtop(),pop()により、K番目に大きい値を取得できる。
		int queue_top = p_queue.top();
		if(queue_top < P[i])
		{
			// p_queue.top<P[i]のとき、K番目に大きい値が変化する。
			// キューの先頭を取り除いてP[i]を追加することで、
			// キューの先頭が新たな「K番目に大きい値」となる。
			p_queue.pop();
			p_queue.push(P[i]);
			queue_top = p_queue.top();
		}
		else
		{
			// p_queue.top>P[i]のとき、K番目に大きい値は変化しない。
			// よって何も処理しない。
			;
		}
		cout << queue_top << endl;
	}

	return 0;
}
