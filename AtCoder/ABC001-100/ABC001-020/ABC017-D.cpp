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

#include <atcoder/modint>
using namespace atcoder;
using mint = modint1000000007;

// ABC017 https://atcoder.jp/contests/abc017

/*
 * 尺取り法で解く。
 * 参考：https://qiita.com/drken/items/ecd1a472d3a0e7db8dce
 * 
 * 実装の添え字が少々やかましいことになったが…愚直にDPを定義するとこうなった。
 * また、問題文のf[]は以下a[]として記載している。
 * 
 * DP[i]を以下のように定義する。
 *   i日目まで見て、i日目でいったん区切るときの、総数
 * 各a[i]について、右側に、仕切りを入れていくイメージ。
 * すると、初日が始まる前のDP値を1にしておきたいので、1-indexedにする。
 * 
 * 尺取り法は区間を[l,r)で管理したい。
 * x日目で区切るときの合法な区間最長を[l,r)とすると、x=r-1.
 * よってdp[r-1]を決めることになる。
 * 前回区切る日は (l-1)日目～(r-1-1)日目 のどこでも良いので、
 * dp[r-1] = dp[l-1]～dp[r-1-1]の総和 となる。
 * ただしこれではO(N^2)でTLEするので、ここを累積和で高速化する。
 * 上記の[l-1,r-1-1]は[l-1,r-1)になるので、
 * dp[r-1] = sum[r-1] - sum[l-1] で求められる。
 * 
 * 合法となる条件は、[l,r)に同じ味が1回以下であること。
 * 各味の登場回数をcnt[]にて管理する。
 * 
 * 以上をrについてループさせる。
 * dp[N]が答。
 */

int main(void)
{
	// 1-indexed
	int i;
	int N, M; cin >> N >> M;
	vector<int> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}
	vector<mint> dp(N+5), dp_sum(N+5);
	dp[0] = 1;
	dp_sum[1] = dp_sum[0] + dp[0];

	vector<int> cnt(M+5);  // cnt[x]:サプリxの味の登場数
	int l, r;
	l = 1;
	for(r = 2; r <= N+1; r++)  // [l,r)
	{
		cnt[a[r-1]]++;  // ここで、区間末尾の味を+1
		while(l < r && cnt[a[r-1]] > 1)  // 区間が合法になるまで、つまり末尾の味が1以下になるまでlを移動
		{
			cnt[a[l]]--;
			l++;
		}
		dp[r-1] = dp_sum[r-1] - dp_sum[l-1];  // dp[l-1]～dp[r-2]の和 -> [l-1,r-2] -> [l-1,r-1) -> sum[r-1]-sum[l-1]
		dp_sum[r-1+1] = dp_sum[r-1] + dp[r-1];
	}
	cout << dp[N].val() << endl;

	return 0;
}
