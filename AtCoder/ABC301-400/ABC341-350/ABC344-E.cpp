#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <unordered_map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC344 https://atcoder.jp/contests/abc344

/*
 * mapで双方向リストっぽく実装して解く。
 * 
 * 要素は相異なることから、Aiの前後の要素をnxt[Ai], prev[Ai]にて持たせる。
 * Aiの上限は10^9と大きいので、連想配列を使う。
 * また先頭と末尾にそれぞれ番兵を入れておくことで、最後の出力など実装が簡易になる。
 * 
 * [備忘録]
 * 2種の番兵を-1,INFとしたとき、全く同じコードで
 *   C++20：920ms
 *   C++23：1590ms
 * と実行時間に大差が発生した。
 * 番兵に負の値を使わずINF,INF+1としたとき、
 *   C++20：920ms
 *   C++23：1060ms
 * となった。何だこれ。
 * 
 * またmapではなくunordered_mapを使ってC++23で提出した場合、
 *   番兵に負の値を使う：400ms
 * 　番兵に負の値を使わない：540ms
 * と逆転した。わけわからん。
 * 
 * とりあえずは…
 * ・要素数が多いとか、先頭から辿っていくとかの場合、unordered_mapを使うことを意識する
 * ・mapを使う場合、キーに負の値は使わないようにする
 * 辺りかな… 「C++23を使わない」というのも何か変な感じがするし。
 */

int main(void)
{
	int i;
	ll N; cin >> N;
	unordered_map<ll,ll> nxt, prev;  // nxt[i]:値iの次の値
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	for(i = 0; i < N; i++)
	{
		if(i != N-1) nxt[a[i]] = a[i+1];
		if(i != 0)   prev[a[i]] = a[i-1];
	}
	// -1 -> A -> INF (番兵)
	nxt[-1] = a[0];
	prev[a[0]] = -1;
	nxt[a[N-1]] = INF32;
	prev[INF32] = a[N-1];

	int Q; cin >> Q;
	while(Q)
	{
		Q--;
		int k; cin >> k;
		if(k == 1)
		{
			ll x, y; cin >> x >> y;
			nxt[y] = nxt[x];
			prev[y] = x;

			prev[nxt[x]] = y;
			nxt[x] = y;
		}
		else
		{
			ll x; cin >> x;
			ll pr = prev[x];
			ll nx = nxt[x];
			prev[nx] = pr;
			nxt[pr] = nx;
			// mapの要素数を抑えるため、eraseしておく
			prev.erase(x);
			nxt.erase(x);
		}
	}

	ll d = nxt[-1];
	while(true)
	{
		cout << d;
		d = nxt[d];
		if(d == INF32) break;
		cout << " ";
	}
	cout << endl;

	return 0;
}
