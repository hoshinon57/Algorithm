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

// ABC387 https://atcoder.jp/contests/abc387

/*
 * 桁DPで解く。
 * 
 * dp[i][sm][lz][j]を以下のように定義する。
 *   i桁目まで見て、  (i=1～L)  
 *   sm: そこまでの値が、N未満である:sm=1, Nと等しい:sm=0 で、
 *   lz: leading-zero中なら1, leading-zeroを抜けたら0で、
 *   ヘビ数としての先頭数値がj (j=1～9)
 *   のときの、ヘビ数の種類数。
 * ★ただしdp,ndpのswapにすることで実際には持たせない、以下その前提で記載する
 * dp[*][0][*]の総和が答。
 * 
 * DP遷移は配るDPにて。
 * sm,lzの次はいつもと同じ。
 * jの次(nj)は以下の方針。
 * ・leading-zero中なら0
 * ・今回、leading-zeroが終わるならnj=d. (ヘビ数として先頭の値が情報として必要なので)
 * ・それ以外はjを引き継ぐ。
 *   ただしヘビ数として非合法なら処理しない。
 * 
 * 【補足】
 * コンテスト時はleading-zeroについて未履修だったこともあり、
 * 例えばfunc(1234)で呼び出したときに、内部で
 *   cal(99)+cal(999)+cal(1234)
 * としてleading-zeroの考慮が不要な実装にした。
 * 入力がlong longに収まる(10^18)ようなケースなら、場合によってはこの方針も覚えておく。
 */

ll func(ll x)
{
	/*
	「10以上の整数」という条件があるため追加した条件。
	この早期returnを有効にする場合、最下部のreturnも ans-9 とする。 
	これが無効でも、f(R)-f(L-1)にてどちらも 1～9 をヘビ数としてカウントするため、打ち消しあってACする。
	(コンテスト時は別実装だったため、これが必要だった)
	*/
//	if(x < 10) return 0;

	string s = to_string(x);
	int L = (int)s.size();

	vector dp(2, vector(2, vector<ll>(10)));
	dp[0][1][0] = 1;

	// 配るDP
	int i, sm, lz, j, d;
	for(i = 0; i < L; i++)
	{
		vector ndp(2, vector(2, vector<ll>(10)));
		for(sm = 0; sm < 2; sm++)
		{
			for(lz = 0; lz < 2; lz++)
			{
				for(j = 0; j < 10; j++)  // dp[sm][lz][j]から配る
				{
					const int D = s[i] - '0';  // 配る先の値
					int ed = (sm ? 9 : D);
					for(d = 0; d <= ed; d++)  // 次に使う値
					{
						int nsm = (sm || d < D);
						int nlz = lz;
						if(d != 0) nlz = 0;

						int nj;
						if(nlz == 1) nj = 0;  // leading-zero中なら0
						else if(lz == 1 && nlz == 0) nj = d;  // 今回leading-zeroが終わったらd
						else
						{
							// それ以外はjを引き継ぐ (前回以前にてleading-zeroが終わっているケース)
							// ただしヘビ数として合法かチェック必要
							nj = j;
							if(nj <= d) continue;  // ヘビ数違法
						}

						ndp[nsm][nlz][nj] += dp[sm][lz][j];
					}
				}
			}
		}
		swap(dp, ndp);
	}

	ll ans = 0;
	for(sm = 0; sm < 2; sm++)
	{
		for(j = 0; j < 10; j++)
		{
			ans += dp[sm][0][j];
		}
	}
	return ans;
}

int main(void)
{
	ll l, r; cin >> l >> r;
	cout << func(r) - func(l-1) << endl;

	return 0;
}
