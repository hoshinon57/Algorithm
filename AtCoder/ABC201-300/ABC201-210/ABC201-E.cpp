#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1000000007;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC201 https://atcoder.jp/contests/abc201

/*
 * 自力で解けず、解説を見た。
 * 
 * 頂点0を根とした木として考える。
 * 頂点i,jのLCAをa, 根をrとすると、
 *    dist(i,j)
 *  = dist(i,a) ^ dist(a,j)    ※i,jのパスはLCAを通るため
 *  = dist(i,a) ^ dist(a,r) ^ dist(r,a) ^ dist(a,j)    ※dist(a,r) ^ dist(r,a) = 0
 *  = dist(r,i) ^ dist(r,j)
 * となる。
 * xorの結果が1になるのは{0,1}のペアのみであるため、
 * dist(i,j)の総和 = (dist(r,*)が0になる頂点数) * (dist(r,*)が1になる頂点数)
 * となる。
 * 
 * xorの計算はbitごとに独立であるため、上記をbitごとに計算すればよい。
 * 前述のdist(r,*)を求めるのはDFSにて行う。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・xor計算はbitごとに独立。
 * ・木上の距離(i,j)を求める場合、根を用いた計算に置き換えられないか。
 * ・A^A=0.
 */

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll w;  // 辺の重み
	Edge(int to_, ll weight_) : to(to_), w(weight_) {}
};
using Graph = vector<vector<Edge>>;

ll cnt_1[60];  // cnt_1[i]:iビット目について、根からのxor距離が1である頂点数

// 頂点vを起点にDFS. xor距離を根から求めていく。
// p:頂点vの親
void dfs(Graph &g, int v, int p = -1, ll dist = 0)
{
	ll i;
	for(i = 0; i < 60; i++)
	{
		// bitごとに、xor距離が1になっている頂点数を更新
		if((dist>>i)&1) cnt_1[i]++;
	}

	for(auto &e : g[v])
	{
		if(e.to == p) continue;
		dfs(g, e.to, v, dist^e.w);  // xor距離を更新して再帰
	}
}

int main(void)
{
	int i;
	ll N; cin >> N;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		ll u, v, w; cin >> u >> v >> w;
		u--; v--;
		g[u].push_back(Edge(v, w));
		g[v].push_back(Edge(u, w));
	}

	dfs(g, 0);
	ll ans = 0;
	for(i = 0; i < 60; i++)
	{
		// (根からのxor距離が1である頂点数)*(0である頂点数)
		// をbitごとに加算していく
		ans += cnt_1[i]*(N-cnt_1[i])%MOD * ((1LL<<i)%MOD);
		ans %= MOD;
	}
	cout << ans << endl;

	return 0;
}
