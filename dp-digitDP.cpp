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

// 桁DPのメモ

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
 * [関連問題]
 * ABC007-D
 * ABC029-D
 * ABC154-E
 * Educational DP Contest S問題 https://atcoder.jp/contests/dp
 */

// 桁DPは配るDPの方が書きやすそう。
// 以下のS問題解説にも言及がある。
// https://kyopro-friends.hatenablog.com/entry/2019/01/12/231035

void solve(void)
{
	// 実装例として、以下の問題を桁DPで解く。
	// [問題]N以下の非負整数は何個あるか (N+1通りだが、桁DPで実装してみる)

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
