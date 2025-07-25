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

// ABC060 https://atcoder.jp/contests/abc060

/*
 * 各種解説とは全然異なる方針だった。DPで解く。
 * 
 * 基本のナップザック問題と違うのは、重さwが大きいこと。
 * DPのキーにwをそのまま乗せることができない。
 * ここで特殊な制約 w1<=wi<=w1+3 に着目すると、
 * k個選んだときの重さ総和は(w1*k)がまず確定で存在し、端数として(wi-w1)を各iについて加算することになる。
 * この端数は1つあたり最大3で、全体でもN*3と小さい。
 * 
 * ここから以下のDPを考える。
 *   i個目まで見て、
 *   j個選び、 (j=0～N)
 *   上記の端数がkであるときの (k=0～N*3)
 *   価値の最大値
 * j,kから重さの総和は復元できるので、これがW以下であるものについてDP値最大のものが答。
 * 
 * 実装ではdp,ndp形式としている。
**/

int main(void)
{
	ll i, j, k;
	ll N, W; cin >> N >> W;
	vector<ll> w(N), v(N); for(i = 0; i < N; i++) {cin >> w[i] >> v[i];}
	vector<vector<ll>> dp(N+1, vector<ll>(N*3+1, -INF64));
	dp[0][0] = 0;
	
	for(i = 0; i < N; i++)
	{
		vector<vector<ll>> ndp(N+1, vector<ll>(N*3+1));
		ndp = dp;  // i個目を使わないケース
		for(j = 0; j <= N; j++)
		{
			for(k = 0; k <= N*3; k++)  // ndp[j][k]への遷移
			{
				ll pk = k-(w[i]-w[0]);
				if(j-1 >= 0 && pk >= 0)
				{
					chmax(ndp[j][k], dp[j-1][pk] + v[i]);
				}
			}
		}
		swap(ndp, dp);
	}

	ll ans = -INF64;
	for(j = 0; j <= N; j++)
	{
		for(k = 0; k <= N*3; k++)
		{
			ll totw = j*w[0] + k;
			if(totw <= W)
			{
				chmax(ans, dp[j][k]);
			}
		}
	}
	cout << ans << endl;

	return 0;
}
