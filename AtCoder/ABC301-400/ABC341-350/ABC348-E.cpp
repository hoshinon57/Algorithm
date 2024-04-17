#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC348 https://atcoder.jp/contests/abc348

/*
 * コンテスト中にWAが取れなかった。
 * 頂点ごとの差分に着目して解く。典型90-39で最初にACした実装と同じ考え方。
 *   https://atcoder.jp/contests/typical90/tasks/typical90_am
 * 
 * ある頂点vから子頂点eに移動したとき、f(v)とf(e)の関係を考えると、
 *   (1)eを根とする部分木について、ΣC だけ減る
 *   (2)それ以外の頂点について、ΣC だけ増える
 * となる。
 * これは事前に各部分木におけるΣCを求めておけば(※)、O(1)で計算できる。
 * (2)はC全体から(1)を引けばよい。
 * 
 * (※)dfs_tree_subnum()の実装を参照。
 *   n[v]を+1ではなくC加算するように変更している。
 * 
 * 頂点1のときだけ、愚直に計算が必要。
 * これには頂点1と各頂点との距離を求めておく必要がある。
 * ⇒自前ライブラリのdfs_tree_depth()を用いる。
 * 
 * …ただ、どうしてもコンテスト中にWAが取れなかった。
 * 原因は min(f(v)) を求める部分にて、INF64より大きい値が答になるケースがあったこと。
 * 自分はINF64を2^60(≒1.1*10^18)としており、
 *   全頂点が横一列に並び、Cは全て最大値
 * のケースにて値を超えてしまい(2.5*10^18ぐらい)、正しいminを求められなかった。
 */

using Graph = vector<vector<int>>;
vector<ll> cost;
vector<ll> depth, subnum;
vector<ll> ans;
ll totc = 0;

// 木であるグラフについて、頂点vを(部分木の)根としてDFS
// 各頂点の深さ(vを0とする)をd[]に設定する
// 呼び出し元からは dfs(g, depth, 0) のように呼び出す
// p:vの親
// now:頂点vの時点の根からの深さ
void dfs_tree_depth(Graph &g_, vector<ll> &d_, int v_, int p_ = -1, int now_ = 0)
{
	d_[v_] = now_;
	for(auto &e : g_[v_])
	{
		if(e == p_) continue;  // 親への逆流を禁止
		dfs_tree_depth(g_, d_, e, v_, now_+1);
	}
}

// 木であるグラフについて、頂点vを(部分木の)根としてDFS
// 各頂点vを部分木としたときの頂点数(vを含む)をn[]に設定する
// 呼び出し元からは dfs(g, sub_num, 0) のように呼び出す
// nは0で初期化しておくこと
// p:vの親
ll dfs_tree_subnum(Graph &g_, vector<ll> &n_, int v_, int p_ = -1)
{
//	n_[v_]++;  // 自身のぶん
	n_[v_] += cost[v_];
	for(auto &e : g_[v_])
	{
		if(e == p_) continue;  // 親への逆流を禁止
		n_[v_] += dfs_tree_subnum(g_, n_, e, v_);
	}
	return n_[v_];
}

void dfs(Graph &g, ll sum, int v, int p = -1)
{
	ans[v] = sum;
	for(auto &e : g[v])
	{
		if(e == p) continue;
		ll d1 = subnum[e];
		ll d2 = totc - subnum[e];
		dfs(g, sum-d1+d2, e, v);
	}
}

int main(void)
{
	ll i;
	int N; cin >> N;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	cost.resize(N);
	for(i = 0; i < N; i++)
	{
		cin >> cost[i];
		totc += cost[i];
	}
	depth.resize(N);
	subnum.resize(N);
	dfs_tree_depth(g, depth, 0);
	dfs_tree_subnum(g, subnum, 0);

	ans.resize(N);
	for(i = 0; i < N; i++)  // ans[0]のみ愚直に計算
	{
		ans[0] += cost[i]*depth[i];
	}
	dfs(g, ans[0], 0);
	ll ans2 = INF64;
	for(i = 0; i < N; i++)
	{
		chmin(ans2, ans[i]);
	}
	cout << ans2 << endl;

	return 0;
}
