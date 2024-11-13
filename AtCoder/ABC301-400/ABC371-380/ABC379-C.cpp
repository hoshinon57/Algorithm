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

// ABC379 https://atcoder.jp/contests/abc379

/*
 * 公式解説の方針で解いてみた。(コンテスト時は違う実装方針)
 *   https://atcoder.jp/contests/abc379/editorial/11323
 * 
 * 1-indexedで、概要は以下。
 * ・{X,A}をXでソートしておく。
 * ・ΣA!=Nなら終了。以下ΣA=Nとする。
 * ・各i(1～M)について、マス(Xi-1)までのΣAが(Xi-1)以上であることが必要。NGなら終了。
 * ・ここまで来たら条件を満たす操作が存在する。
 *   最終系を1+2+3+...+N=N(N+1)/2と考えると、
 *   N(N+1)/2 - Σ(Ai*Xi) が答。
 */

int main(void)
{
	ll i;
	ll N, M; cin >> N >> M;
	using pll = pair<ll,ll>;  // {X, A}
	vector<pll> st(M);
	for(i = 0; i < M; i++) cin >> st[i].first;
	for(i = 0; i < M; i++) cin >> st[i].second;
	sort(st.begin(), st.end());

	ll suma = 0;
	{
		for(auto [x,a] : st) suma += a;
		if(suma != N)
		{
			cout << -1 << endl;
			return 0;
		}
	}
	suma = 0;
	for(auto [x,a] : st)
	{
		if(suma < (x-1))
		{
			cout << -1 << endl;
			return 0;
		}
		suma += a;
	}

	ll ans = N*(N+1)/2;
	for(auto [x,a] : st) ans -= x*a;
	cout << ans << endl;
	
	return 0;
}
