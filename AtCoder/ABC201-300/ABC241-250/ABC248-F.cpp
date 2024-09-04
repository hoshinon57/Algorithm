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

// ABC248 https://atcoder.jp/contests/abc248

/*
 * 自力で解けず、解説を見た。動的計画法で解く。
 * 以下解説のうち、hamayanhamayan氏とkyopro_friends氏の解説が分かりやすかった。
 *   https://blog.hamayanhamayan.com/entry/2022/04/16/234841
 *   https://x.com/kyopro_friends/status/1515327690228006916
 *   https://kyo-pro.hatenablog.com/entry/ABC248F
 *   https://qiita.com/u2dayo/items/f695270c45edbe0a2734
 * 
 * 遷移を頑張って考えるやつ。
 * dp[i][j][k]を以下のように定義する。
 *   左からi列目の頂点まで見て、 (i=1～N)
 *   辺を消した本数がj本で、 (j=0～N-1)
 *   i列目までの頂点が k=1:連結, k=0:非連結
 *   のときの辺の取り除き方の数。
 * dp[N][*][1]が答。
 * ※頂点数が2N,辺数が(3N-2)なので、連結を保てるのは辺削除が(N-1)本が限界。(限界のとき辺数=2N-1で、木となる)
 * 
 * 列ごとに"コ"の字で辺を見ていく。
 * 例えばi列目に対して _| の形で辺を2本足す(=辺を1本消す)場合、
 *   (i-1)列目まで連結： _| で辺を足せて、連結になる
 *   (i-1)列目まで非連結： _| で辺を足せない。非連結の部分を以降で連結にできないため
 * となる。
 * コの字で3本なので、2^3=8パターン、上記のような繊維を考える。
 * 詳しくは実装を参照。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・hamayanhamayan氏の解説にあり、造語だが、連結DPというのを覚えておく。
 * ・DPっぽい、というのは思いつける前提で。
 *   連結性を求められているので、i列目まで見て、連結しているかどうかを考えてみる。
 *   あとは遷移を頑張って考えてみる形か。
 */

ll dp[3005][4000][2];

int main(void)
{
	ll N, P; cin >> N >> P;
	dp[1][0][1] = dp[1][1][0] = 1;  // DP初期値 他は0

	// dp[i1][j1][k1] += dp[i1-1][j2][k2]
	// 範囲外アクセスの除外とMOD計算をしつつ加算
	auto add = [&](int i1, int j1, int k1, int j2, int k2) -> void
	{
		if(j2 < 0) return;
		dp[i1][j1][k1] += dp[i1-1][j2][k2];
		dp[i1][j1][k1] %= P;
	};
	// もらうDP
	int i, j;
	for(i = 2; i <= N; i++)
	{
		for(j = 0; j <= N+10; j++)  // 減らせるのは最大で(N-1)本だが、念のため多めに
		{
			// 3本消すのはNG

			// ￣
			// 2本消す 連結->非連結へ
			add(i, j, 0, j-2, 1);  // [i][j][0] <- [i-1][j-2][1]

			// _
			// 2本消す 連結->非連結へ
			add(i, j, 0, j-2, 1);

			//   |
			// これはNG

			//  ￣|
			// 1本消す 連結->連結へ
			add(i, j, 1, j-1, 1);

			//  _|
			// 1本消す 連結->連結へ
			add(i, j, 1, j-1, 1);

			//  _
			//  _
			// 1本消す 連結->連結 もしくは 非連結->非連結
			add(i, j, 1, j-1, 1);
			add(i, j, 0, j-1, 0);

			//  _
			//  _|
			// 0本消す  連結->連結 もしくは 非連結->連結
			add(i, j, 1, j, 1);
			add(i, j, 1, j, 0);
		}
	}

	for(i = 1; i <= N-1; i++)
	{
		cout << dp[N][i][1];
		if(i != N-1) cout << " ";
	}
	cout << endl;

	return 0;
}
