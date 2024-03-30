#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC148 https://atcoder.jp/contests/abc148

/*
 * 深さ優先探索で解く。
 * 
 * まず、ある頂点iに高橋君が行けるかを考える。
 * これはスタート地点u,vについて
 *   (a)u->iの距離 < (b)v->iの距離
 * であれば到達可能。
 * 
 * また、高橋君の最適な行動は
 *   ある葉について、葉とその1つ親を行ったり来たりする
 * のが最適っぽいと考える。
 * それより親側で行ったり来たりは、メリットが無いので。
 * 
 * 以上を踏まえると、ある葉の頂点iについて
 *   (a)u->iの距離 < (b)v->iの距離
 * であれば、(b)-1が青木くんが移動する回数になる。
 * ※-1は紙で色々書いたら分かった。
 * 　距離の偶奇とかは関係なく、必ず葉の1つ親で捕まる形になる。
 *   これはゲーム終了の条件が「青木くんの移動回数」であるため。「高橋君の移動回数」ならば場合分けが必要だったっぽい。
 * これを全ての葉について計算し、その最大値が答。
 * 
 * …というのを、紙にごちゃごちゃ書いてたら見えてきた。
 * 
 * [ACまでの思考の流れ]
 * ・お互いにどういう行動になるかを考えてみる。
 *   高橋君はきっと葉とその1つ親の間で行き来することになるだろう。
 * ・お互いのスタート地点をそれぞれ根としたときの、各頂点への距離を求めたらどうか。
 * ・2人の距離の差が偶数か奇数かで場合分け必要？
 *   ⇒いくつか紙で試したところ、場合分けは不要だった。
 * 
 * [参考になるページ]
 * https://at274.hatenablog.com/entry/2020/01/02/110121
 * https://ikatakos.com/pot/programming_algorithm/contest_history/atcoder/2019/1222_abc148
 * https://kmjp.hatenablog.jp/entry/2019/12/22/1030
 */

using Graph = vector<vector<int>>;

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

int main(void)
{
	// 0-indexed
	int i;
	int N, u, v; cin >> N >> u >> v;
	u--; v--;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}

	vector<int> a_dep(N), b_dep(N);  // a_dep[i]:高橋君がいる頂点を根としたときの、頂点iへの距離
	dfs_tree_depth(g, a_dep, u);
	dfs_tree_depth(g, b_dep, v);

	int ans = 0;
	for(i = 0; i < N; i++)
	{
		if(g[i].size() != 1) continue;  // 根のみ調べる
		if(a_dep[i] < b_dep[i])
		{
			chmax(ans, b_dep[i]-1);
		}
	}
	cout << ans << endl;

	return 0;
}
