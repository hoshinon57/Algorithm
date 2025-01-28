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

// ABC390 https://atcoder.jp/contests/abc390

/*
 * 動的計画法（と二分探索）で解く。いやーこれムズイと思うんだが水DIFF最底辺か…
 * 
 * N種の食べ物について、取得できるビタミンごとに区別して考える。
 * 制約より、食べ物の番号とカロリーで動的計画法を考える。（ビタミン量を添字に取ると間に合わない）
 * いつものdp[i][j],
 *   i番目の食べ物まで見て、
 *   取得したカロリーがj"以下"のときの、
 *   ビタミン量の最大値
 * をベースに、これをdp[3][j],
 *   xx番目のビタミンが取れる食べ物を最後まで見て、  (xx:0～2)
 *   取得したカロリーがj"以下"のときの、
 *   ビタミン量の最大値
 * として定義する。
 * 以上は実装のdp[0]～dp[2],およびcal_dp()を参照。
 * 
 * ここから解の算出に向けて、二分探索と全探索の2通りがある。
 * コンテスト時は全探索としたが、二分探索の方がビタミンの種類が増えたときにも対応できそう。
 * (1)二分探索：
 *   ビタミン量zを達成できるか、の判定問題を考える。
 *   dp[*]についてdp[*][j]>=zとなる最小のカロリーjを求め、その和がX以下であればOK.
 * (2)全探索：
 *   dp[0]からiカロリー、dp[1]からjカロリー取得するとして、i,jで全探索。
 *   dp[2]からkカロリーは X-(i+j) で求められる。
 *   dp[0][i],dp[1][j],dp[2][k] の最小値、の最大値が答。
 * 
 * [ACまでの思考の流れ]
 * ・答が最小値の最大値なので二分探索かと思ったが（実際、最後はそうなのだが）、
 *   見た目がそもそもDP.
 * ・「ビタミン1,2,3のうち摂取量が～～～」なので、ビタミンの種類ごとにわけて考えるのが良さそう。
 *   そうするとナップザックDPっぽい。
 * ・最後はビタミン1,2の取得カロリーを全探索すれば、ビタミン3の取得カロリーは一意に求まるので、
 *   その最小値の最大値を求めればよさそう。
 */

int main(void)
{
	ll i, j;
	ll N, X; cin >> N >> X;
	vector<vector<ll>> a(3), c(3);
	for(i = 0; i < N; i++)
	{
		ll v_, a_, c_; cin >> v_ >> a_ >> c_;
		v_--;
		a[v_].push_back(a_);  // ビタミン量
		c[v_].push_back(c_);  // カロリー
	}
	vector<vector<ll>> dp(3);

	auto cal_dp = [&](int id) -> vector<ll>
	{
		vector<ll> d(X+1);
		for(i = 0; i < (int)a[id].size(); i++)  // i番目の品物
		{
			vector<ll> nd = d;
			for(j = 0; j <= X; j++)  // カロリーj
			{
				ll prev = j - c[id][i];
				if(prev >= 0) chmax(nd[j], d[prev] + a[id][i]);
			}
			swap(nd, d);
		}
		return d;
	};
	dp[0] = cal_dp(0);
	dp[1] = cal_dp(1);
	dp[2] = cal_dp(2);

#if 1
	// ビタミン量zを達成できるか
	auto check = [&](ll z) -> bool
	{
		ll csum = 0;
		for(i = 0; i < 3; i++)
		{
			if(dp[i][X] < z) return false;  // ビタミン量を達成できない
			for(j = 0; j < X+1; j++)
			{
				if(dp[i][j] >= z) break;
			}
			csum += j;
		}
		return csum <= X;
	};
	ll ok = 0;
	ll ng = INF64;
	while(ng-ok > 1)
	{
		ll mid = (ok+ng)/2;
		if(check(mid)) ok = mid;
		else ng = mid;
	}
	cout << ok << endl;
#else
	// 0:カロリーi, 1:カロリーjで全探索
	ll ans = 0;
	for(i = 0; i < X+1; i++)
	{
		for(j = 0; j < X+1; j++)
		{
			ll k = X-(i+j);
			if(k < 0) continue;
			chmax(ans, min({dp[0][i], dp[1][j], dp[2][k]}));
		}
	}
	cout << ans << endl;
#endif

	return 0;
}
