#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC341 https://atcoder.jp/contests/abc341

/*
 * DPで解く。いやー添字が間違えやすい、大変だった。
 * 
 * 頂点iにコマが1個あるとき、何回操作ができるかを考えてみる。
 * ΣWy<Wxより、Wが小さい順に考えていった方が良さそう。
 * ある頂点iについて、dp[j][k]を
 *   j:頂点iと繋がっている頂点のうち、j番目の頂点 (j:1～g[i].size)
 *   k:ここまで選んだWの総和
 * とすると、ナップザックDPとなる。
 * dp[g[i].size()][Wi-1] が、「頂点iにコマが1個あるときの操作できる回数」となる。
 * 
 * Wが小さい順に求めていき、最後にAとの積を足していったものが答。
 */

using Graph = vector<vector<int>>;

int main(void)
{
	int i, j, k;
	ll N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	vector<ll> w(N); for(i = 0; i < N; i++) {cin >> w[i];}
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<int> w_idx(N);  // この順番でアクセスすると、w[]の昇順
	iota(w_idx.begin(), w_idx.end(), 0);
	sort(w_idx.begin(), w_idx.end(), [&](int x, int y){ return w[x]<w[y]; });

	vector<ll> c(N);  // c[i]:マスiにコマが1個置かれているときの、操作回数
	for(i = 0; i < N; i++)  // w_idx[i]
	{
		auto idx = w_idx[i];
		// 頂点(idx)について計算
		int gsz = (int)g[idx].size();
		vector<vector<ll>> dp(gsz+1, vector<ll>(w[idx], 0));  // dp[gsz+1][w[idx]]
		for(j = 1; j <= gsz; j++)
		{
			int v = g[idx][j-1];  // 頂点idxと繋がっている頂点
			for(k = 0; k < w[idx]; k++)
			{
				dp[j][k] = dp[j-1][k];
				if(k-w[v] >= 0)
				{
					chmax(dp[j][k], dp[j-1][k-w[v]] + c[v]);
				}
			}
		}
		c[w_idx[i]] = dp[gsz][w[idx]-1] + 1;  // +1は自分から配る分の操作
	}

	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		ans += c[i] * a[i];
	}
	cout << ans << endl;
	return 0;
}
