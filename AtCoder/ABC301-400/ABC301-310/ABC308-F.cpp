#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC308 https://atcoder.jp/contests/abc308

/*
 * 貪欲法で解く。方針が同じ解説は以下。
 *   https://twitter.com/kyopro_friends/status/1675139908531548162
 *   https://atcoder.jp/contests/abc308/editorial/6724  ※実装例もほぼ同じだった
 *   https://programming-hiroba.com/abc308-f/
 * 
 * クーポン{D,L}の順のペアについて、割引額が大きい順に見ていく。
 * このとき各クーポンについて、L以上の最も安い商品に対して使うのが最適。
 * 商品の定価をmultisetで管理し、lower_bound()で求めた。
 * 
 * lower_bound()で使える商品が見つかればansに加算し、またmultisetからは取り除く。
 * クーポンを全て見た後multisetに残っているのはクーポン未使用のものなので、そのままansに加算していく。
 * 
 * [ACまでの思考の流れ]
 * ・貪欲法というのは、コンテスト当時のツイートで知ってしまっていた。
 * ・どんな手順で貪欲していったら上手くいきそうかを考えていったとき、
 *   「クーポンは割引額が大きいものを使う方が良さそう」という観点から考えていった。
 */

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	multiset<ll> p;
	for(i = 0; i < N; i++)
	{
		ll tmp; cin >> tmp;
		p.insert(tmp);
	}
	vector<ll> l_(M); for(i = 0; i < M; i++) {cin >> l_[i];}
	vector<ll> d_(M); for(i = 0; i < M; i++) {cin >> d_[i];}
	vector<pair<ll,ll>> cp;  // {D,L}
	for(i = 0; i < M; i++)
	{
		cp.push_back({d_[i], l_[i]});
	}
	sort(cp.begin(), cp.end(), greater<pair<ll,ll>>());  // 割引額Dの大きい順に

	ll ans = 0;
	for(auto [d, l] : cp)
	{
		auto itr = p.lower_bound(l);  // 定価L円以上のなるべく安いものに対し、クーポンを使う
		if(itr == p.end()) continue;  // クーポンを使える商品が無い
		ans += *itr-d;
		p.erase(itr);
	}
	for(auto &e : p) ans += e;  // クーポン未使用の商品が残っているので加算していく
	cout << ans << endl;

	return 0;
}
