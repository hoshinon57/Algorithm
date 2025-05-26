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

// ABC044 https://atcoder.jp/contests/abc044

/*
 * 制約が小さいので、簡単な実装は
 * dp[i][j]:
 *   そのカードまで見たときに、
 *   i枚選んでいて、
 *   総和がjである通り数
 * でDP. O(N^3*A)とかになる。
 * これがsolve1().
 * 
 * 計算量が良い方針として、
 * ＞選んだ値の平均がA ⇒ 各値xiを(xi-A)に変換しておき、選んだ値の総和が0.
 * となるので、これでDP. O(N^2*A)とかになる。
 * これがsolve2().
 * 
 * 前者は15ms, 後者は1msとかだった。
**/

void solve1(void)
{
	ll i, j, k;
	ll N, A; cin >> N >> A;
	vector<ll> x(N); for(i = 0; i < N; i++) {cin >> x[i];}

	vector<vector<ll>> dp(N+1, vector<ll>(2500+1, 0));  // dp[N+1][2500+1]
	dp[0][0] = 1;
	for(i = 0; i < N; i++)
	{
		vector<vector<ll>> ndp = dp;  // そのカードを選ばないケース
		// 選ぶケース
		for(j = 1; j <= N; j++)
		{
			for(k = 0; k <= 2500; k++)
			{
				if(k-x[i] >= 0) ndp[j][k] += dp[j-1][k-x[i]];
			}
		}

		swap(dp, ndp);
	}

	ll ans = 0;
	for(j = 1; j <= N; j++)
	{
		ans += dp[j][j*A];
	}
	cout << ans << endl;
}

void solve2(void)
{
	ll i, j;
	ll N, A; cin >> N >> A;
	const ll geta = 2500;
	vector<ll> x(N); for(i = 0; i < N; i++) {cin >> x[i]; x[i]-=A;}
	vector<ll> dp(geta*2+1);
	dp[0+geta] = 1;
	for(i = 0; i < N; i++)
	{
		vector<ll> ndp = dp;  // 選ばないケース
		for(j = -geta; j <= geta; j++)  // 和j
		{
			if(j-x[i]+geta >= 0)
			{
				ndp[j+geta] += dp[j-x[i]+geta];
			}
		}
		swap(dp, ndp);
	}
	cout << dp[0+geta]-1 << endl;  // "0枚選ぶ"が含まれるので-1
}

int main(void)
{
//	solve1();
	solve2();

	return 0;
}
