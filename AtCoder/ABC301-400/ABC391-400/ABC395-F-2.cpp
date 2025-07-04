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

// ABC395 https://atcoder.jp/contests/abc395

/*
 * 公式解説の二分探索。
 *   https://atcoder.jp/contests/abc395/editorial/12344
 *   https://atcoder.jp/contests/abc395/submissions/63314314 snuke氏の実装
 * 
 * あるHで条件を満たせたとすると、(H-1)でも満たせる。
 *   (微妙にピンと来なかったが、Hで条件を満たすなら、下の歯を1ずつ削り、下の歯が削れなければ上の歯を削る、
 *    という方針で構築できるらしい)
 * 逆にHで満たせない場合、(H+1)でも満たせない。
 * ということで二分探索。
 * 
 * hで条件を満たすか、の判定問題を考える。
 * u[i],d[i],hがあったとき、
 *   u[i]の最小値：h-d[i] (下限0)
 *   u[i]の最大値：u[i] (上限h)
 * の範囲となる。これを[nl,nr]と表現する。
 * 1つ前の区間を[bl,br]とすると、差がX以下という制約があるので、
 *   nl = max(nl, bl-X)
 *   nr = min(nr, br+X)
 * となる。これが空区間ならNG.
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・二分探索は見えた、判定問題が見えなかった。
 * ・ある要素についてOKとなる区間があり、隣の要素との制約もある場合、2つの区間の共通部分を求める問題に帰着できる。
 *   2つの閉区間の共通部分は、L=max(l1,l2), R=min(r1,r2) として [L,R] と表せる。L<=Rならば共通部分を持つ。 [典型]
**/

int main(void)
{
	ll i;
	ll N, X; cin >> N >> X;
	vector<ll> u(N), d(N); for(i = 0; i < N; i++) {cin >> u[i] >> d[i];}

	// u+d=hが成立するか
	auto chk = [&](ll h) -> bool
	{
		ll bl = max(h-d[0], 0LL);
		ll br = min(u[0], h);
		if(bl > br) return false;  // [bl,br]が許容範囲

		for(i = 1; i < N; i++)
		{
			ll nl = max(h-d[i], 0LL);
			ll nr = min(u[i], h);
			chmax(nl, bl-X);
			chmin(nr, br+X);
			// [nl,nr]が許容範囲
			if(nl > nr) return false;
			bl = nl;
			br = nr;
		}
		return true;
	};

	ll ok = 0;
	ll ng = 1e10;
	while(ng - ok > 1)
	{
		ll mid = (ok+ng)/2;
		if(chk(mid)) ok = mid;
		else ng = mid;
	}

	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		ans += u[i]+d[i]-ok;
	}
	cout << ans << endl;

	return 0;
}
