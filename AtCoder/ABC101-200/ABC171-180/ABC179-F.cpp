#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC179 https://atcoder.jp/contests/abc179

/*
 * 自力で解けず、解説を見た。
 *   https://atcoder.jp/contests/abc179/editorial/122
 *   https://drken1215.hatenablog.com/entry/2020/09/20/081100
 *   後者のけんちょん氏のと、実装もほぼ同じ。
 * 
 * 基本は解説の通りで、黒石の左上の区画 (1,1)～(R,C) を考える。
 * また tate[x]を、(1,x)に白石を置いたときにひっくり返せる個数とする。
 * 
 * (1,x)に置く場合、
 * C<xのとき：
 *   tate[x]の分だけ黒石が減る。
 * C>xのとき：
 *   R-1だけ減る。
 *   またtate[x]～tate[C]について、R-1で確定する。
 * (x,1)に置く場合も同様。
 * 
 * tate[*]を更新する回数は最大1回なので、処理は間に合う。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・tate[*],yoko[*]を管理＆更新したいが、愚直にやるとクエリあたりO(N)かかってしまう。
 * ・tate[*]が「今後更新されなくなる」というケースに着目してみる。
 */

int main(void)
{
	// 1-indexed
	ll i;
	ll N, Q; cin >> N >> Q;
	vector<ll> tate(N+1, N-2);  // tate[x]:(1,x)に置いたときにひっくり返せる数
	vector<ll> yoko(N+1, N-2);  // yoko[y]:(y,1)に同上
	ll R=N-1, C=N-1;  // (1,1)～(R,C)が左上の黒石の区画
	ll ans = (N-2)*(N-2);  // 黒石数 初期値は(N-2)^2
	while(Q > 0)
	{
		Q--;
		ll k; cin >> k;
		if(k == 1)
		{
			ll x; cin >> x;  // (1,x)
			if(C < x)
			{
				ans -= tate[x];
			}
			else
			{
				ans -= (R-1);
				for(i = x+1; i <= C; i++) tate[i] = R-1;
				C = x-1;
			}
		}
		else
		{
			ll y; cin >> y;  // (y,1)
			if(R < y)
			{
				ans -= yoko[y];
			}
			else
			{
				ans -= (C-1);
				for(i = y+1; i <= R; i++) yoko[i] = C-1;
				R = y-1;
			}
		}
	}
	cout << ans << endl;

	return 0;
}
