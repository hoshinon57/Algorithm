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

// ABC174 https://atcoder.jp/contests/abc174

/*
 * 二分探索で解く。
 * 
 * まず、問題文に「小数点以下を切り上げた値を～～」とあるが、これは整数の位置で切ってしまえば無視できる。
 * 
 * 丸太を長さx以下になるように切っていくのを考えると、xを大きくしていったときに
 *   K回より多くかかる(=NG)
 *   K回以下で終わる(=OK)
 * がどこかで切り替わる。これを二部探索で求める。
 * 
 * よって判定問題として
 *   丸太を長さx以下になるように切っていったときに、その回数はK回以下か？
 * を考えればよい。
 * 
 * 計算量は、A1～Anのうち最大値をAMaxとすると、
 *   判定問題の部分がO(N),
 *   二部探索の部分がO(log(AMax))
 * であり、全体でO(Nlog(AMax))となる。
 */

int main(void)
{
	int i;
	ll N, K; cin >> N >> K;
	ll a_max = -1;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i]; a_max = max(a_max, a[i]);}

	ll l, r;  // 切る長さで二分探索
	l = 0;  // 切る回数がK回より多くかかる側(=NG)
	r = INF64;  // 切る回数がK回以下となる側(=OK)
	r = a_max+1;
	// 丸太を長さxで切っていくときに、トータル何回切るかを返す
	auto calc = [&](ll x) -> ll
	{
		ll cnt = 0;
		for(auto &e : a)
		{
			cnt += (e-1)/x;
		}
		return cnt;
	};
	while(r - l > 1)
	{
		ll mid = (l+r)/2;
		if(calc(mid) > K)  // K回まで切れる -> K+1回はNG, K回以下はOK.
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
