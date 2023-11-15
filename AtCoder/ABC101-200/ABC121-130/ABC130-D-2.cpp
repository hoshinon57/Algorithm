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

// ABC130 https://atcoder.jp/contests/abc130

// 二分探索で解く版

int main(void)
{
	int i;
	ll N, K; cin >> N >> K;
	vector<ll> a(N), a_sum(N+1);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		a_sum[i+1] = a_sum[i] + a[i];
	}

	ll ans = 0;
	for(int l = 0; l < N; l++)  // 条件を満たす[l,r)を求める
	{
		int r = lower_bound(a_sum.begin(), a_sum.end(), K+a_sum[l]) - a_sum.begin();
//		if(r == N+1) break;
		ans += N+1-r;
	}
	cout << ans << endl;

	return 0;
}
