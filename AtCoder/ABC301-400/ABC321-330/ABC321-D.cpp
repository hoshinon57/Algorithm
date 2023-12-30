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

// ABC321 https://atcoder.jp/contests/abc321

// 副菜を昇順ソートし累積和を計算しておくと、各主菜aについて
// 副菜の左からある範囲はP以下で、そこから右はPが価格となる。これは二分探索で求められる。
// 前者の個数をxとすると sum[x]+a*x となり、
// 後者の個数をyとすると P*y となる。

int main(void)
{
	ll i;
	ll N, M, P; cin >> N >> M >> P;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> b(M); for(i = 0; i < M; i++) {cin >> b[i];}

	sort(b.begin(), b.end());
	vector<ll> b_sum(M+1);
	for(i = 0; i < M; i++)
	{
		b_sum[i+1] = b_sum[i] + b[i];
	}

	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		// P-a[i]がボーダー
		ll x = lower_bound(b.begin(), b.end(), P-a[i]) - b.begin();  // 上限クリップしない数
		ll y = M-x;  // 上限クリップする数
		ans += (b_sum[x] + a[i]*x) + P*y;
	}
	cout << ans << endl;

	return 0;
}
