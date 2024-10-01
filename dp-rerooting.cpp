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

/*
 * [ざっくり概要]
 * 全方位木DPのライブラリ。
 * 辺の統合merge, 子u->親vへの辺追加add_edge, 頂点v追加add_root などを定義することで、
 * 全方位木DPを解けるようにしている。
 * 
 * ★代表的なmerge,add_root等は各Test関数やmain()を参照。
 * 
 * [考え方]
 * ・dp[v][i] :
 *     頂点vを根として考えたときに、i番目の有効辺に対応する値、として定義する
 *     頂点v自体はDP値に含まれない点に注意
 * ・1回目のDFSにて、親v->子uを考えたときに、
 *     再帰でuの部分木(uを含む)のDP値を求め、
 *     v->uの辺を加え、
 *     v以下の辺をマージし、
 *     最後に頂点vを加える
 *   というイメージ。
 * ・2回目のDFSにて、頂点vから全方向に木DPを考えたときに、親v->子uにて
 *     v->u"以外" の辺をマージし、
 *     頂点vを加え、
 *     v->uの辺を加え(マージではない点に注意)、
 *     頂点uへDP値を渡す
 *   というイメージ。
 * 
 * [メソッド等説明]
 * ・T : DPの型 基本はint,llなどで、上位2つの値を保持するならpair<int,int>など
 * ・identity : 単位元e merge()にて e*a=a*e=e であるような値
 * ・Edge : 辺の型 基本はto,wで良いと思われる
 * 
 * ・Rerooting(N,idnt) : 頂点数N, 単位元idnt, そしてmerge(),add_edge(),add_root()で初期化
 * ・merge(x1,x2) : 2つの辺(DP値)に対する二項演算
 * ・add_edge(DP,edge) : DP値に頂点parent->childへの辺を加える
 * ・add_root(DP,v) : DP値に根(頂点v)を加える
 * ・dfs1(v) : DFS1回目 まずはvを根として通常の木DPを行う
 * ・dfs2(v, dp_p) : DFS2回目 頂点vに対して全方向に木DPを行う
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
 * ・EDPC-V(Subtree)
 * ・TDPC-N(木)
 * ・ABC160-F(Distributing Integers)
 * ・ABC222-F(Expensive Expense)
 * ・ABC348-E(Minimize Sum of Distances)
 */

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

/*
 * https://atcoder.jp/contests/typical90/tasks/typical90_c
 * 想定解法は木の直径だが、全方位木DPで解いてみた版。
 * dp[v][i]を、頂点vからのi番目の有向辺以下の部分木にて(vは除く)、その辺の本数と定義する。
 * dp[v][*]を全mergeしたものが、頂点vから最も遠い頂点までの距離。
 * 全頂点についての最大値+1,が答となる。 (問題文より、サイクルを作ってそこも通るため)
 * 
 * merge:2辺のmax
 * add_edge:1加算
 * add_root:何もしない (頂点に重みは無いため)
 * とすると上手くいく。
 */
void Test_Tenkei90_003(void)
{
	int i;
	using T = int;
	auto merge = [](T x1, T x2) -> T {
		return T(max(x1, x2));
	};
	auto add_edge = [](T x, Edge e) -> T {
		return x+1;
	};
	auto add_root = [](T x, int v) -> T {
		return x;
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

/*
 * https://atcoder.jp/contests/abc222/tasks/abc222_f
 * dp[v][i]を、頂点vからのi番目の有向辺以下の部分木にて(vは除く)、
 *   u:vから出る有向辺の先の頂点として、
 *   町vからuの方向へ見たときの、旅費の最大値
 * と定義する。
 * dp[0][*]の全mergeが答。
 * 
 * merge:2辺のmax
 * add_edge:
 *   頂点v->uのコストを加算
 *   頂点uから親v方向へコストを加算するイメージ
 * add_root:
 *   DP値とD[v]のmax
 *   子方向のコストと、自身vのみのコストを比較するイメージ
 * とすると上手くいく。
 */
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
}

/*
 * https://atcoder.jp/contests/abc348/tasks/abc348_e
 * dp[v][i]を、頂点vからのi番目の有向辺以下の部分木にて(vは除く)、
 *   u:vから出る有向辺の先の頂点として、
 *   {f(u), Cの総和}
 * と定義する。
 * dp[0][*]の全mergeが答。
 * 
 * merge:DP値を単純加算
 * add_edge:firstにsecondを加算、つまりf(u)にv->u間の分のCを加算
 * add_root:secondにc[v]を加算
 * とすると上手くいく。
 */
void Test_ABC348_E_Minimize_Sum_of_Distances(void)
{
	int i;
	int N; cin >> N;
	vector<ll> c(N);
	using T = pair<ll,ll>;
	auto merge = [](T x1, T x2) -> T {
		T ret;
		ret.first = x1.first + x2.first;
		ret.second = x1.second + x2.second;
		return ret;
	};
	auto add_edge = [](T x, Edge e) -> T {
		return T(x.first+x.second, x.second);
	};
	auto add_root = [&](T x, int v) -> T {
		return T(x.first, x.second + c[v]);
	};
	T identity = {0,0};
	Rerooting<T> reroot(N, identity, merge, add_edge, add_root);
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		reroot.make_edge(a, {b, 0});
		reroot.make_edge(b, {a, 0});
	}
	for(i = 0; i < N; i++) {cin >> c[i];}
	reroot.build();
	ll ans = INF64;
	for(i = 0; i < N; i++)
	{
		chmin(ans, reroot.ans[i].first);
	}
	cout << ans << endl;
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
	const int mode = 0;
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
		Test_ABC348_E_Minimize_Sum_of_Distances();
	}
	else if(mode == 4) {
		Test_edge_weight_longest();
	}
	else if(mode == 5) {
		Test_count_child();
	}

/*
	その他の問題における定義は以下。
	TDPC-N(木)  https://atcoder.jp/contests/tdpc/tasks/tdpc_tree
	-----------
	using T = pair<ll,ll>;  // {辺数, 通り数}
	auto merge = [](T x1, T x2) -> T {
		T ret;
		ret.first = x1.first + x2.first;
		ret.second = nCk(x1.first+x2.first, x1.first) * x1.second % MOD * x2.second % MOD;
		return ret;
	};
	auto add_edge = [](T x, Edge e) -> T {
		return T(x.first+1, x.second);
	};
	auto add_root = [&](T x, int v) -> T {
		return x;
	};
	T identity = {0,1};
	-----------
 */
	return 0;
}
