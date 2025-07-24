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

// ABC076 https://atcoder.jp/contests/abc076

/*
 * 前後から見る。こちらが近いかな、と。
 *   https://ikatakos.com/pot/programming_algorithm/contest_history/atcoder/2017/1028_abc076
 * 
 * dpl[i]を以下のように定義する。
 *   i番目のチェックポイント(※)における速度の最大値。
 *   (※)0秒地点, t1,t2,...,tN, の計(N+1)地点。
 * dpl[0] = 0.
 * dpl[i]は、1つ前の地点dpl[i-1]から、常に速度を1ずつ増やしていったときの速度と、
 * 今の区間の制限速度とのminとなる。
 * 
 * 一方で、ゴール地点から逆方向に、同じように計算したdp値をdprとする。
 * 各地点において、dplとdprのうち小さい方を採用する。
 * そこから移動距離を計算するのは、頑張る。
 * 自分は制限速度v[i]を満たす範囲で速度を上げていったとき、v[i]に到達するか否かで場合分けをした。
**/

int main(void)
{
	int i;
	int N; cin >> N;
	vector<double> t(N), v(N);
	for(i = 0; i < N; i++) cin >> t[i];
	for(i = 0; i < N; i++) cin >> v[i];

	auto caldp = [&](void) -> vector<double>
	{
		vector<double> dp(N+1);
		dp[0] = 0;
		for(i = 1; i <= N; i++)
		{
			dp[i] = min(dp[i-1]+t[i-1], v[i-1]);
		}
		return dp;
	};
	auto dpl = caldp();
	reverse(t.begin(), t.end());
	reverse(v.begin(), v.end());
	auto dpr = caldp();
	reverse(t.begin(), t.end());
	reverse(v.begin(), v.end());
	vector<double> dp(N+1);
	for(i = 0; i <= N; i++)
	{
		dp[i] = min(dpl[i], dpr[N-i]);
	}

	double ans = 0.0;
	for(i = 0; i < N; i++)  // 左からi番目の区間
	{
		double ky = (dp[i]+dp[i+1]+t[i])/2;  // 交点のy座標
		double tmp1, tmp2, tmp3;
		if(v[i] < ky)  // max speedに達しないケース
		{
			tmp1 = (dp[i]+v[i])*(v[i]-dp[i])/2;
			tmp2 = v[i]*(t[i]-2*v[i]+dp[i]+dp[i+1]);
			tmp3 = (dp[i+1]+v[i])*(v[i]-dp[i+1])/2;
			ans += tmp1+tmp2+tmp3;
		}
		else
		{
			tmp1 = (ky+dp[i])*(ky-dp[i])/2;
			tmp2 = (ky+dp[i+1])*(ky-dp[i+1])/2;
			ans += tmp1+tmp2;
		}
	}
	cout << std::fixed << std::setprecision(20);
	cout << ans << endl;

	return 0;
}
