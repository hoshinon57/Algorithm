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

// https://atcoder.jp/contests/code-festival-2014-quala/tasks/code_festival_qualA_d

/*
 * 桁DPで解く。
 * 
 * (1)A以下で条件を満たす値の最大値
 * (2)A以上で条件を満たす値の最小値
 * をそれぞれ求め、Aに近い方が答。
 * 
 * (1)は以下の桁DPにて解く。
 * dp[i][sm][lz][j]:
 *   i桁目まで見て、 (i=1～(桁数))
 *   sm:0ならAと一致、1ならA未満
 *   lz:0ならleading-zeroを抜けている、1ならleading-zero中
 *   j:使用した値をbitにて管理 (j=0～1023)
 * ★iはdp,ndpのswapにより次元に持たせていない。以下その前提で記載する。
 * dp[*][0][(bitcount<=K)] のうち最大値が求める値。
 * 
 * ※leading-zeroは無くても解けたかも。
 *   考慮すべきは、例えばAが6桁で、答が5桁の99999 のようなケース。
 *   K>=1より99999は常に合法なので、このケースだけ特別に計算でいけたかもしれない。
 * 
 * (2)も同様に、smallerとgreaterを交換して解ける。
 * こちらはleading-zeroの考慮は不要。
 * ※例えばAが6桁で、答が1桁多い7桁のようなケースはありえない。
 *   これなら999999より良い解は存在しないため。
 */

int main(void)
{
	ll N; cin >> N;
	string s; s= to_string(N);
	ll K; cin >> K;
	ll ans1 = -INF64;
	ll ans2 = INF64;

// (1)A以下で条件を満たす値の最大値を求める
// よってDP値は大きい方が良い
{
	int L = (int)s.size();
	vector dp(2, vector(2, vector<ll>(1024, -INF64)));
	dp[0][1][0] = 0;  // sm=0(Aと一致), lz=1(leading-zero中), 値使用無し

	// 配るDP
	int i, sm, lz, j, d;
	for(i = 0; i < L; i++)
	{
		vector ndp(2, vector(2, vector<ll>(1024, -INF64)));
		for(sm = 0; sm < 2; sm++)
		{
			for(lz = 0; lz < 2; lz++)
			{
				for(j = 0; j < 1024; j++)
				{
					const int D = s[i] - '0';  // 配る先の値
					int ed = (sm ? 9 : D);
					for(d = 0; d <= ed; d++)  // 次に使う値
					{
						int nsm = (sm || d < D);
						int nlz = lz;
						if(d != 0) nlz = 0;

						int nj = j;
						if(nlz == 0)  // leading-zero終わってたら
						{
							nj |= (1<<d);
						}
						else nj = 0;  // leading-zero中

						chmax(ndp[nsm][nlz][nj], dp[sm][lz][j]*10+d);
					}
				}
			}
		}
		swap(dp, ndp);
	}
	// dp[*][0][bitカウント<=K] の最大値
	for(sm = 0; sm < 2; sm++)
	{
		for(j = 0; j < 1024; j++)
		{
			if(__builtin_popcount(j) > K) continue;
			chmax(ans1, dp[sm][0][j]);
		}
	}
}

// (2)A以上で条件を満たす値の最小値
// よってDP値は小さい方が良い
{
	int L =(int)s.size();
	// dp[greater][j]
	// gr:0ならAと一致、1ならAより大きい
	vector<vector<ll>> dp(2, vector<ll>(1024, INF64));
	dp[0][0] = 0;

	int i, gr, j, d;
	for(i = 0; i < L; i++)
	{
		vector<vector<ll>> ndp(2, vector<ll>(1024, INF64));
		for(gr = 0; gr < 2; gr++)
		{
			for(j = 0; j < 1024; j++)
			{
				const int D = s[i] - '0';
				// いつものsmallerとは反対
				// forのstartが変化する
				int st = (gr ? 0 : D);  // 既に大きいなら、次の数値は何でもよい(0スタート)
				for(d = st; d < 10; d++)
				{
					int ngr = (gr || d > D);  // すでにgr=1か、次に使う数値が入力Aより大きければ、ngr=1
					int nj = j;
					nj |= (1<<d);
					chmin(ndp[ngr][nj], dp[gr][j]*10+d);
				}
			}
		}
		swap(dp, ndp);
	}
	// dp[*][bitカウント<=K] の最小値
	for(gr = 0; gr < 2; gr++)
	{
		for(j = 0; j < 1024; j++)
		{
			if(__builtin_popcount(j) > K) continue;
			chmin(ans2, dp[gr][j]);
		}
	}
}

	ll tmp1 = abs(N-ans1);
	ll tmp2 = abs(N-ans2);
	cout << min(tmp1, tmp2) << endl;

	return 0;
}
