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

// 解説はそのままで、実装を2025/9に再解きしたものに置き換えた

// ABC271 https://atcoder.jp/contests/abc271

/*
 * (考え方は)動的計画法で解く。
 * 以下、0-indexedで記載する。
 * 
 * dp[i][j]を以下のように定義する。
 *   数列Eのi番目の要素までを見たときに、 (i:0～K-1)
 *   始点から都市jまでの、 (j:0～N-1)
 *   最短距離
 * (初期値の距離はINFとしておく)
 * 
 * 遷移式は、E(i)に対応する道を "a->b 距離c" としたときに、
 * b==jの場合：
 *   dp[i+1][j] = min(dp[i][j], dp[i][a]+c)
 * b!=jの場合：
 *   dp[i+1][j] = dp[i][j]
 * となる。
 * すると、dpは二次元配列にする必要はなく、添字jに相当する部分のみ持てばよい。
 * ※公式解説では「in-placeで更新可能」と書かれている
 * 
 * 以下のコードでは、一次元配列であるdist[N]を用意して実装している。
 */

int main(void)
{
	// 0-indexed
	ll i, k;
	ll N, M, K; cin >> N >> M >> K;
	vector<ll> a(M), b(M), c(M); for(i = 0; i < M; i++) {cin >> a[i] >> b[i] >> c[i]; a[i]--; b[i]--;}
	vector<ll> e(K); for(i = 0; i < K; i++) {cin >> e[i]; e[i]--; }

	vector<ll> dist(N, INF64);  // dist[i]: 頂点0⇒iへの最短距離
	dist[0] = 0;
	for(k = 0; k < K; k++)
	{
		ll aa = a[e[k]];
		ll bb = b[e[k]];  // aa->bb
		chmin(dist[bb], dist[aa]+c[e[k]]);
	}
	
	ll ans = dist[N-1];
	if(ans == INF64) ans = -1;
	cout << ans << endl;

	return 0;
}
