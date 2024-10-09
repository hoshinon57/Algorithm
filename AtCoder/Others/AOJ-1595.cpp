#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <functional>  // function
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1595

// 全方位木DPライブラリをぺたり。
// 各頂点iについて、頂点iを根とした木の深さをDとしたときに、
// 2(N-1)-D が答。
// ※最後、頂点iに戻ってくるという条件なら 2(N-1) が移動距離になる。
//   戻らないぶん移動距離が減り、それは頂点iから最も遠い点にすれば最適。

// 問題ごとに必要なら書き換え
struct Edge {
	int to;
	long long w;  // 辺の重み(重み無しは0)
};

// 全方位木DPのライブラリ
// [事前準備]
//   T:DPの型を決める。
//   merge(),add_edge(),add_root(),および単位元identityを定義し、
//   頂点数をNとして以下のように定義する。
//     Rerooting<T> reroot(N, identity, merge, add_edge, add_root);
//   dfs2()でのans[]への設定を、問題に応じて書き換える。
// [使い方]
//   (1)make_edge()でグラフを構築
//   (2)build()で全方位木DPを計算
//   (3)ans[]を出力、または呼び出し元でdp[v][i]を全マージして必要ならadd_root()するなど
template <typename T>
struct Rerooting {
public:
	using Graph = vector<vector<Edge>>;
	vector<vector<T>> dp;  // dp[v][i]:頂点vを根として考えたときに、i番目の有効辺に対応する値
	vector<T> ans;  // ans[v]:頂点vに対する答
	Graph g;
private:  // add_edgeとmake_edgeを取り違えるという理由でprivateにて…
	const T identity;
	using f1 = function<T(T,T)>; // merge(x1,x2):2つの辺(DP値)に対する二項演算
	using f2 = function<T(T,Edge)>;  // add_edge(DP,e):DP値に頂点root->childへの辺を加える
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
	// DFS1回目 まずはvを根として通常の木DPを行う
	// 頂点vの部分木について、辺ごとにDP値を求める -> dp[v][i]
	// 戻り値:頂点v以下(vを含む)の部分木のDP値
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
	// dp_p:vの親方向における辺のDP値
	void dfs2(int v, const T &dp_p, int p = -1) {
		int i;
		int deg = g[v].size();
		for(i = 0; i < deg; i++) {
			if(g[v][i].to == p) dp[v][i] = dp_p;
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

int main(void)
{
	int i;
	using T = int;
	auto merge = [](T x1, T x2) -> T { return max(x1,x2); };
	auto add_edge = [](T x, Edge e) -> T { return x+1; };
	auto add_root = [&](T x, int v) -> T { return x; };
	T identity = {0};
	int N; cin >> N;
	Rerooting<T> reroot(N, identity, merge, add_edge, add_root);
	for(i = 0; i < N-1; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		reroot.make_edge(u, {v, 0});
		reroot.make_edge(v, {u, 0});
	}
	reroot.build();
	for(i = 0; i < N; i++)
	{
		cout << 2*(N-1) - reroot.ans[i] << endl;
	}

	return 0;
}
