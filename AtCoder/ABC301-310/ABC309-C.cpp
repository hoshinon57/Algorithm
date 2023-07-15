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

// ABC309 https://atcoder.jp/contests/abc309

/*
 * 二分探索で解く。
 * ※コンテスト時はイベントソートで解いた
 *   https://atcoder.jp/contests/abc309/submissions/43355487
 * 
 * 日数の経過により飲む数は減っていくので、
 * 飲む数がどこかの日で「K錠以下」「K錠より多い」が切り替わる。
 * それを二分探索で求めればよい。
 */
int main(void)
{
	int i;
	ll N, K; cin >> N >> K;
	vector<ll> a(N), b(N); for(i = 0; i < N; i++) {cin >> a[i] >> b[i];}

	ll l, r;
	l = 0;
	r = INF32;
	// n日目の飲む数がK錠より多いならtrue
	auto check = [&](ll n) -> bool
	{
		ll count = 0;
		for(i = 0; i < N; i++)
		{
			if(n <= a[i]) count += b[i];
		}
		return count > K;
	};
	while(r - l > 1)
	{
		ll mid = (l+r)/2;
		if(check(mid))
		{
			l = mid;
		}
		else
		{
			r = mid;
		}
	}
	cout << r << endl;

	return 0;
}	
