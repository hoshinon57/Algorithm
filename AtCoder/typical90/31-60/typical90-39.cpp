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

// 競プロ典型90問:39 https://atcoder.jp/contests/typical90/tasks/typical90_am

/*
 * 差分に着目して解く。
 * ※公式解説とは異なる解法だった。
 * 
 * ある頂点uについて、問題文のdist(u,*)の総和をXとする。
 * uの子頂点u'に遷移したとき、Xに対してdist(u',*)がどれぐらい変化するかを考えると、
 *   u'の子頂点側：それぞれ距離が1ずつ減る
 *   上記の反対側：それぞれ距離が1ずつ増える
 * となる。
 * これは各頂点を部分木としたときの頂点数を求めておけば、O(1)で計算できる。
 * 以上をDFSで探索していく。
 * 
 * 頂点1(根)についてだけは、事前に愚直に求めておく。
 * これは各頂点の深さを求めることで計算できる。
 */

using Graph = vector<vector<int>>;

// 木であるグラフについて、頂点vを(部分木の)根としてDFS
// 各頂点vを部分木としたときの頂点数(vを含む)をn[]に設定する
// 呼び出し元からは dfs(g, sub_num, 0) のように呼び出す
// nは0で初期化しておくこと
// p:vの親
int dfs_tree_subnum(Graph &g, vector<int> &n, int v, int p = -1)
{
	n[v]++;  // 自身のぶん
	for(auto &e : g[v])
	{
		if(e == p) continue;  // 親への逆流を禁止
		n[v] += dfs_tree_subnum(g, n, e, v);
	}
	return n[v];
}

// 木であるグラフについて、頂点vを(部分木の)根としてDFS
// 各頂点の深さ(vを0とする)をd[]に設定する
// 呼び出し元からは dfs(g, depth, 0) のように呼び出す
// p:vの親
// now:頂点vの時点の根からの深さ
void dfs_tree_depth(Graph &g, vector<int> &d, int v, int p = -1, int now = 0)
{
	d[v] = now;
	for(auto &e : g[v])
	{
		if(e == p) continue;  // 親への逆流を禁止
		dfs_tree_depth(g, d, e, v, now+1);
	}
}

// 問題文のdist(v,*)の総和を求める
// sub:部分木の頂点数
void dfs(Graph &g, vector<int> &dist, vector<int> &sub, int v, int p = -1)
{
	for(auto &e : g[v])
	{
		if(e == p) continue;
		dist[e] = dist[v];
		// 頂点eを部分木とした頂点数だけ、総和は減る (=sub[e])
		// 上記部分木の反対側の頂点数だけ、総和は増える (=N-sub[e])
		dist[e] += (g.size()-sub[e]) - sub[e];
		dfs(g, dist, sub, e, v);
	}
}

int main(void)
{
	int i;
	int N; cin >> N;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	// 各頂点について、部分木の頂点数と深さを求めておく
	vector<int> subnum(N), depth(N);
	dfs_tree_subnum(g, subnum, 0);
	dfs_tree_depth(g, depth, 0);

	vector<int> dist(N);  // dist[i]:問題文のdist(i,*)の総和
	dist[0] = reduce(depth.begin(), depth.end());  // dist[0]は事前に求めておく
	dfs(g, dist, subnum, 0);
	ll ans = reduce(dist.begin(), dist.end(), 0LL);  // オーバーフローに注意(第3引数)
	cout << ans/2 << endl;

	return 0;
}
