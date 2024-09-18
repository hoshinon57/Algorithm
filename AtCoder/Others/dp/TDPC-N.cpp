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

// https://atcoder.jp/contests/tdpc/tasks/tdpc_tree

/*
 * 全方位木DPで解いた。
 * ※木DPを頂点数回繰り返すO(N^2)が想定解法っぽい？
 *   自分は全方位木DPの関連問題として紹介されていたのを見たので、まず全方位木DPで考えてしまった。
 * 
 * ある頂点vを根として考えたときに、葉の方から考えてみる。
 * 1-2, 1-3, 3-4 の3辺があったとき、
 * (1)1-2側は辺1つ、描き方1つ。
 * (2)1-3側は辺2つ、描き方1つ。
 * 全体で、辺3つ、描き方3つとなる。
 * いくつかパターンを手で書いてみたところ、{辺数x, 描き方y}の2辺があったとき、
 *   Comb(x1+x2, x1) * y1 * y2
 * が2辺の描き方になりそう、となった。
 * グループ間での描き方がComb, グループ1単体での描き方がy1なので、その積。
 * これを全方位木DPライブラリのmergeに用いる。
 * 
 * add_edgeおよび単位元は実装を参照。単位元の決め方にちょっと悩んだ。
 * 各頂点を根としたときの描き方の総和が答。
 * ただしある辺について頂点v->uとu->v側、それぞれカウントされるので2で割る。
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

const ll MOD = 1e9+7;
const ll NCK_SIZE = 200001;  // N<=2*10^5 など、問題文の制約に合わせる
ll fact[NCK_SIZE], inv_fact[NCK_SIZE], inv[NCK_SIZE];

// Combination用の事前計算
void nCk_init(void)
{
	fact[0] = fact[1] = 1;
	inv[0] = inv[1] = 1;
	inv_fact[0] = inv_fact[1] = 1;

	for(int i = 2; i < NCK_SIZE; i++)
	{
		// 階乗
		fact[i] = fact[i - 1] * i % MOD;
		// 逆元
		inv[i] = MOD - inv[MOD%i] * (MOD / i) % MOD;
		// 逆元の階乗
		inv_fact[i] = inv_fact[i - 1] * inv[i] % MOD;
	}
}

// CombinationのnCkを求める
ll nCk(ll n, ll k)
{
	if (n < k) return 0;
	if (n < 0 || k < 0) return 0;

	ll x = fact[n];  // n!の計算
	ll y = inv_fact[n-k];  // (n-k)!の計算
	ll z = inv_fact[k];  // k!の計算
	
	return x * ((y * z) % MOD) % MOD;  //nCkの計算
}

// mod mの世界におけるaの逆元を返す
// 以下URLのコードをそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modinv(long long a, long long m)
{
	long long b = m, u = 1, v = 0;
	while (b) {
        long long t = a / b;
        a -= t * b; swap(a, b);
        u -= t * v; swap(u, v);
    }
    u %= m; 
    if (u < 0) u += m;
    return u;
}

int main(void)
{
	nCk_init();
	int i;
	int N; cin >> N;
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
	Rerooting<T> reroot(N, identity, merge, add_edge, add_root);
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		reroot.make_edge(a, {b, 0});
		reroot.make_edge(b, {a, 0});
	}
	reroot.build();
	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		ans += reroot.ans[i].second;
		ans %= MOD;
	}
	ans = ans * modinv(2, MOD) % MOD;
	cout << ans << endl;

	return 0;
}
