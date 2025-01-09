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

// AOJ 2879
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2879

/*
 * 桁DPで解く。いやー難問だが良問だった。
 * 
 * dp[i][sm][v][f]を以下のように定義する。
 *   上からi桁目まで見て、 (i=1～L  LはNの桁数)
 *   sm:未満フラグ (0:同じ, 1:未満)
 *   v:i桁目の直近3桁の値 (v=0～999)
 *   f:ごちうさ数登場フラグ (0:未登場, 1:登場済)
 *   である値の種類数。
 * (実装ではdp,ndpを用いて[i]の次元を削除している。以下その前提で書く)
 * dp[*][*][1]の総和が答。
 * 
 * よくあるDPのやつで、4桁を見たいときに直近の3桁をDPテーブルに持たせる。
 * 
 * DP遷移について、sm,vは実装を参照。
 * fの行き先nfは、基本はfを引き継ぐ。ただしvとdの計4桁がごちうさ数であれば、nf=1とする。
 */

int main(void)
{
	string s; cin >> s;
	int L = (int)s.size();

	vector dp(2, vector(1000, vector<ll>(2, 0)));
	dp[0][0][0] = 1;  // sm:一致, 直近3桁:0, ごちうさ数:未登場 のケースが1

	// 配るDP
	int i, sm, v, f;
	for(i = 0; i < L; i++)
	{
		vector ndp(2, vector(1000, vector<ll>(2, 0)));
		for(sm = 0; sm < 2; sm++)
		{
			for(v = 0; v < 1000; v++)
			{
				for(f = 0; f < 2; f++)
				{
					const int D = s[i] - '0';  // 配る先の値
					int ed = (sm ? 9 : D);
					for(int d = 0; d <= ed; d++)  // 次に使う値
					{
						int nsm = (sm || d < D);
						int nv = (v%100)*10 + d;  // v=519,d=3 なら193にしたい
						int nf = f;
						if(v/10 == 51 && d == 3) nf = 1;  // 基本はfを引継ぎ、直近の"4桁"がごちうさ数なら1に変更
						ndp[nsm][nv][nf] += dp[sm][v][f];
					}
				}
			}
		}
		swap(dp, ndp);
	}

	ll ans = 0;
	for(sm = 0; sm < 2; sm++)
	{
		for(v = 0; v < 1000; v++)
		{
			ans += dp[sm][v][1];
		}
	}
	cout << ans << endl;

	return 0;
}
