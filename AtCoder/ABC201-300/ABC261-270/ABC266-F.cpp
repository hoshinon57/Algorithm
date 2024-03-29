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

// ABC266 https://atcoder.jp/contests/abc266

/*
 * いやー方針に漏れがあったのと、実装どうするかで結構苦戦した。
 * DFSやUnionFindを用いて解く。
 * 
 * 基本は公式解説の通り。
 *   https://atcoder.jp/contests/abc266/editorial/4698
 *   https://twitter.com/kyopro_friends/status/1563527591075942400
 * N頂点N辺のグラフはサイクルを1つだけ持つ。
 * 非サイクル上の各頂点について、サイクル上の頂点(1つ)とを同じグループにしておくと(※)、
 * クエリについては同じグループかどうかで判定できる。
 * (※)DFSとUnionFindにて求められる
 */

using Graph = vector<vector<int>>;

struct UnionFind
{
	vector<int> parent;  // parent[x]:xの親の番号 xが根であれば-1
	vector<int> siz;  // siz[x]:xの属するグループの頂点数

	// 頂点数nで初期化
	UnionFind(int n) : parent(n, -1), siz(n, 1) {}

	// 根を求める
	int root(int x)
	{
		if(parent[x] == -1) return x;  // xが根であれば、xを返す
		else
		{
			parent[x] = root(parent[x]);
			return parent[x];
		}
	}

	// xとyが同じグループに属するか
	bool issame(int x, int y)
	{
		return root(x) == root(y);  // 根が同一なら同じグループ
	}

	// xを含むグループとyを含むグループを併合する
	bool unite(int x, int y)
	{
		// x,yをそれぞれ根まで移動させる
		x = root(x);
		y = root(y);

		// すでに同じグループであれば何もしない
		if(x == y) return false;

		// 頂点数が少ない方が子になるようにする
		if(siz[x] < siz[y])
		{
			// xをyの子にする
			parent[x] = y;
			siz[y] += siz[x];
		}
		else
		{
			// yをxの子にする
			parent[y] = x;
			siz[x] += siz[y];
		}
		return true;
	}

	// xを含むグループの頂点数
	int size(int x)
	{
		return siz[root(x)];
	}
};

// 無向グラフにて、頂点vを起点にDFS.
// サイクルを見つけたらtrueを返す。その場合、vからサイクル終端までをhistoryに保持する。
//   historyの例：0->1->2->3->4->2  サイクルは2-3-4の部分。末尾の2が2回出てくることに注意すること。
// p:vの親で、逆流防止に使う。呼び出し元からは-1で指定すること。
// seen, finishedの要素数は頂点数で、falseで初期化しておくこと。
bool dfs_cycle_detection_undirected(Graph &g, int v, int p /* =-1 */, vector<bool> &seen, vector<bool> &finished, vector<int> &history)
{
	seen[v] = true;
	history.push_back(v);  // vを行きがけ時に登録し、帰りがけ時に削除

	for(auto &e : g[v])
	{
		if(e == p) continue;  // 親への逆流を禁止
		if(finished[e]) continue;
		if(seen[e] && !finished[e])
		{
			// サイクル検出した
			finished[v] = true;  // 頂点vについて探索完了
			history.push_back(e);  // サイクルの終端
			return true;
		}
		if(dfs_cycle_detection_undirected(g, e, v, seen, finished, history))  // サイクル検出してreturnしてきた場合は、再帰的にreturnしていく
		{
			finished[v] = true;  // この場合も、頂点vについて探索完了扱いとする
			return true;
		}
	}

	finished[v] = true;
	history.pop_back();
	return false;
}

void dfs(UnionFind &uf, Graph &g, int v, int p = -1)
{
	if(p != -1) uf.unite(v, p);
	for(auto &e : g[v])
	{
		if(e == p) continue;
		dfs(uf, g, e, v);
	}
}

int main(void)
{
	// 0-indexed
	int i;
	int N; cin >> N;
	Graph g(N);
	for(i = 0; i < N; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}

	// 1つだけ存在するサイクルを検出し、incyc[]に設定する
	vector<bool> seen(N, false), finished(N, false);
	vector<int> hist;
	dfs_cycle_detection_undirected(g, 0, -1, seen, finished, hist);
	vector<bool> incyc(N, false);  // incyc[i]:頂点iがサイクルに含まれていればtrue
	auto itr = find(hist.begin(), hist.end(), hist.back());
	while(itr != hist.end())
	{
		incyc[*itr] = true;
		itr++;
	}

	// サイクル上の頂点から、非サイクル上の頂点に向けてDFS
	UnionFind uf(N);
	for(i = 0; i < N; i++)
	{
		if(!incyc[i]) continue;
		for(auto &e : g[i])
		{
			if(incyc[e]) continue;
			dfs(uf, g, e, i);  // サイクル上のiを親とする これにより、探索途中でサイクル上の頂点に行くことは無い(サイクルは1つなので)
		}
	}

	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		int x, y; cin >> x >> y;
		x--; y--;
		YesNo(uf.issame(x, y));
	}

	return 0;
}
