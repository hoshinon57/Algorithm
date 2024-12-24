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

// ABC364 https://atcoder.jp/contests/abc364

/*
 * 動的計画法にて、「キーと値の入れ替えテク」を用いて解く。
 * 基本は公式解説(およびevima氏、kyopro_friends氏、snuke氏の解説)の通り。
 * 
 * dp[i][j][k]を、単純に
 *   i品目まで見て、食べた甘さの合計がj, しょっぱさの合計がkのときの、
 *   食べられる品数の最大値
 * と考えると状態数が10^10近くになるため到底間に合わない。
 * 
 * ここで、品数が甘さ、しょっぱさより小さいことから、DPのキーと値を入れ替えるテクニック[典型]を用いる。
 * dp[i][j][k]:
 *   i品目まで見て、j個食べていて、 (i,j 1～N)
 *   食べた甘さの合計がkのときの、 (k=0～X)
 *   しょっぱさの合計の最小値
 * と定義すると、状態数はN^2*Xとなり十分に間に合う。
 * 
 * DP値、つまりしょっぱさの合計がY以下であれば、まだあと1品は食べられる状態。
 * よってdp[i][j][k]<=Yであるようなjの最大値+1が答…ではなく、
 * 上限Nでクリップしたものが答。
 * 
 * [ACまでの思考の流れ]
 * ・単純に甘さとしょっぱさをキーにしたDPを思いついたが、到底間に合わない。
 * ・NとX,Yの差が大きいのが気になる。
 *   ⇒キーと値の入れ替えテクを思い出した。
 */

int main(void)
{
	// 1-indexed
	int i, j, k;
	int N, X, Y; cin >> N >> X >> Y;
	vector<int> a(N+1), b(N+1); for(i = 1; i <= N; i++) {cin >> a[i] >> b[i];}  // 1-indexed
	vector dp(N+1, vector(N+1, vector<int>(X+1, INF32)));
	dp[0][0][0] = 0;
	int ans = 0;
	for(i = 1; i <= N; i++)
	{
		for(j = 0; j <= N; j++)
		{
			for(k = 0; k <= X; k++)
			{
				int &d = dp[i][j][k];
				d = dp[i-1][j][k];  // i品目を食べない
				if(k-a[i] >= 0 && j-1 >= 0) chmin(d, dp[i-1][j-1][k-a[i]] + b[i]);

				if(d <= Y) chmax(ans, j);  // しょっぱさが制約以下なら、最大値更新
			}
		}
	}
	// DP値は「あと1品は食べられる状態」なので+1. ただし最大でN
	ans = min(ans+1, N);
	cout << ans << endl;

	return 0;
}
