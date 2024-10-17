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

// ABC375 https://atcoder.jp/contests/abc375

/*
 * 動的計画法で解く。公式解説の通り。
 *   https://atcoder.jp/contests/abc375/editorial/11140
 * 
 * 強さBの総和をbsum, 3で割った平均をbaveとする。
 * まずbsumを3で割り切れない場合は全チームの強さを同一にできないため、答は-1.
 * 以下、bsumを3で割り切れるケースについて考える。
 * 
 * ΣB<=1500より、bave<=500.
 * つまり各チームの強さの総和は500を超えることは無い。
 * ここから以下のdp[i][j][k]を考える。
 *   i人目まで見て、 (i=0～N-1)
 *   チーム1の強さの総和がj, (j=0～bave)
 *   チーム2の強さの総和がk  (k=0～bave)
 *   のときの、所属チーム変更数の最小値。
 * i人目まで見たとき、それまでのチーム1,2の強さの総和が分かればチーム3のそれも計算できるため、
 * チーム3についての情報を持たずに次元を減らせる、という点がポイント。[典型]
 * ※実装ではiの次元を消し、今回と前回のDP値のみ保持する形としている。
 * 
 * DP遷移は、人iが元々チーム1のときについて考えると、
 *   チーム1に配置：dp[i][j][k] <- dp[i-1][j-b[i]][k]   とのmin
 *   チーム2に配置：dp[i][j][k] <- dp[i-1][j][k-b[i]]+1 とのmin
 *   チーム3に配置：dp[i][j][k] <- dp[i-1][j][k]+1      とのmin
 * となる。
 * 元々チーム2,3のときも同様。
 * 
 * 答はdp[N][bave][bave]だが、
 * DP処理の結果すべてのチームの強さを同一にできないケースもあり、そこの判定は必要。(1WA)
 * 
 * [ACまでの思考の流れ]
 * ・難しい…20分以上方針が見えなかった。
 *   NとΣBが小さいのがポイントっぽい。
 * ・行き詰ったらとりあえずDP...
 *   先頭の人から見ていって、各チームの強さをDPキーに持たせるとすると、
 *   チーム3の強さ総和はDPキーに持たせる必要は無いので、次元を減らせる。
 *   これで上手くDPできそう。
 */

int main(void)
{
	int i, j, k;
	int N; cin >> N;
	vector<int> a(N), b(N);
	int bsum = 0;
	for(i = 0; i < N; i++) {
		cin >> a[i] >> b[i];
		bsum += b[i];
	}
	if(bsum%3 != 0)
	{
		cout << -1 << endl;
		return 0;
	}
	int bave = bsum/3;
	vector<vector<int>> dp(bave+1, vector<int>(bave+1, INF32));  // dp[bave+1][bave+1]
	dp[0][0] = 0;
	for(i = 0; i < N; i++)
	{
		auto ndp = dp;
		for(j = 0; j <= bave; j++)
		{
			for(k = 0; k <= bave; k++)  // ndp[j][k]を求める
			{
				ndp[j][k] = INF32;
				if(a[i] == 1)
				{
					if(j-b[i] >= 0) chmin(ndp[j][k], dp[j-b[i]][k]);  // 元チーム1 -> 1
					if(k-b[i] >= 0) chmin(ndp[j][k], dp[j][k-b[i]]+1);  // 元チーム1 -> 2
					chmin(ndp[j][k], dp[j][k]+1);  // 元チーム1 -> 3
				}
				else if(a[i] == 2)
				{
					if(j-b[i] >= 0) chmin(ndp[j][k], dp[j-b[i]][k]+1);
					if(k-b[i] >= 0) chmin(ndp[j][k], dp[j][k-b[i]]);
					chmin(ndp[j][k], dp[j][k]+1);
				}
				else
				{
					if(j-b[i] >= 0) chmin(ndp[j][k], dp[j-b[i]][k]+1);
					if(k-b[i] >= 0) chmin(ndp[j][k], dp[j][k-b[i]]+1);
					chmin(ndp[j][k], dp[j][k]);
				}
			}
		}
		swap(dp, ndp);
	}

	int ans = dp[bave][bave];
	if(ans == INF32) ans = -1;
	cout << ans << endl;

	return 0;
}
