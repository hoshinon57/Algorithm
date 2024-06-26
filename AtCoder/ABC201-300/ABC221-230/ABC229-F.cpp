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

// ABC229 https://atcoder.jp/contests/abc229

/*
 * 円環DPで解く。
 *   https://x.com/kyopro_friends/status/1464592352069054464
 *   https://atcoder.jp/contests/abc229/editorial/2964
 *   https://kyopro-memorial.hatenadiary.jp/entry/2021/11/30/002645
 * 
 * dp[i][j]を以下のように定義する。
 *   頂点iまで見て、  (i=1～N)
 *   その頂点の色がj(0:白, 1:黒)のときの、
 *   二部グラフにするために削除する辺の重みの総和の最小値。
 * 
 * 頂点0を白と決め打ちしてよい。
 * dp[i][j]への遷移は、頂点0と頂点(i-1)から決めることができて、
 *   dp[i][0]:
 *     頂点(i-1)が白ならB(i-1)の辺を消し、黒なら消さない。
 *     ⇒dp[i-1][0]+b[i-1] と dp[i-1][1] とのmin
 *     また、頂点0との辺は常に消す。
 *   dp[i][1]:
 *     頂点(i-1)が黒ならB(i-1)の辺を消し、白なら消さない。
 *     また、頂点0との辺は常に残す。
 * と計算できる。
 * 
 * ただし円環のため、頂点1,N間の辺についてケアが必要。
 * これは頂点1を白で塗るパターンと黒で塗るパターンの2つについて、それぞれDP計算すればよい。
 * 
 * [ACまでの思考の流れ]
 * ・円環でのDPは、最初の要素を選ぶ/選ばないで場合分け。 [典型]
 *   ＞一周回ってくるので最初の情報は保持しておく必要がある。
 */

int main(void)
{
	int i;
	ll N; cin >> N;
	vector<ll> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}  // 1-indexed
	vector<ll> b(N+1); for(i = 1; i <= N; i++) {cin >> b[i];}

	// 円環なので、頂点1を白or黒決め打ちで2回DP実施
	// mode:0なら頂点1は白、1なら頂点1は黒
	auto calc = [&](int mode) -> ll
	{
		vector<vector<ll>> dp(N+1, vector<ll>(2, INF64));  // dp[N+1][2]
		// DP初期値設定
		if(mode == 0)  // 頂点1は白
		{
			dp[1][0] = a[1];  // 頂点0,1間の辺は切る
			dp[1][1] = INF64;
		}
		else  // 頂点1は黒
		{
			dp[1][0] = INF64;
			dp[1][1] = 0;
		}

		// DP遷移
		for(i = 2; i <= N; i++)
		{
			// dp[i][0]
			dp[i][0] = min(dp[i-1][0] + b[i-1], dp[i-1][1]);
			dp[i][0] += a[i];

			// dp[i][1]
			dp[i][1] = min(dp[i-1][0], dp[i-1][1] + b[i-1]);
		}

		// 頂点Nと頂点1を繋ぐ辺についてケアする
		ll ans = INF64;
		if(mode == 0)  // 頂点1は白
		{
			ans = min(dp[N][0] + b[N], dp[N][1]);
		}
		else
		{
			ans = min(dp[N][0], dp[N][1] + b[N]);
		}
		return ans;
	};

	auto t1 = calc(0);
	auto t2 = calc(1);
	cout << min(t1, t2) << endl;

	return 0;
}
