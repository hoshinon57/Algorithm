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
 * https://torus711.hatenablog.com/entry/20150423/1429794075
 * https://qiita.com/pinokions009/items/1e98252718eeeeb5c9ab
 * https://drken1215.hatenablog.com/entry/2019/02/04/013700
 * https://zenn.dev/ichinoseac/articles/199cf94ed49926  leading-zero
 * https://algo-logic.info/digit-dp/
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
 * 簡易な実装は
 *   solve() : leading-zero無し
 *   solve_leading_zero() : leading-zero有り
 * を参照のこと。初見では面食らう部分もあるが、慣れると分かりやすいし、かなり機械的に解けるようになった。
 * 桁DPは配るDPの方が書きやすそう。(以下のS問題解説にも言及がある https://kyopro-friends.hatenablog.com/entry/2019/01/12/231035 )
 * よってdp[i][sm][j]のように定義した場合、forループはi,sm,jにて回すべし。
 * 
 * [関連問題]
 * ABC007-D (禁止された数字)
 * ABC029-D (1)
 * ABC117-D (XXOR) 実装が面倒だった
 * ABC129-E (Sum Equals Xor)
 * ABC154-E
 * ABC155-E
 * ABC208-E (Digit Products) leading-zero
 * ABC336-E 難しかった
 * AOJ 2879 (ごちうさ数/Gochiusa-Number) 実力がつく良問 https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2879
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
	dp[0][0] = 1;  // DP初期値 (入力の先頭に'0'を追加したものをイメージすると良さげ)

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
				int nsm = (sm || d < D);  // 配る元が"未満", もしくは配り先の値がD未満なら、smaller=1.
				dp[i+1][nsm] += dp[i][sm];
			}
		}
	}
	cout << dp[L][0] + dp[L][1] << endl;
}

// [実装テンプレート]
// 以下の問題を桁DPで解く。
// [問題]1以上N以下の整数で、0が含まれるものは何個あるか
//   該当：10,909,1000など
//   非該当：005 のようにleading-zeroは対象外
//   f(100)=10, f(9999)=2619, f(10000)=2620, f(4006)=1000
void solve_leading_zero(void)
{
	string s; cin >> s;
	int L = (int)s.size();
	// dp[i][smaller][leading-zero][j]
	//   上からi桁目まで見たときに、  (i=1～L)
	//   そこまでの値が、N未満である:smaller=1, Nと等しい:smaller=0 のときに、
	//   leading-zero中なら1, leading-zeroを抜けたら0で、
	//   j=0:0を含んでいない, j=1:0を含んでいる
	//   整数の数
	// ※以下の実装では[i]を直近2要素のみ持つことで、次元を削除している
	vector dp(2, vector(2, vector<ll>(2)));

	// DP初期値は、「Nと等しく、leading-zero中」を設定する感じになりそう。
	// N=1234なら"01234"とみなすイメージ。
	// DP遷移のforループ中では存在しない特殊ケース。
	dp[0][1][0] = 1;

	// 配るDP
	// よって、forはdp[i][sm][lz][j]のi,sm,lz,jにて回すべし
	for(int i = 0; i < L; i++)
	{
		vector ndp(2, vector(2, vector<ll>(2)));
		for(int sm = 0; sm < 2; sm++)
		{
			for(int lz = 0; lz < 2; lz++)
			{
				for(int j = 0; j < 2; j++)
				{
					const int D = s[i] - '0';  // 配る先(i+1文字目)の値 indexedの関係で[i]アクセスの点に注意
					int ed = (sm ? 9 : D);  // 配る元が"未満"なら9まで、まだ一致ならDまで
					for(int d = 0; d <= ed; d++)  // 次に使う値
					{
						int nsm = (sm || d < D);  // 配る元が"未満", もしくは配り先の値がD未満なら、smaller=1.
						// nlzはlzを引き継ぐ。ただし次に使う値が0以外ならnlz=0.
						int nlz = lz;
						if(d != 0) nlz = 0;
						// njはjを引き継ぐ。ただし次に使う値が非0で、leading-zeroを抜けていたらnj=1.
						int nj = j;
						if(d == 0 && nlz == 0) nj = 1;
						
						ndp[nsm][nlz][nj] += dp[sm][lz][j];
					}
				}
			}
		}
		swap(dp, ndp);
	}
	// dp[L][*][0][1]の総和が答
	// 1以上という条件があるため、leading-zero=0.
	ll ans = 0;
	for(int sm = 0; sm < 2; sm++)
	{
		ans += dp[sm][0][1];
	}
	cout << ans << endl;
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
	solve_leading_zero();
	return 0;
}
