#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>  // function
#include <array>
#include <cassert>
using namespace std;
typedef long long ll;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }

// 全方位木DPのメモやライブラリ
// ★注意★ #include <functional> を忘れずに。ローカル環境では無くてもビルドが通るが、AtCoderではCEになる。

// memo/todo:
// dfs1,dfs2をコメント等整理
// [済]抽象化 add_edge
// [済]木の直径を解いてみる
// [済]EDPC-V
// [済]ABC222-F
// ABC348-E
// ABC220-F(Distance Sums 2)
// ABC160-F(Distributing Integers) 挑戦
// TDPC-N(木) 挑戦 https://atcoder.jp/contests/tdpc/tasks/tdpc_tree

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

// 問題ごとに必要なら書き換え
struct Edge {
	int to;
	ll w;  // 辺の重み(重み無しは0)
};

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
public:
	using Graph = vector<vector<Edge>>;
	vector<vector<T>> dp;  // dp[v][i]:頂点vを根として考えたときに、i番目の有効辺に対応する値
	vector<T> ans;  // ans[v]:頂点vに対する答
	Graph g;
private:  // add_edgeとmake_edgeを取り違えるという理由でprivateにて…
	const T identity;
	using f1 = function<T(T,T)>; // merge
	using f2 = function<T(T,Edge)>;  // add_edge(T,e):DP値に頂点root->childへの辺を加える
	using f3 = function<T(T,int)>;  // add_root(DP,v):DP値に根(頂点v)を加える
	f1 merge;
	f2 add_edge;
	f3 add_root;

public:
	// 頂点数N,単位元idntで初期化
	Rerooting(int N, T idnt, f1 merge_, f2 add_edge_, f3 add_root_) : identity(idnt), merge(merge_), add_edge(add_edge_), add_root(add_root_) {
		g.resize(N);
		dp.resize(N);
		ans.resize(N, identity);
	}
	void make_edge(int v, Edge e) {
		g[v].push_back(e);
	}
	void build(void) {
		dfs1(0);
		dfs2(0, identity);
	}
private:
	// 頂点0を根とし、初回の木DP
	// 頂点vの部分木について、辺ごとにDP値を求める -> dp[v][i]
	// 頂点v以下(vを含む)の部分木のDP値を返す
	T dfs1(int v, int p = -1) {
		T dpcum = identity;
		int deg = g[v].size();
		dp[v].resize(deg, identity);
		for(int i = 0; i < deg; i++)
		{
			int u = g[v][i].to;  // v->u
			if(u == p) continue;
			// dp[v][i]を求めるにあたり、頂点u以下(uを含む)の部分木のDP値に、v->uへの辺を追加する
			// それを全辺についてマージしておき、最後に頂点vを追加する
			dp[v][i] = dfs1(u, v);
			dp[v][i] = add_edge(dp[v][i], g[v][i]);
			dpcum = merge(dpcum, dp[v][i]);
		}
		return add_root(dpcum, v);
	}
	// 頂点vに対して全方向に木DP
	// val_p:vの親方向における辺のDP値
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

		// 答出力 問題ごとに書き換え
		ans[v] = val_l[deg];
		// ans[v] = add_root(val_l[deg], v);

		for(i = 0; i < deg; i++)  // i番目の辺へ向かって再帰
		{
			int u = g[v][i].to;  // v->u
			if(u == p) continue;
			// 以下、v->uへの順序としては "vまでの辺", "頂点v", "v->uへの辺"であるので、この順に操作
			T t = merge(val_l[i], val_r[deg-i-1]);  // "v->uへ向かう辺" 以外の辺を統合
			t = add_root(t, v);  // 頂点v追加
			t = add_edge(t, g[v][i]);  // v->uへの辺追加
			dfs2(u, t, v);
		}
	}
};

// https://atcoder.jp/contests/typical90/tasks/typical90_c
void Test_Tenkei90_003(void)
{
	int i;
	using T = int;
	auto merge = [](T x1, T x2) -> T {
		return T(max(x1, x2));
	};
	auto add_edge = [](T x, Edge e) -> T {
		return x;  // 辺の重み無し
	};
	auto add_root = [](T x, int v) -> T {
		return T(x + 1);
	};
	// Rerooting::dfs2()におけるans[]は以下を設定する
	// ans[v] = val_l[deg];
	T identity = 0;
	int N; cin >> N;
	Rerooting<T> reroot(N, identity, merge, add_edge, add_root);
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		reroot.make_edge(a, {b, 1});
		reroot.make_edge(b, {a, 1});
	}
	reroot.build();
	cout << *max_element(reroot.ans.begin(), reroot.ans.end())+1 << endl;
}

/*
 * https://atcoder.jp/contests/dp/tasks/dp_v
 * dp[v][i]を、v->iへの有向辺(vは除く)にて、頂点iが黒で合法な通り数とする。
 * 頂点i以下が白のケースはadd_root()にて加算される。
 * merge()は各辺の積となることもあり、identity=1が適切。
 * 
 * 最初、mergeにて(x1+1)*(x2+1)とやっていて計算が合わなかった。
 * 上記だとモノイドになっていない。(左右からの結合則が成り立たない)
 */
