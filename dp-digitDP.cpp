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
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 桁DPのメモ
// 桁に関する条件を数え上げる⇒桁DPを考えてみる

/*
 * [参考記事]
 * https://drken1215.hatenablog.com/entry/2019/02/04/013700
 * https://algo-logic.info/digit-dp/
 * https://qiita.com/pinokions009/items/1e98252718eeeeb5c9ab
 * https://torus711.hatenablog.com/entry/20150423/1429794075
 * 
 * 以下のようにdp[]を定義するのが良さそう。
 * dp[i][smaller][*]
 *   上からi桁目まで見たときに、
 *   そこまでの値が、N未満である:smaller=1, Nと等しい:smaller=0 のときに、
 *   何らかの条件"*"を満たすような値
 *   の種類数
 * ※多次元配列なので、vectorではなくdp[100][2][100]; のように生配列で定義した方が楽かな。
 *   またグローバル変数で定義すれば、スタックあふれを気にせず、また自動で0初期化される。
 *   
 * 以下3パターンの遷移を考える。
 * (1)smaller=1 -> smaller=1
 * (2)smaller=0 -> smaller=0
 * (3)smaller=0 -> smaller=1
 * ※smaller=1から0への遷移は存在しない
 * 
 * 遷移先のi桁目の値をdとすると、
 * (1)は遷移先にて0～9のどの値も使える。10通り。
 * (2)は遷移先にてdのみ使える。1通り。
 * (3)は遷移先にて0～(d-1)が使える。d通り。
 *    よって条件によってはd=0のときに遷移できない点に注意。
 * 
 * [実装方針]
 * 簡易な実装はsolve()を参照のこと。初見では面食らう部分もあるが、慣れると分かりやすい。
 * 桁DPは配るDPの方が書きやすそう。(以下のS問題解説にも言及がある https://kyopro-friends.hatenablog.com/entry/2019/01/12/231035 )
 * よってdp[i][sm][j]のように定義した場合、forループはi,sm,jにて回すべし。
 * 
 * [関連問題]
 * ABC007-D (禁止された数字)
 * ABC029-D (1)
 * ABC129-E
 * ABC154-E
 * ABC155-E
 * ABC336-E 難しかった
 * Educational DP Contest S問題 https://atcoder.jp/contests/dp
 * Typical DP Contest E問題 https://atcoder.jp/contests/tdpc/tasks/tdpc_number
 */

// [実装テンプレート]
// 以下の問題を桁DPで解く。
// [問題]N以下の非負整数は何個あるか (N+1通りだが、桁DPで実装してみる)
void solve(void)
{
	string s; cin >> s;
	int L = (int)s.size();
	// dp[i][smaller]
	//   上からi桁目まで見たときに、  (i=1～L)
	//   そこまでの値が、N未満である:smaller=1, Nと等しい:smaller=0 のときに、
	//   条件を満たす値の種類数
	vector<vector<ll>> dp(L+1, vector<ll>(2, 0));  // dp[L+1][2]
	dp[0][0] = 1;  // DP初期値

	// 配るDP
	// よって、forはdp[i][sm]のi,smにて回すべし
	for(int i = 0; i < L; i++)
	{
		for(int sm = 0; sm < 2; sm++)  // dp[i][sm]から配る
		{
			const int D = s[i] - '0';  // 配る先(i+1文字目)の値 indexedの関係で[i]アクセスの点に注意
			int ed = (sm ? 9 : D);  // 配る元が"未満"なら9まで、まだ一致ならDまで
			for(int d = 0; d <= ed; d++)  // 次に使う値
			{
				int nsm = (sm || d < D);  // 配る元が"未満", もしくは配り先の値がD以下なら、smaller=1.
				dp[i+1][nsm] += dp[i][sm];
			}
		}
	}
	cout << dp[L][0] + dp[L][1] << endl;
}

// 以前の書き方による実装例。
// 以下の問題を桁DPで解く。
// [問題]N以下の非負整数は何個あるか (N+1通りだが、桁DPで実装してみる)
void solve_old(void)
{
	int i;
	string s; cin >> s;
	int L = (int)s.size();
	// dp[i][smaller]
	//   上からi桁目まで見たときに、  (i=1～L)
	//   そこまでの値が、N未満である:smaller=1, Nと等しい:smaller=0 のときに、
	//   条件を満たす値の種類数
	vector<vector<ll>> dp(L+1, vector<ll>(2, 0));  // dp[L+1][2]
	dp[0][0] = 1;  // DP初期値

	// もらうDP
	for(i = 1; i <= L; i++)
	{
		int d = s[i-1]-'0';  // i文字目の値

		// smaller=1 -> smaller=1 への遷移
		dp[i][1] += dp[i-1][1] * 10;  // i桁目は0～9から自由に選べる

		// smaller=0 -> smaller=0 への遷移
		dp[i][0] += dp[i-1][0];  // 選べるのはi桁目の値のみ

		// smaller=0 -> smaller=1 への遷移
		dp[i][1] += dp[i-1][0] * d;  // i桁目は0～(d-1)のd通りから選べる

	}
	cout << dp[L][0] + dp[L][1] << endl;
}

int main(void)
{
	solve();
	return 0;
}
