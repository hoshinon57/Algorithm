#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC357 https://atcoder.jp/contests/abc357

/*
 * 自分が考えたABC357-E.cppが、イマイチ実装が上手くない感覚があったので、各種解説を参考にした。
 *   https://x.com/kyopro_friends/status/1799437184002601417
 *   https://www.youtube.com/watch?v=yR-51b-0i4E
 * 
 * ABC357-E.cppと違い、問題文の(u,v)にてuについて全探索することを考える。
 * つまり各uにて (u,*) を満たす頂点数を求めて、総和が答。
 * 
 * (1)uがサイクル内の頂点：サイクル内の頂点数
 * (2)uが非サイクル内の頂点：サイクル内の頂点数 + uからサイクルまでの距離
 * となる。
 * 
 * まず(1)を求め、
 * (2)はサイクル内の頂点から、逆方向に辺を張ったグラフにてDFSすれば求められる。
 */

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

using Graph = vector<vector<int>>;
vector<ll> ans;

// 有向グラフにて、頂点vを起点にDFS.
// サイクルを見つけたらtrueを返す。その場合、vからサイクル終端までをhistoryに保持する。
//   historyの例：0->1->2->3->4->2  サイクルは2-3-4の部分。末尾の2が2回出てくることに注意すること。
// seen, finishedの要素数は頂点数で、falseで初期化しておくこと。
// 
// このDFSにて、
// (1)1つの連結成分に2つ以上のサイクルがある場合、
//    サイクル有無の判定は可能。
//    サイクル検出は何らか1つなら可能。全てのサイクル検出は不可能。
// (2)Functional Graphなら連結成分ごとにサイクルは1個なので、
//    呼び出し元で未探索の頂点からDFSすることで全てのサイクルを検出可能。
bool dfs_cycle_detection_directed(Graph &g, int v, vector<bool> &seen, vector<bool> &finished, vector<int> &history)
{
	seen[v] = true;
	history.push_back(v);  // vを行きがけ時に登録し、帰りがけ時に削除

	for(auto &e : g[v])
	{
		if(finished[e]) continue;
		if(seen[e] && !finished[e])
		{
			// サイクル検出した
			finished[v] = true;  // 頂点vについて探索完了
			history.push_back(e);  // サイクルの終端
			return true;
		}
		if(dfs_cycle_detection_directed(g, e, seen, finished, history))  // サイクル検出してreturnしてきた場合は、再帰的にreturnしていく
		{
			finished[v] = true;  // この場合も、頂点vについて探索完了扱いとする
			return true;
		}
	}

	finished[v] = true;
	history.pop_back();
	return false;
}

void dfs(Graph &rg, int v, int p, int cnt)
{
	ans[v] = cnt;
	for(auto &e : rg[v])
	{
		if(e == p) continue;
		dfs(rg, e, v, cnt+1);
	}
}

int main(void)
{
	int i;
	int N; cin >> N;
	Graph g(N), rg(N);
	UnionFind uf(N);
	for(i = 0; i < N; i++)
	{
		int a; cin >> a;
		a--;
		g[i].push_back(a);  // i->a
		rg[a].push_back(i);
		uf.unite(i, a);
	}

	vector<bool> incyc(N, false);  // incyc[i]:頂点iがサイクル内にあればtrue
	ans.resize(N);
	for(i = 0; i < N; i++)
	{
		if(uf.root(i) != i) continue;  // 連結成分ごと
		// 連結成分のどの頂点から探索しても、得られる結果は同じ
		vector<bool> se(N, false), fi(N, false);
		vector<int> history;
		dfs_cycle_detection_directed(g, i, se, fi, history);
		// サイクル内の頂点uについては、(u,*)を満たす数はサイクル内の頂点数と等しい
		int st = find(history.begin(), history.end(), history.back()) - history.begin() + 1;
		for(int j = st; j < (int)history.size(); j++)
		{
			int idx = history[j];
			incyc[idx] = true;
			ans[idx] = (int)history.size() - st;  // サイクル内の頂点数
		}
	}

	// サイクル内の頂点から、逆方向へDFS
	for(i = 0; i < N; i++)
	{
		if(!incyc[i]) continue;
		for(auto &e : rg[i])
		{
			if(incyc[e]) continue;
			dfs(rg, e, i, ans[i]+1);
		}
	}
	ll tot = 0;
	for(auto &e : ans) tot += e;
	cout << tot << endl;

	return 0;
}
