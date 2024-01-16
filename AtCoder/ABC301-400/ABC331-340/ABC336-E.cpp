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

// ABC336 https://atcoder.jp/contests/abc336

/*
 * コンテスト中に解けず、解説を見た。
 * 桁DPで解く。
 * 
 * 桁和sを固定して考える。
 * 制約より、最大のsは9*14=136であり、s=1～136それぞれについて計算する。
 * 
 * dp[i][j][k][smaller]を以下のように定義する。
 *   i桁目までを見たときに、  (i=1～N.size)
 *   それまでの桁和がjで、 (j=0～s)
 *   それまでの値を10進数で見たときに、sで割った余りがkで、 (k=0～s-1)
 *   それまでの値が、smaller=0:Nと等しい 1:Nより小さい
 *   ときの組み合わせの数
 * これを配るDPにて実装する。
 * 
 * dp初期値はdp[0][0][0][0]=1.
 * 答はdp[N.size][s][0][0]+dp[N.size][s][0][1].
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・桁に関する条件を数え上げる⇒桁DPを考えてみる
 * ・DPにおいて必要な情報として、
 *   ・どの桁まで見ているか
 *   ・そこまでの桁和
 *   ・桁和で割り切れるかを知りたい⇒そこまでの値を桁和で割った余り
 *   ・そこまでの値がNと等しいかNより小さいか(桁DP)
 *   を区別すれば状態は網羅できそう。これをdp[]にて表現する。
 */

int main(void)
{
	string N; cin >> N;
	int Nlen = (int)N.size();
	const int kmax = 14*9;
	ll ans = 0;
	int i, j, k, h;
	for(int s = 1; s <= kmax; s++)  // 桁和
	{
		vector dp(Nlen+1, vector(s+1, vector(s, vector<ll>(2, 0))));
		dp[0][0][0][0] = 1;
		for(i = 0; i < Nlen; i++)  // i->i+1へ配るDP
		{
			int d = (int)(N[i]-'0');  // 次の桁の値
			for(j = 0; j <= s; j++)  // これまでの桁和j
			{
				for(k = 0; k < s; k++)  // これまでの値をsで割った余りk
				{
					int nj, nk;
					// smaller 0->0
					// dp[i][j][k][0] -> dp[i+1][nj][nk][0]
					nj = j+d;
					if(nj > s) {;}
					else
					{
						nk = (k*10+d)%s;
						dp[i+1][nj][nk][0] += dp[i][j][k][0];
					}
					
					// smaller 0->1
					// dp[i][j][k][0] -> dp[i+1][nj][nk][1]
					for(h = 0; h < d; h++)  // 次の桁h d未満までしか使えない
					{
						nj = j+h;
						if(nj > s) continue;
						nk = (k*10+h)%s;
						dp[i+1][nj][nk][1] += dp[i][j][k][0];
					}

					// smmaller 1->1
					// dp[i][j][k][1] -> dp[i+1][nj][nk][1]
					for(h = 0; h < 10; h++)  // 次の桁h
					{
						nj = j+h;
						if(nj > s) continue;
						nk = (k*10+h)%s;
						dp[i+1][nj][nk][1] += dp[i][j][k][1];
					}
				}
			}
		}
		ans += dp[Nlen][s][0][0] + dp[Nlen][s][0][1];
	}
	cout << ans << endl;

	return 0;
}
