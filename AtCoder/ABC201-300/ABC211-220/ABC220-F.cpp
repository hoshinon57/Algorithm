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

// ABC220 https://atcoder.jp/contests/abc220

/*
 * ある頂点aについてΣdis(a,*)を求めたとする。
 * aの子頂点bについて、差分を考えることで高速に計算することを考える。
 * a->bへ移動することで、
 *   bの部分木(b含む)のサイズをsとすると、
 *   sだけdisが減る
 *   (N-s)だけdisが増える
 * となり、O(1)でΣdis(b,*)を求めることができる。
 * 
 * よってまず頂点1について Σdis(1,*) を求めておく。
 * これは頂点1を根として各頂点の深さを加算すればよく、O(N)で求められる。
 * 
 * あとは他の頂点について前述の通り求めていけばよい。
 * depth[]をint型にしており、reduce()にて総和を求めるときにオーバーフローしてしまい1WA.
 * 
 * [ACまでの思考の流れ]
 * ・愚直にやるとO(N^2)とかになりそうで当然TLE.
 * ・ある頂点について求まったとき、その隣の頂点を簡単に求められないか(典型)。
 *   差分を考えることで高速に計算できないか。
 */

using Graph = vector<vector<int>>;
vector<int> depth;
vector<int> vnum;

// 木であるグラフについて、頂点vを(部分木の)根としてDFS
// 各頂点の深さ(vを0とする)をd[]に設定する
// 呼び出し元からは dfs(g, depth, 0) のように呼び出す
// p:vの親
// now:頂点vの時点の根からの深さ
void dfs_tree_depth(Graph &g, vector<int> &d, int v, int p = -1, ll now = 0)
{
	d[v] = now;
	for(auto &e : g[v])
	{
		if(e == p) continue;  // 親への逆流を禁止
		dfs_tree_depth(g, d, e, v, now+1);
	}
}

// 木であるグラフについて、頂点vを(部分木の)根としてDFS
// 各頂点vを部分木としたときの頂点数(vを含む)をn[]に設定する
// 呼び出し元からは dfs(g, sub_num, 0) のように呼び出す
// nは0で初期化しておくこと
// p:vの親
int dfs_tree_vnum(Graph &g, vector<int> &n, int v, int p = -1)
{
	n[v]++;  // 自身のぶん
	for(auto &e : g[v])
	{
		if(e == p) continue;  // 親への逆流を禁止
		n[v] += dfs_tree_vnum(g, n, e, v);
	}
	return n[v];
}

// Σdis(v,*)を求めていく
void dfs(Graph &g, vector<ll> &ans, int v, ll dis, int p = -1)
{
	ans[v] = dis;
	int N = g.size();
	for(auto &e : g[v])
	{
		if(e == p) continue;  // 親への逆流を禁止
		ll tmp = dis;
		tmp -= vnum[e];  // eの部分木の数だけdisは減る
		tmp +=(N-vnum[e]);  // N-(eの部分木)の数だけdisは増える
		dfs(g, ans, e, tmp, v);
	}
}

int main(void)
{
	int i;
	int N; cin >> N;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	depth.resize(N);
	vnum.resize(N);
	dfs_tree_depth(g, depth, 0);
	dfs_tree_vnum(g, vnum, 0);

	// ans[0]を元に他を差分計算していく
	vector<ll> ans(N);
	ll dis = reduce(depth.begin(), depth.end(), 0LL);  // int型だとオーバーフローするので第3引数にLL指定
	dfs(g, ans, 0, dis);
	for(auto &e : ans) cout << e << endl;

	return 0;
}
