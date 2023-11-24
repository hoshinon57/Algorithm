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

// ABC185 https://atcoder.jp/contests/abc185

/*
 * 連続した白マスの数をx1, x2, ...とすると、
 * ・最小のものをハンコの幅kとするのが最適。
 * ・x/kの切り上げ が各xにおけるハンコを押す回数。
 */

int main(void)
{
	int i;
	ll N, M; cin >> N >> M;
	vector<ll> a(M); for(i = 0; i < M; i++) {cin >> a[i];}
	sort(a.begin(), a.end());
	a.push_back(N+1);  // 番兵

	ll base = 1;
	vector<ll> wht;  // 連続する白マスの数を順に登録
	for(i = 0; i < M+1; i++)
	{
		if(a[i]-base > 0)
		{
			wht.push_back(a[i]-base);
		}
		base = a[i]+1;
	}

	ll ans = 0;
	sort(wht.begin(), wht.end());  // wht[0]がハンコの幅kとなる
	for(auto &e : wht)
	{
		ans += (e+wht[0]-1)/wht[0];
	}
	cout << ans << endl;

	return 0;
}
