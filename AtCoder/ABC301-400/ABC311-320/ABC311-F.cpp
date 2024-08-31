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
const ll MOD = 998244353;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC311 https://atcoder.jp/contests/abc311

/*
 * 自力で解けず、解説を見た…が、公式もしっくり来るものが無かった。Google検索でもあまりヒットせず。
 * Twitterで"ABC311 F"で検索するとしっくり来るものがあり、それを元に実装した。
 *   https://x.com/myau_atcoder/status/1682768470289842178
 *   https://x.com/hotaosa/status/1682750445322903552
 *   https://x.com/tomerun/status/1682748870726680576
 *   https://asamicrayon.hatenablog.com/entry/2023/07/29/164510
 *   https://drken1215.hatenablog.com/entry/2023/07/24/182416
 * 
 * まず事前準備として、入力の#に対してその下と右下のマスを#に変換する。
 * これにより、#のマスは黒一択となる。
 * ---------
 *  ....
 *  ..#.
 *  #...
 *  ....
 * ---------
 *  ....
 *  ..#.
 *  #.##
 *  ####
 * ---------
 * 
 * 変換した図をぐっとにらむと、列ごとに見て
 * ・上から見ていったときに .と# の切り替わりが最大で1つ存在する。
 *   全部.または#のケースもある。これは範囲外の部分で切り替わっているとみなすこともできる。
 * ・ある列の.#の切り替わりが上からxマス目のとき、
 *   その右列の切り替わりは(x+1)か、それより上のマスに存在する。
 * といったことが分かる。
 * 
 * ここから以下のdp[i][j]を定義する。
 *   i列目まで見て、 (i=0～M-1)
 *   その列の#マスのてっぺんがj行目であるときの、 (j=0～N Nはその列が全て"."で、範囲外で#に切り替わっているイメージ)
 *   ※これまで行ごとに見るDPがほとんどだったので、列ごとに考えるという点が厄介だった。
 * dp[M-1][0]～dp[M-1][N]の総和が答。[N]も必要。
 * 
 * dp[i][j]への遷移を考える。これはマスとして(j,i)を指す。
 * まず、1マス上である(j-1, i)マスが'#'であれば、マス(j,i)が「#のてっぺん」にはならないため、dp[i][j]=0.
 * そうではないケースを考えると、#は右下へ進んでいくイメージとなるので、
 *   dp[i][j] = Σdp[i-1][k] (k=j-1～N)
 * となる。
 * Σを単純にO(N)で処理するとTLEなので、累積和でO(1)に高速化すればAC.
 * (DP遷移を累積和で高速化する、という典型)
 * 
 * 0列目や0行目に番兵を入れることで処理を簡易にできるが、本実装では行っていない。
 * (難しすぎてやり始めると頭が混乱しそう…となったため)
 */

int main(void)
{
	int i, j;
	int N, M; cin >> N >> M;
	vector<string> s(N); for(i = 0; i < N; i++) {cin >> s[i];}
	// 事前準備として、#の下と右下を#に置き換える
	for(i = 0; i < N-1; i++)
	{
		for(j = 0; j < M; j++)
		{
			if(s[i][j] == '#')
			{
				s[i+1][j] = '#';
				if(j+1 < M) s[i+1][j+1] = '#';
			}
		}
	}
	vector<vector<ll>> dp(M+1, vector<ll>(N+1, 0));
	// 0列目だけ先に計算
	for(i = 0; i <= N; i++)  // s[i][0]
	{
		if(i == 0) dp[0][i] = 1;
		else if(s[i-1][0] == '.') dp[0][i] = 1;
	}

	for(i = 1; i < M; i++)  // i列目
	{
		// dp[i-1][*]の累積和
		vector<ll> sdp(dp[i-1].size()+1);
		for(int k = 0; k < (int)dp[i-1].size(); k++) {sdp[k+1] = sdp[k] + dp[i-1][k]; sdp[k+1] %= MOD;}

		for(j = 0; j <= N; j++)  // j行目  dp[i][j]
		{
			// 自分の1つ上のマスが#なら0
			if(j-1 >= 0 && s[j-1][i] == '#') continue;  // 初期値0
			
			// dp[i-1][j-1]～dp[i-1][N]の和  // [j-1, N+1)
			if(j == 0) dp[i][j] = sdp[N+1];
			else dp[i][j] = (sdp[N+1] - sdp[j-1] + MOD) % MOD;
		}
	}
	ll ans = 0;
	for(i = 0; i <= N; i++)
	{
		ans += dp[M-1][i];
		ans %= MOD;
	}
	cout << ans << endl;

	return 0;
}
