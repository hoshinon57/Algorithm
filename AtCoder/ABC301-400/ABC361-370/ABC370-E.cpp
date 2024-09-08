#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC370 https://atcoder.jp/contests/abc370

/*
 * 自力で解けず、解説を見た。動的計画法で解く。
 *   https://atcoder.jp/contests/abc370/editorial/10858
 * 
 * 以下、1-indexedで記載する。
 * 
 * i番目の仕切りを、Aiの後ろ側に入れる仕切りとする。
 * また0,N番目の仕切りは必ず使うものとする。
 * dp[i]を以下のように定義する。
 *   i番目の仕切りまで見て、i番目の仕切りを使うときに合法な仕切りの入れ方の総数 (i=1～N)
 * 前述の前提とも合わせ、dp[N]が答。
 * 
 * まず愚直解法を考えてみる。
 * A1～Aiまでの総和をsum[i]とすると、  ※実装とは+1ずれている点に注意
 *   dp[i] = Σ(0<=k<i)dp[k] ただし (sum[i]-sum[k])!=K であるようなk
 * となる。
 * 区間和がKとならない部分について加算するイメージ。
 * ただしこれではO(N^2)なので、高速化を考える。
 * 
 * 前式のΣ部分を言い換えると
 *   (これまでのDP値合計) - (先頭からの総和が sum[i]-K となるようなDP値の合計)
 * となる。
 * mapにて mp[先頭からの累積和] = その総数
 * として管理すると、高速に計算できる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・コンテスト時、「以前に出た累積和の位置」だけに思考がかたより、行き詰った後にDPを思いつけなかった。
 * ・「行き詰ったらDPで考える」とか、「反対側の条件を求めて全体から引く」ではなく愚直に計算するなど、
 *   思考をリセットできるようにする練習が必要。
 * ・DPをセグメント木で高速化、累積和で高速化などの手法があるが、
 *   連想配列を使って不要なものを除外することで高速化、という手法も覚えておく。
 */

int main(void)
{
	int i;
	ll N, K; cin >> N >> K;
	vector<ll> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}
	vector<ll> a_sum(a.size()+1); for(int kk = 0; kk < (int)a.size(); kk++) {a_sum[kk+1] = a_sum[kk] + a[kk];}

	vector<ll> dp(N+1); dp[0] = 1;
	ll dptot = 1;  // これまでのDP値総和
	map<ll,ll> mp; mp[0] = 1;  // mp[先頭からの累積和] = その組み合わせ数

	for(i = 1; i <= N; i++)
	{
		// Aiまでの総和 [0,i] -> [0,i+1) -> a_sum[i+1]
		// 1-indexedだがA0=0なので同じ

		// 先頭からの総和が (Aiまでの総和 - K) となるような箇所がNGなので、
		// mapでその総数を求めて全体から引く
		dp[i] = dp[i] + dptot - mp[a_sum[i+1]-K] + MOD;
		dp[i] %= MOD;
		// mp,dptot更新
		mp[a_sum[i+1]] += dp[i];
		mp[a_sum[i+1]] %= MOD;
		dptot += dp[i];
		dptot %= MOD;
	}
	cout << dp[N] << endl;

	return 0;
}
