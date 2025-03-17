#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC054 https://atcoder.jp/contests/abc054

/*
 * 半分全列挙で解いた。
 * (解説を見たら、ナップザックDPでいけるじゃん…ってなった)
 * 
 * N個を前半後半に2分割することを考える。
 * 前半N/2個について選ぶ/選ばないの組み合わせは 2^20 より全探索できる。
 * mapで l[{Aの総和, Bの総和}] = 価格cの最小値
 * とすると、制約よりAの総和は10*(40/2)=200グラムが最大で、mapの要素数は4*10^4に収まる。
 * 後半もどうように求めておく。
 * 
 * 前半の各要素について、ここから{Ma*i,Mb*i}を作ることを考える。
 * 制約よりMa*iかMb*iが400を超えることは無いので、iはこれで打ち切りができる。
 * 前半の要素からあるべき後半の要素が決まるので、それが存在するかをチェック。
 * 存在するならコストの最小値を記録する。
**/

int main(void)
{
	ll i, j;
	ll N, ma, mb; cin >> N >> ma >> mb;
	vector<ll> a(N), b(N), c(N); for(i = 0; i < N; i++) {cin >> a[i] >> b[i] >> c[i];}
	map<pair<ll,ll>,ll> l, r;  // 半分全列挙の左側と右側
	// [0,N/2) と [N/2,N) で分ける  N=11なら[0,5) [5,11) の5個と6個
	ll nn = N/2;
	for(i = 0; i < (1<<nn); i++)
	{
		ll ga=0, gb=0, tc=0;
		for(j = 0; j < nn; j++)
		{
			if( ((i>>j)&1) == 0) continue;
			ga += a[j];
			gb += b[j];
			tc += c[j];
		}
		if(l.count({ga,gb}) == 0) l[{ga,gb}] = tc;
		else
		{
			chmin(l[{ga,gb}], tc);
		}
	}
	for(i = 0; i < (1<<(N-nn)); i++)
	{
		ll ga=0, gb=0, tc=0;
		for(j = 0; j < (N-nn); j++)
		{
			if( ((i>>j)&1) == 0) continue;
			ga += a[j+nn];
			gb += b[j+nn];
			tc += c[j+nn];
		}
		if(r.count({ga,gb}) == 0) r[{ga,gb}] = tc;
		else
		{
			chmin(r[{ga,gb}], tc);
		}
	}

	ll ans = INF64;
	for(auto &[k,v] : l)
	{
		auto [ga,gb] = k;
		for(i = 1; i <= 200; i++)  // {Ma*i, Mb*i}を作る
		{
			if(ma*i > 400 || mb*i > 400) break;
			ll rema = ma*i - ga;
			ll remb = mb*i - gb;
			if(r.count({rema, remb}) > 0)
			{
				chmin(ans, v + r[{rema, remb}]);
			}
		}
	}
	if(ans == INF64) ans = -1;
	cout << ans << endl;

	return 0;
}
