#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <deque>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// https://atcoder.jp/contests/cf16-tournament-round3-open/tasks/asaporo_d

/*
 * 解説AC. スライド最小値(本問では最大値)とDPで解く。
 * ※解説ではセグメント木でも解けるような書き方だったが(そうは言ってない？)、セグ木だとTLEする。
 *   https://kmyk.github.io/blog/writeups/algo-atcoder-code-festival-2016-asapro-3-a/
 * 
 * dp[i][j]を以下のように定義する。
 *   i回当てたときに、 (i=0～K)
 *   i回目を当てたのがパネルj枚目で、 (j=1～N)
 *   そのときのスコア最大値。
 * dp[K][*]の最大値が答。
 * 
 * dp[i][j]は、max{dp[i-1][j-1]～dp[i-1][j-M]} から求められる。
 * よって連続するM要素の最大値を高速に求めたくなり、スライド最大値を用いる。
 * 
 * 詳細は実装を参照。
 * dequeには最大値そのものではなく、最大値となる要素番号を持たせている。
 */

int main(void)
{
	// 1-indexed
	ll i, j;
	ll N, M, K; cin >> N >> M >> K;
	vector<ll> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}

	vector<vector<ll>> dp(K+1, vector<ll>(N+1, -INF64));
	for(i = 0; i < N+1; i++) dp[0][i] = 0;

	for(i = 1; i <= K; i++)
	{
		// スライド最大値
		deque<ll> de;  // dp[i-1][*]の最大値 idxを持たせる
		de.push_back(0);  // 最初の番兵的な これがあるため、DP初期値としてdp[0][*]は全て0にしておく
		for(j = 1; j <= N; j++)
		{
			// dp[i-1][j-1]～dp[i-1][j-M]の最大値が欲しい

			// スコア計算
			// dequeの先頭が、dp[i-1][*]の最大値の要素番号
			ll idx = de.front();
			dp[i][j] = dp[i-1][idx] + i*a[j];

			// 次のループに備え、j-Mを削除してjを追加する

			// j-M削除
			if(j-M >= 0 && de.front() == j-M) de.pop_front();

			// jを追加
			// deque内でdp[i-1][j]よりも小さいものは、今後最大値になりえないので削除していく
			// これによりdeque内は単調減少となる
			while(de.size() > 0 && dp[i-1][de.back()] < dp[i-1][j]) de.pop_back();
			de.push_back(j);
		}
	}

	cout << *max_element(dp[K].begin(), dp[K].end()) << endl;

	return 0;
}
