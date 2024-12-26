#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <unordered_map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// AGC044 https://atcoder.jp/contests/agc044

/*
 * ABC188-Fの類題として解いた問題。
 * 値を倍々にしていくのは大変らしいので、逆方向に見ていく。つまり値xに対して
 *   コストAで/2 (ただしx%2==0)
 *   コストBで/3 (ただしx%3==0)
 *   コストCで/5 (ただしx%5==0)
 *   コストDで±1
 * を考える。
 * 
 * ここで、例えば操作B,Dを考えると、+1側のDについて
 *   D,D,D,B (+3して/3)
 * よりも
 *   B,D (/3して+1)
 * の方が、結果は同じでかつコスパが良い。
 * つまりDDDBという操作は考える必要が無い。
 * 同様にDDAやDDDDDCも無駄である。
 * ※ただし、最後にDを連打して0に持っていくケースのみ、考える必要がある
 * 
 * つまり、ある値xに対して
 *   xの前後の2の倍数まで操作Dで持って行って、操作A
 *   xの前後の3の倍数まで操作Dで持って行って、操作B
 *   xの前後の5の倍数まで操作Dで持って行って、操作C
 *   x->0まで、操作D
 * のケースに絞ることができる。
 * 実装では、これをcal()でまとめている。
 * メモ化再帰を使いながら、これらの最小値を求めていけばよい。
 * 
 * [ACまでの思考の流れ]
 * ・ABC188-Fの類題として挙げられていたので、方針はネタバレしていたようなもの。
 * ・倍々の操作は大変なので、逆方向の除算に置き換えてみる。
 */

// limitを上限として、a*bを返す
// 乗算でオーバーフローを防ぐ用途で用いる
// -> min(a*b, limit)  a,b,u>=0
// hamamuさんの以下コードをほぼそのまま持ってきている
//   https://twitter.com/hamamu_kyopro/status/1612015305110454274/
long long limitmul(long long a, long long b, long long limit)
{
	if(b == 0) return 0;  // a*b=0

	if(a <= limit / b)
		return a*b;
	else
		return limit;
}

unordered_map<ll,ll> mp;
ll A, B, C, D;

ll func(ll x)
{
	if(mp.count(x) > 0) return mp[x];
	if(x == 1) return D;
	if(x == 0) return 0;

	ll ans = INF64;
	auto cal = [&](ll m, ll cost) -> void  // mで割る操作(コスト:cost)
	{
		ll i, nxt;
		// xに対し、前後直近のmの倍数を求める
		for(i = 0; i < m; i++)
		{
			if((x-i)%m == 0)
			{
				nxt = x-i;
				chmin(ans, func(nxt/m) + cost + i*D);
				break;
			}
		}
		for(i = 1; i < m; i++)
		{
			if((x+i)%m == 0)
			{
				nxt = x+i;
				chmin(ans, func(nxt/m) + cost + i*D);
				break;
			}
		}
	};

	cal(2, A);
	cal(3, B);
	cal(5, C);
	chmin(ans, limitmul(x, D, INF64));  // x*Dはオーバーフローの可能性がある

	return (mp[x] = ans);
}

void solve(void)
{
	ll N; cin >> N >> A >> B >> C >> D;
	mp.clear();
	cout << func(N) << endl;
}

int main(void)
{
	int t; cin >> t;
	while(t > 0)
	{
		t--;
		solve();
	}

	return 0;
}
