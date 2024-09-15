#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <functional>  // function
#include <numeric>
using namespace std;
typedef long long ll;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 全方位木DPのメモやライブラリ
// ★注意★ #include <functional> を忘れずに。ローカル環境では無くてもビルドが通るが、AtCoderではCEになる。

// memo/todo:
// [済]木の直径を解いてみる
// EDPC-V
// [済]ABC222-F
// 2024/6ごろABC-Eの、直径で解く問題を全方位木DPで

/*
 * [ざっくり概要]
 * ToDo:書く
 * 
 * ★代表的なmerge,add_root等はmain()に記述している。
 * 
 * [メソッド等説明]
 * ・T : DPの型 基本はint,llなどで、上位2つの値を保持するならpair<int,int>など
 * ・identity : 単位元的なやつ、らしい
 * ・Edge : 辺の型 基本はto,wで良いと思われる
 * 
 * ・merge(x1,x2) : 2つのDP値に対する二項演算
 * ・add_root(x) : DP値に根を追加する
 * ・Rerooting(N,idnt) : 頂点数N,単位元idntで初期化
 * ・add_edge(v,e) : 頂点vに辺eを追加
 * ・dfs1(v) : DFS1回目 まずはvを根として通常の木DPを行う
 * ・dfs2() : DFS2回目 全方位木DPを行う
 * ・build() : DFS1,2回目を実施
 * 
 * [参考資料]
 *   https://algo-logic.info/tree-dp/
 *   https://trap.jp/post/1161/
 *   https://drken1215.hatenablog.com/entry/2019/12/26/225500
 *   https://ei1333.hateblo.jp/entry/2018/12/21/004022
 * 
 * [関連する問題 / verifyした問題]
 * ・典型90-003 https://atcoder.jp/contests/typical90/tasks/typical90_c 木の直径
 */

vector<ll> d;  // ABC222-F用

// 全方位木DPのライブラリ
// [事前準備]
//   T:DPの型
//   先頭のmerge,add_root,Edgeを問題に応じて書き換える。
//   dfs2()でのans[]への設定を、問題に応じて書き換える。
// [使い方]
//   (1)make_edge()でグラフを構築
//   (2)build()で全方位木DPを計算
//   (3)ans[]を出力
// 以下URLをほぼそのまま持ってきている
// https://algo-logic.info/tree-dp/
template <typename T>
struct Rerooting {
#if 1  // 問題ごとに書き換え
	function<T(T,T)> merge = [](T x1, T x2) -> T {
		return T(max(x1, x2));
	};
	function<T(T,int)> add_root = [](T x, int v) -> T {
//		return T(x + 1);
		// return T(x);
		return max(x, T(d[v]));
	};
	struct Edge {
		int to;
		ll w;  // 辺の重み(重み無しは0)
	};
#elif 0
	function<T(T,T)> merge = [](T x1, T x2) -> T {
		return T(max(x1, x2));
	};
	function<T(T)> add_root = [](T x) -> T {
		return T(x + 1);
	};
	struct Edge {
		int to;
		ll w;  // 辺の重み(重み無しは0)
	};
#else  // 各頂点について、その頂点を根としたときに(自身を含め)頂点がいくつあるかを求める場合 もちろん答えは全てN.
	const T identity = 0;
	function<T(T,T)> merge = [](T x1, T x2) -> T {
		return T(x1 + x2);
	};
	function<T(T)> add_root = [](T x) -> T {
		return T(x + 1);
	};
	struct Edge {
		int to;
	};
#endif  // 問題ごとに書き換え
	const T identity;
	using Graph = vector<vector<Edge>>;

	vector<vector<T>> dp;  // dp[v][i]:頂点vを根として考えたときに、i番目の有効辺に対応する値
	vector<T> ans;  // ans[v]:頂点vに対する答
	Graph g;

	// 頂点数N,単位元idntで初期化
	Rerooting(int N, T idnt) : identity(idnt) {
		g.resize(N);
		dp.resize(N);
		ans.resize(N, identity);
	}
	void make_edge(int v, Edge e) {
		g[v].push_back(e);
	}
	T dfs1(int v, int p = -1) {
		T dpcum = identity;
		int deg = g[v].size();
		dp[v].resize(deg, identity);
		for(int i = 0; i < deg; i++)
		{
			int u = g[v][i].to;  // v->u
			if(u == p) continue;
			dp[v][i] = dfs1(u, v);
			// test make_edge
			dp[v][i] += g[v][i].w;
			dpcum = merge(dpcum, dp[v][i]);
		}
		return add_root(dpcum, v);
	}
	// vの親方向における辺のDP値
	void dfs2(int v, const T &val_p, int p = -1) {
		int i;
		int deg = g[v].size();
		for(i = 0; i < deg; i++) {
			if(g[v][i].to == p) dp[v][i] = val_p;
		}
		vector<T> val_l(deg+1, identity), val_r(deg+1, identity);
		for(i = 0; i < deg; i++) {
			val_l[i+1] = merge(val_l[i], dp[v][i]);  // val_l[i]:左からi個のマージ
			val_r[i+1] = merge(val_r[i], dp[v][deg-i-1]);  // val_r[i]:右からi個のマージ
		}

		// 答出力 問題ごとに書き換え (add_rootは不要、など)
		// ans[v] = add_root(val_l[deg], v);
		ans[v] = val_l[deg];

		for(i = 0; i < deg; i++)
		{
			int u = g[v][i].to;  // v->u
			if(u == p) continue;
			T tmp = merge(val_l[i], val_r[deg-i-1]);
			// v->uへの順序としては、"vまでの辺", "頂点v", "v->uへの辺"であるので、この順に操作
//			dfs2(u, add_root(tmp, v), v);
			tmp = add_root(tmp, v);
			// test make_edge
			tmp += g[v][i].w;
			dfs2(u, tmp, v);
		}
	}
	void build(void) {
		dfs1(0);
		dfs2(0, identity);
	}
};

