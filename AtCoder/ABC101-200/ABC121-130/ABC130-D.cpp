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

/*
 * 尺取り法で解く。
 * 
 * ある区間[l,r]についてK以上という条件を満たす場合、
 * rについてはr+1以降の全ての要素で条件を満たす。
 * よって尺取り法で、lを固定したときに条件を満たす最小のrを求めていく。
 * 
 * 実装では半開区間 [l,r) としている。
 * この場合、各lについて N-r+1 が条件を満たす数となる。
 * 
 * [ACまでの思考の流れ]
 * ・ある[l,r]について条件を満たすなら、以降のrについても条件を満たす。
 *   なら尺取り法だ、となった。
 */

int main(void)
{
	int i;
	ll N, K; cin >> N >> K;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	int l, r;
	ll sum = 0;
	ll ans = 0;
	for(l = 0, r = 0; l < N; l++)  // [l,r)
	{
		while(sum < K && r < N)  // r==Nになったら終了
		{
			sum += a[r];
			r++;
		}
		if(sum < K) break;  // 右端まで取っても条件を満たさないため終了
		ans += N-r+1;
		sum -= a[l];
	}
	cout << ans << endl;

	return 0;
}
