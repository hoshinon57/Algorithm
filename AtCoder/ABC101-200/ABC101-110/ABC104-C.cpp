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

// ABC104 https://atcoder.jp/contests/abc104

/*
 * 動的計画法で解いた。(各種解説ではbit全探索が多かった)
 * 
 * 以下、1-indexedで記載する。
 * dp[i][j]を以下のように定義する。
 *   100i点の問題集まで見て、 (i=1～D)
 *   これまで全部でj問解いたときの、 (j=0～1000)
 *   スコア最大値。
 * dp[D][j]でdp>=Gとなるもののうち、jが最小のものが答。
 * 
 * コンプリートボーナスの存在により、配るDPが立式しやすかった。
 * dp[i][j]の状態からk問解くときに、k=pかどうかでコンプリートボーナスの加算が決まる。
 * あとは実装を参照。
 * 
 * 状態数がO(D*DP), 遷移がP通りであることから、O(D^2*P^2)とかになる。
 * D=10,P=100とかなので、間に合う。
**/

int main(void)
{
	// 1-indexed
	ll i, j, k;
	ll D, G; cin >> D >> G;
	vector<ll> p(D+1), c(D+1); for(i = 1; i <= D; i++) {cin >> p[i] >> c[i];}
	const ll MX = 1000;  // 問題数MAX
	vector<vector<ll>> dp(D+1, vector<ll>(MX+1, -INF64));
	dp[0][0] = 0;
	// 配るDP
	for(i = 0; i < D; i++)
	{
		for(j = 0; j <= MX; j++)  // dp[i][j]からの遷移
		{
			for(k = 0; k <= p[i+1]; k++)  // p問解く
			{
				if(j+k > MX) break;
				ll add = (i+1)*100*k;
				if(k == p[i+1]) add += c[i+1];  // コンプボーナス
				chmax(dp[i+1][j+k], dp[i][j] + add);
			}
		}
	}

	int ans = 0;
	// dp[D][j]、dp>=Gとなる最小のj
	for(j = 0; j <= MX; j++)
	{
		if(dp[D][j] >= G)
		{
			ans = j;
			break;
		}
	}	
	cout << ans << endl;

	return 0;
}