// https://atcoder.jp/contests/typical90/tasks/typical90_c
void Test_Tenkei90_003(void)
{
/*	
	(1)merge,add_root,Edgeは以下を設定	
	(2)dfs2()にてans[v]へは以下を設定
	   ans[v] = add_root(val_l[deg]);
	-------
	function<T(T,T)> merge = [](T x1, T x2) -> T {
		return T(max(x1, x2));
	};
	function<T(T)> add_root = [](T x) -> T {
		return T(x + 1);
	};
	struct Edge {
		int to;
		ll w;  // 辺の重み(重み無しは0)
	};
	-------
*/
	int i;
	using T = int;
	T identity = 0;
	int N; cin >> N;
	Rerooting<T> reroot(N, identity);
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		reroot.make_edge(a, {b, 1});
		reroot.make_edge(b, {a, 1});
	}
	reroot.build();
	cout << *max_element(reroot.ans.begin(), reroot.ans.end()) << endl;
}

void Test_ABC222_F_Expensive_Expense(void)
{
	int i;
	int N; cin >> N;
	using T = ll;
	T identity = 0;
	Rerooting<T> reroot(N, identity);
	for(i = 0; i < N-1; i++)
	{
		int a, b;
		ll c;
		cin >> a >> b >> c;
		a--; b--;
		reroot.make_edge(a, {b, c});
		reroot.make_edge(b, {a, c});
	}
	d.resize(N);
	for(i = 0; i < N; i++) {cin >> d[i];}
	reroot.build();
	for(i = 0; i < N; i++)
	{
		cout << reroot.ans[i] << endl;
	}
	return;
	cout << "---" << endl;
	for(int v = 0; v < N; v++)  // 頂点v
	{
		cout << v+1 << ":" << endl;
		for(auto &e : reroot.dp[v])
		{
			cout << "  " << e << endl;
		}
	}
}

int main(void)
{
/*
	int i;
	int N; cin >> N;
	using T = ll;
	T identity = 0;
	Rerooting<T> reroot(N, identity);
	for(i = 0; i < N-1; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		reroot.make_edge(u, {v, 1});
		reroot.make_edge(v, {u, 1});
	}
	reroot.build();

	for(i = 0; i < N; i++)
	{
		cout << reroot.ans[i] << endl;
	}
*/
	/*
	[代表的なmerge,add_root等の例]
	各頂点から最も遠い頂点までの距離
	---------------
	function<T(T,T)> merge = [](T x1, T x2) -> T {
		return T(max(x1, x2));
	};
	function<T(T)> add_root = [](T x) -> T {
		return T(x + 1);
	};
	struct Edge {
		int to;
		ll w;  // 辺の重み(重み無しは1)
	};
	---
	dfs2()にて :
	ans[v] = val_l[deg];
	---
	呼び出し元にて :
	using T = int;
	---------------
	*/

#if 0
	// 辺に重みがあり、頂点ごと最長の距離
	/*
	input:
	7
	1 2 1
	2 3 2
	3 4 4
	3 5 2
	2 7 5
	1 6 5
	output:
	1:7
	2:6
	3:8
	4:12
	5:10
	6:12
	7:11
	*/
	{
		int i;
		using T = int;
		T identity = 0;
		int N; cin >> N;
		Rerooting<T> reroot(N, identity);
		for(i = 0; i < N-1; i++)
		{
			int u, v, edge_w; cin >> u >> v >> edge_w;
			u--; v--;
			reroot.make_edge(u, {v, edge_w});
			reroot.make_edge(v, {u, edge_w});
		}
		reroot.build();
		for(i = 0; i < N; i++)
		{
			cout << i+1 << ":" << reroot.ans[i] << endl;
		}
		return 0;
	}	
#endif

	const int mode = 2;
	if(mode == 0) {
		Test_Tenkei90_003();
	}
	else if(mode == 1)
	{
		// Test_EDPC_V_Subtree();
	}
	else if(mode == 2)
	{
		Test_ABC222_F_Expensive_Expense();
	}

	return 0;
}
