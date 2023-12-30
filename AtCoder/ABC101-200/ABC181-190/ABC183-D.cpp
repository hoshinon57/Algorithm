#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC183 https://atcoder.jp/contests/abc183
// いもす法で解く

bool solve(void)
{
	int i;
	ll N, W; cin >> N >> W;
	vector<ll> imos(200000+5);
	for(i = 0; i < N; i++)
	{
		ll s, t, p; cin >> s >> t >> p;
		imos[s] += p;
		imos[t] -= p;
	}
	ll mx = imos[0];
	for(i = 1; i <= 200000; i++)
	{
		imos[i] += imos[i-1];
		chmax(mx, imos[i]);
	}
	return mx <= W;
}

int main(void)
{
	YesNo(solve());

	return 0;
}
