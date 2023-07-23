#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC220 https://atcoder.jp/contests/abc220

// Aの累積和を計算しておく。
// X/(Aの総和) でAを何周するかが分かる。
// その残りはupper_bound()で求まる。

int main(void)
{
	ll i;
	ll N; cin >> N;
	vector<ll> a_sum(N+1);
	for(i = 0; i < N; i++)
	{
		ll a; cin >> a;
		a_sum[i+1] = a_sum[i] + a;
	}
	ll a_total = a_sum[N];
	ll X; cin >> X;

	ll ans = 0;
	ans = (X/a_total) * N;
	X %= a_total;
	ans += upper_bound(a_sum.begin(), a_sum.end(), X) - a_sum.begin();
	cout << ans << endl;

	return 0;
}
