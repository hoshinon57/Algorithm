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

// 2025/9に再解きしたものに置き換えた。
// ミニマックス法で簡易に実装できたと思う。

int main(void)
{
	ll i;
	ll N, K; cin >> N >> K;
	vector<ll> a(K); for(i = 0; i < K; i++) {cin >> a[i];}
	vector<ll> dp(N+1, -1);

	auto f = [&](auto &self, ll n) -> ll
	{
		ll &d = dp[n];
		if(d != -1) return d;

		d = 0;
		for(ll k = 0; k < K; k++)
		{
			if(n < a[k]) continue;;
			chmax(d, n-self(self, n-a[k]));
		}
		return d;
	};
	cout << f(f, N) << endl;

	return 0;
}
