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
 * 公式解説を元に、主客転倒で解く。
 *   https://twitter.com/e869120/status/1392974101061378049
 * 
 * ある頂点vとその子頂点eを結ぶ辺について考える。
 * この辺が使われる回数は、cn[e]=(頂点eを部分木としたときの頂点数)とすると、
 *   (N-cn[e]) * cn[e]
 * となる。
 * これを各頂点についてDFSを用いて計算すればよい。
 * 
 * ※上記式、int型の範囲を超える点には注意(1WA).
 */

using Graph = vector<vector<int>>;
int N;
ll ans = 0;
vector<int> cn; // 頂点iを部分木としたときの頂点数

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

void dfs(Graph &g, int v, int p = -1)
{
	for(auto &e : g[v])
	{
		if(e == p) continue;
		// 頂点vとeを結ぶ辺が何回使われるか
		ans += (ll)cn[e] * (N-cn[e]);
		dfs(g, e, v);
	}
}

int main(void)
{
	int i;
	cin >> N;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	cn.resize(N);
	dfs_tree_subnum(g, cn, 0);
	dfs(g, 0);
	cout << ans << endl;

	return 0;
}
