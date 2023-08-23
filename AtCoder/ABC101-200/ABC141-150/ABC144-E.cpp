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

// ABC144 https://atcoder.jp/contests/abc144

/*
 * 二分探索で解く。
 * 
 * Aiを降順ソート、Fiを昇順ソートすると、人と食べ物の最適な組み合わせはAi*Fiとなる。
 * (公式解説にその理由が説明されているが、解いたときはいわゆるエスパーをした)
 * 
 * 「成績x（もしくはx以下）を実現できるか」という判定問題を考える。
 * 各iについて a[i]*f[i]>x であれば修行する必要があり、最小の修行回数は a[i] - x/f[i] となるため、
 * これを加算していった総和がK以下かどうかで判断できる。
 */

int main(void)
{
	int i;
	ll N, K; cin >> N >> K;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	sort(a.begin(), a.end(), greater<ll>());
	vector<ll> f(N); for(i = 0; i < N; i++) {cin >> f[i];}
	sort(f.begin(), f.end());

	// lは成績xを実現できない側、rはできる側
	ll l = -1;
	ll r = INF64;
	// 成績x（もしくはx以下）を実現できるか
	auto check = [&](ll x) -> bool
	{
		ll sum = 0;  // 修行回数
		for(i = 0; i < N; i++)
		{
			if(a[i]*f[i] <= x) continue;  // 修行する必要無し
			sum += a[i] - x/f[i];  // 何回修行すればよいかを加算
		}
		return (sum <= K);
	};
	while(r - l > 1)
	{
		ll mid = (l+r)/2;
		if(check(mid))
		{
			r = mid;
		}
		else
		{
			l = mid;
		}
	}
	cout << r << endl;

	return 0;
}