void Test_EDPC_V_Subtree(void)
{
	ll N, M; cin >> N >> M;
	using T = ll;
	auto merge = [&](T x1, T x2) -> T {
		return x1*x2%M;
	};
	auto add_edge = [](T x, Edge e) -> T {
		return x;
	};
	auto add_root = [&](T x, int v) -> T {
		return x+1;  // v以下の頂点が全て白のケースで+1
	};
	// Rerooting::dfs2()におけるans[]は以下を設定する
	// ans[v] = val_l[deg];
	T identity = 1;
	Rerooting<T> reroot(N, identity, merge, add_edge, add_root);
	int i;
	for(i = 0; i < N-1; i++)
	{
		int x, y; cin >> x >> y;
		x--; y--;
		reroot.make_edge(x,{y,0});
		reroot.make_edge(y,{x,0});
	}
	reroot.build();
	for(i = 0; i < N; i++) cout << reroot.ans[i] << endl;
}

void Test_ABC222_F_Expensive_Expense(void)
{
	int i;
	int N; cin >> N;
	vector<ll> d(N);

	using T = ll;
	auto merge = [](T x1, T x2) -> T {
		return T(max(x1, x2));
	};
	auto add_edge = [](T x, Edge e) -> T {
		return x+e.w;
	};
	auto add_root = [&](T x, int v) -> T {
		return max(x, T(d[v]));
	};
	// Rerooting::dfs2()におけるans[]は以下を設定する
	// ans[v] = val_l[deg];
	T identity = 0;
	Rerooting<T> reroot(N, identity, merge, add_edge, add_root);
	for(i = 0; i < N-1; i++)
	{
		int a, b;
		ll c;
		cin >> a >> b >> c;
		a--; b--;
		reroot.make_edge(a, {b, c});
		reroot.make_edge(b, {a, c});
	}
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

// 全方位木DPテスト用
// 辺に重みがあり、頂点ごとに最長の距離を求める
// Rerooting::dfs2()におけるans[]は以下を設定する
// ans[v] = val_l[deg];
void Test_edge_weight_longest(void)
{
	// テストケース
	const int N = 7;
	const vector<array<int,3>> edge = {{1,2,1}, {2,3,2}, {3,4,4}, {3,5,2}, {2,7,5}, {1,6,5}};  // {u,v,weight}
	const vector<int> ans = {7, 6, 8, 12, 10, 12, 11};

	cout << "Test start : Test_edge_weight_longest()" << endl;
	int i;
	using T = int;
	auto merge = [](T x1, T x2) -> T {
		return T(max(x1, x2));
	};
	auto add_edge = [](T x, Edge e) -> T {
		return x+e.w;
	};
	auto add_root = [&](T x, int v) -> T {
		return x;  // 頂点の重み無し
	};
	T identity = 0;
	Rerooting<T> reroot(N, identity, merge, add_edge, add_root);
	for(i = 0; i < N-1; i++)
	{
		auto [u,v,edge_w] = edge[i];
		u--; v--;
		reroot.make_edge(u, {v, edge_w});
		reroot.make_edge(v, {u, edge_w});
	}
	reroot.build();
	for(i = 0; i < N; i++) assert(reroot.ans[i] == ans[i]);
	cout << "Test end : Test_edge_weight_longest()" << endl;
}

// 全方位木DPテスト用
// 頂点ごとに子の頂点数を返す もちろん答は全てN-1
// Rerooting::dfs2()におけるans[]は以下を設定する
// ans[v] = val_l[deg];
void Test_count_child(void)
{
	// テストケース
	const int N = 8;
	const vector<array<int,2>> edge = {{1,2}, {2,3}, {3,4}, {3,5}, {2,7}, {7,8}, {1,6}};  // {u,v}

	cout << "Test start : Test_count_child()" << endl;
	int i;
	using T = int;
	auto merge = [](T x1, T x2) -> T {
		return T(x1+x2);
	};
	auto add_edge = [](T x, Edge e) -> T {
		return x;  // 辺の重み無し
	};
	auto add_root = [&](T x, int v) -> T {
		return x+1;
	};
	T identity = 0;
	Rerooting<T> reroot(N, identity, merge, add_edge, add_root);
	for(i = 0; i < N-1; i++)
	{
		auto [u,v] = edge[i];
		u--; v--;
		reroot.make_edge(u, {v, 0});
		reroot.make_edge(v, {u, 0});
	}
	reroot.build();
	for(i = 0; i < N; i++) assert(reroot.ans[i] == N-1);
	cout << "Test end : Test_count_child()" << endl;
}

int main(void)
{
	const int mode = 1;
	if(mode == 0) {
		Test_Tenkei90_003();
	}
	else if(mode == 1) {
		Test_EDPC_V_Subtree();
	}
	else if(mode == 2) {
		Test_ABC222_F_Expensive_Expense();
	}
	else if(mode == 3) {
		Test_edge_weight_longest();
	}
	else if(mode == 4) {
		Test_count_child();
	}

	return 0;
}
