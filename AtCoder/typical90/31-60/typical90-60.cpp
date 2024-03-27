#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:60 https://atcoder.jp/contests/typical90/tasks/typical90_bh

/*
 * 最長増加部分列(LIS)を両側から見て解く。公式解説とほぼ同じ実装だった。
 *   https://twitter.com/e869120/status/1402034055734980608
 * 
 * まず、前側からLISを求める。(長さは0スタートする)
 * fr[i]に、i番目の要素のLISにおける位置を保存しておく。
 * 
 * 次に後側からLISを求める。
 * bk[i]に、同様に位置を保存しておく。
 * 
 * 各iについて fr[i]+bk[i]+1 が、iを頂点としたときの、問題文の条件を満たす部分列Bの長さとなる。
 * これの最大値が答。
 * 
 * [ACまでの思考の流れ]
 * ・前後それぞれから考えたときの結果を組み合わせる[典型]
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	vector<int> fr(N), bk(N);  // fr[i]:前から見ていったときの、最長増加部分列におけるa[i]の位置
	vector<int> dp(N, INF32);
	for(i = 0; i < N; i++)
	{
		int idx = lower_bound(dp.begin(), dp.end(), a[i]) - dp.begin();
		fr[i] = idx;
		dp[idx] = a[i];
	}
	dp.assign(N, INF32);
	for(i = N-1; i >= 0; i--)
	{
		int idx = lower_bound(dp.begin(), dp.end(), a[i]) - dp.begin();
		bk[i] = idx;
		dp[idx] = a[i];
	}

	int ans = 0;
	for(i = 0; i < N; i++)
	{
		chmax(ans, fr[i]+bk[i]+1);
	}
	cout << ans << endl;

	return 0;
}
