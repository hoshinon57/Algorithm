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

// ABC334 https://atcoder.jp/contests/abc334

/*
 * コンテスト中に解けず、解説を見た。
 * 動的計画法で解く。参考：ABC181-E Transformable Teacher
 * 
 * 余った靴下について、基本は先頭から2つずつ取っていくのが最適になる。Kが偶数時はこれでいける。
 * 問題はKが奇数のとき。捨てる靴下の決め方を調べる必要がある。
 * ※ちなみに、捨てる靴下は奇数番目に固定してよい。
 * 
 * そこで、dp[i][j]を以下のように定義する。
 *   i個目のペアまで作り、  (i:1～K/2)
 *   それまでに j=0:靴下を捨てていない, j=1:捨てた
 *   のときのコスト最小値
 * 答はdp[K/2][0]とdp[K/2][1]のうち小さい方となる。
 * (最後の靴下を捨てるのが最適な場合、dp[K/2][0]が解となる)
 * 
 * 各iについて、
 *   j:0->0
 *   j:0->1
 *   j:1->1
 * の3パターンについて遷移があるのでそれぞれ計算する。
 * 
 * ちなみに上記の処理を使えば、Kが偶数時の場合も同じ処理で解ける。
 * Aの末尾にINFを追加すれば奇数になり、かつ最後の靴下を確実に捨てるため。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・要素の差分の総和を求めるような問題にて、
 *   要素を1つ削除したり1つ追加したりするような場合、
 *   ABC181-E Transformable Teacher(https://atcoder.jp/contests/abc181)を思い出すこと。
 *   自分はDPで解くのがしっくりきた。
 */

// 使わなくてよい。偶数のときは末尾にINFを加えて奇数にすることで、K=奇数と同じ処理で解ける。
int even(int N, int K, vector<int> &a)
{
	int i;
	int ans = 0;
	// 先頭から2個ずつの差の総和が答
	for(i = 0; i < K; i+=2)
	{
		ans += a[i+1]-a[i];
	}
	return ans;
}

int odd(int N, int K, vector<int> &a)
{
	int i;
	vector<vector<int>> dp(K/2+1, vector<int>(2, 0));
	for(i = 1; i <= K/2; i++)
	{
		// j:0->0
		int idx1 = i*2-2;  // ペアの1個目
		int idx2 = i*2-1;  // ペアの2個目
		dp[i][0] = dp[i-1][0] + a[idx2]-a[idx1];

		// j:0->1
		idx1++; idx2++;  // 靴下を1つ捨てるので、1つずれる
		dp[i][1] = dp[i-1][0] + a[idx2]-a[idx1];

		// j:1->1
		chmin(dp[i][1], dp[i-1][1] + a[idx2]-a[idx1]);
	}

	return min(dp[K/2][0], dp[K/2][1]);
}

int main(void)
{
	int i;
	int N, K; cin >> N >> K;
	vector<int> a(K); for(i = 0; i < K; i++) {cin >> a[i];}

#if 0  // 偶奇で処理を分けなくても、奇数に処理を統合して解ける
	int ans = 0;
	if(K%2 ==0) ans = even(N, K, a);
	else ans = odd(N, K, a);
	cout << ans << endl;
#else
	if(K%2 == 0)  // 偶数なら末尾にINFを追加して奇数扱いにする
	{
		a.push_back(INF32);
		K++;
	}
	cout << odd(N, K, a);
#endif

	return 0;
}
