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

// ABC160 https://atcoder.jp/contests/abc160

/*
 * 最初に考えた手法。
 * 優先度付きキューを使うが、後に解説を見たらここまで複雑に考える必要は全くなかった。
 * 公式解説を元にしたABC160-E.cppの方がずっとシンプル。
 * 供養として残しておく。
 * 
 * 赤の上位X個、緑の上位Y個以外は使わない、という点は同じ。
 * まずこれらをまとめて小さい順に優先度付きキューに入れる。
 * 一方、白をこちらは大きい順に優先度付きキューに入れる。
 * 
 * 赤緑の方のキューが空になるまで、以下の手順で1つずつりんごを確定させていく。
 *   赤緑のキューから1つ取り出す (小さい方から取り出される)
 *   白のキューの先頭の値を取得する。まだキューからは消さない
 *   ⇒白の方が美味しさが大きい場合、白の方のりんごを使う (キューから消す)
 *     そうでなければ、赤緑の方のりんごを使う
 */

int main(void)
{
	int i;
	int x, y; cin >> x >> y;
	int a, b, c; cin >> a >> b >> c;

	vector<ll> p(a); for(i = 0; i < a; i++) {cin >> p[i];}
	vector<ll> q(b); for(i = 0; i < b; i++) {cin >> q[i];}
	sort(p.begin(), p.end(), greater<ll>());  // 大きい方からx個
	sort(q.begin(), q.end(), greater<ll>());
	priority_queue<ll, vector<ll>, greater<ll>> rg_que;  // 赤トップx個、緑トップy個を、小さい順に
	for(i = 0; i < x; i++) rg_que.push(p[i]);
	for(i = 0; i < y; i++) rg_que.push(q[i]);
	
	priority_queue<ll> wht_que;  // 白を大きい順に
	for(i = 0; i < c; i++)
	{
		int tmp; cin >> tmp;
		wht_que.push(tmp);
	}

	ll ans = 0;
	while(!rg_que.empty())
	{
		ll rg = rg_que.top();
		rg_que.pop();

		ll wht = -1;
		if(!wht_que.empty())
		{
			wht = wht_que.top();
			// popはまだ
		}

		if(rg < wht)
		{
			// 白の方を使う
			ans += wht;
			wht_que.pop();
		}
		else ans += rg;
	}
	cout << ans << endl;

	return 0;
}
