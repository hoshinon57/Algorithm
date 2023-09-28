#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC214 https://atcoder.jp/contests/abc214

/*
 * イベントソートや優先度付きキューを使って解く。
 * ※ただし結構ごちゃごちゃした実装になってしまい、公式解説(YouTube)の方がずっとスマートなので…
 *   自身の記載はかなり省略して書く。
 * 
 * 以下2種のイベントを登録する。
 *   {L, 0, R}   // 0:kind(in)  L側の登録
 *   {R, 1, -1}  // 1:kind(out) R側の登録 -1はダミー
 * 
 * 各ボールについて、R側の制約が厳しい方から箱に入れていく。
 * 
 * イベント処理は以下のイメージ。
 * ----------
 * イベントが残っている間
 *   イベントから1個取り出す
 *   kind=inなら、
 *     p_queにRを入れる
 *     prev != L かつ直前にp_queが空であれば、prev=L-1
 *   L-prev個まで、p_queから取り出す
 *   prev=L
 *   kind=outなら、
 *     p_queが空ならセーフ
 *     p_queの先頭がR以前なら、箱に入れられなかったのでNG.
 * ----------
 * 
 */

bool solve(void)
{
	int i;
	int N; cin >> N;
	vector<vector<ll>> ev;
	for(i = 0; i < N; i++)
	{
		ll L, R; cin >> L >> R;
		ev.push_back({L, 0, R});
		ev.push_back({R, 1, -1});
	}
	sort(ev.begin(), ev.end());

	priority_queue<ll, vector<ll>, greater<ll>> p_que;
	ll prev = -1;
	for(i = 0; i < (int)ev.size(); i++)
	{
		ll d0, kind, d2;
		d0 = ev[i][0];
		kind = ev[i][1];
		d2 = ev[i][2];
		if(kind == 0)  // in
		{
			p_que.push(d2);  // R
			if(prev != d0 && p_que.size()==1)
			{
				prev = d0-1;
			}
		}
		
		while(!p_que.empty())
		{
			if(prev == d0) break;
			p_que.pop();
			prev++;
		}
		prev = d0;

		if(kind == 1 && !p_que.empty()) // out
		{
			if(p_que.top() <= d0) return false;
		}
	}
	return true;
}

int main(void)
{
	int T; cin >> T;
	while(T > 0)
	{
		YesNo(solve());
		T--;
	}

	return 0;
}
