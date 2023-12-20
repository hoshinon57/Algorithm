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

// ABC181 https://atcoder.jp/contests/abc181

/*
 * 動的計画法で解く。
 * 
 * 「ペアの身長の差の合計を最小化」なので、
 * 生徒の身長でソートして先頭から2人ずつペアを作っていくのが最適になる。
 * そこに先生を1人入れる形になる。
 * よって、先生とペアを組むのは常に偶数番目の生徒となる。(0-indexed)
 * 
 * dp[i][j]を以下のように定義する。
 *   ペアをi組まで作り、  (i=0～(N+1)/2)
 *   先生をペアに j=0:組んでいない j=1:組んでいる
 *   ときの、ペアの身長の差の合計の最小値
 * 
 * 各iについて、
 *   j=0 -> j=0
 *   j=0 -> j=1
 *   j=1 -> j=1
 * それぞれの遷移を計算していく。(j=1->j=0の遷移はありえない)
 * 
 * 先生は必ずペアを組むので、dp[(N+1)/2][1]が答。
 */

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	vector<ll> h(N); for(i = 0; i < N; i++) {cin >> h[i];}
	vector<ll> w(M); for(i = 0; i < M; i++) {cin >> w[i];}
	h.push_back(INF64);  // 番兵
	sort(h.begin(), h.end());
	w.push_back(INF64);  // 番兵
	w.push_back(-INF64);
	sort(w.begin(), w.end());

	int p = (N+1)/2;  // ペアの数
	vector<vector<ll>> dp(p+1, vector<ll>(2, 0));
	for(i = 1; i <= p; i++)  // i個目のペア
	{
		// j=0->j=0の遷移
		int s1 = i*2 - 2;  // ペアの1人目
		int s2 = i*2 - 1;  // ペアの2人目
		dp[i][0] = dp[i-1][0] + (h[s2]-h[s1]);

		// j=0->j=1の遷移
		// 先生とペアを組む  組む生徒はs1
		// 生徒の身長h[s1]の上下それぞれ、最も近い先生の身長を二分探索で求める
		auto itr = lower_bound(w.begin(), w.end(), h[s1]);  // 生徒より高い側
		auto itr2 = itr; itr2--;  // 生徒より低い側
		int diff = min(abs(*itr - h[s1]), abs(h[s1] - *itr2));
		dp[i][1] = dp[i-1][0] + diff;

		// j=1->j=1
		if(i != 1)
		{
			s1--; s2--;  // 先生を使ったので、番号が1つずれる
			dp[i][1] = min(dp[i][1], dp[i-1][1] + (h[s2]-h[s1]));
		}
	}
	cout << dp[p][1] << endl;

	return 0;
}
