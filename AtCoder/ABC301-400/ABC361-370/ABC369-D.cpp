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

// ABC369 https://atcoder.jp/contests/abc369

/*
 * dp[i][j]を
 *   i匹目まで見て、  (i=1～N)
 *   倒した数が j=0:偶数, j=1:奇数
 *   のときの経験値
 * のように定義しつつ、dp配列がnew/oldの2つで十分なので1次元配列で実装してみた。
 * 参考：https://atcoder.jp/contests/abc369/submissions/57289721 (blueberry1001氏の提出)
 * 
 * DP遷移自体はやるだけ。
 */


int main(void)
{
	// 1-indexed
	ll i;
	ll N; cin >> N;
	vector<ll> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}
	vector<ll> dp(2, -INF64);
	dp[0] = 0;
	for(i = 1; i <= N; i++)
	{
		auto ndp = dp;
		chmax(ndp[0], dp[1] + a[i]*2);
		chmax(ndp[1], dp[0] + a[i]);
		swap(dp, ndp);
	}
	cout << *max_element(dp.begin(), dp.end()) << endl;

	return 0;
}
