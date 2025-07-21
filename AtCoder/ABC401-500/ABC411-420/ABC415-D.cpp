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

// ABC415 https://atcoder.jp/contests/abc415

/*
 * まず、ビンにコーラが入っているか否かは関係ない。
 *   アイテム1：ビン
 *   アイテム2：シール
 * とみなして、「Ai-Biだけアイテム1を消費して、アイテム2を1つゲットする」という操作で考えても問題ない。
 * 
 * すると、(Ai-Bi)が小さい方の操作を使うのが最善、となる。
 * A,Bを(Ai-Bi)が小さい順、同じならAの小さい順(これはどちらでも良さそうだが)にソートしておく。
 * 
 * Nが大きいためシミュレーションは間に合わない。計算してこなす必要がある。
 * ソートしたA,Bを戦闘から見ていって、x=Ai-Biとすると、
 *   (N-Ai)/x + 1
 * だけこの操作を行うことができる。
**/

using pll = pair<ll,ll>;
bool mycomp(pll l, pll r)
{
	ll xl = l.first-l.second;
	ll xr = r.first-r.second;
	if(xl != xr)
	{
		return xl<xr;
	}
	else
	{
		return l.first < r.first;
	}
}

int main(void)
{
	ll i;
	ll N, M; cin >> N >> M;
	vector<pll> d(M);
	for(i = 0; i < M; i++)
	{
		cin >> d[i].first >> d[i].second;
	}
	sort(d.begin(), d.end(), mycomp);

	ll ans = 0;
	for(auto &[a,b] : d)
	{
		if(N-a < 0) continue;
		ll x = a-b;
		ll cnt = (N-a)/x + 1;
		N -= cnt*x;
		ans += cnt;
	}
	cout << ans << endl;

	return 0;
}
