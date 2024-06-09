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
 * 問題文の(u,v)にて、vについて全探索することを考える。
 * つまり各vにて (*,v) を満たす頂点数を求めて、総和が答。
 * 
 * functional graphであるため、
 * 各連結成分ごとに常に1つのサイクルが存在する。
 * これを念頭に、以下の手順にて実装した。
 * ・連結成分ごとに、サイクル内の頂点を列挙する。
 *   ⇒incycに設定した。
 * ・入次数が0である頂点からBFS.
 *   頂点vにおける (*,v) を満たす頂点数をans[v]とすると、
 *   v->e となる辺があったときに、ans[e]にans[v]を加算していけばよい。
 *   (v,v)は条件を満たすので、必ず+1されるようにしておく。
 * ・サイクル内の頂点について(*,v)を満たす数は、その連結成分の頂点数に等しい。
 * ・以上でans[]が求まるので、その総和が答。
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
vector<bool> incyc;  // incyc[i]:頂点iがサイクル内にあればtrue

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

int main(void)
{
	int i;
	int N; cin >> N;
	Graph g(N);
	UnionFind uf(N);
	vector<int> in(N);  // in[i]:頂点iの入次数
	for(i = 0; i < N; i++)
	{
		int a; cin >> a;
		a--;
		g[i].push_back(a);  // i->a
		uf.unite(i, a);
		in[a]++;
	}

	incyc.resize(N, false);
	for(i = 0; i < N; i++)
	{
		if(uf.root(i) != i) continue;  // 連結成分ごと
		// 連結成分のどの頂点から探索しても、得られる結果は同じ
		vector<bool> se(N, false), fi(N, false);
		vector<int> history;
		dfs_cycle_detection_directed(g, i, se, fi, history);
		auto itr = find(history.begin(), history.end(), history.back());
		while(itr != history.end())
		{
			incyc[*itr] = true;
			itr++;
		}
	}

	vector<ll> ans(N);
	queue<int> que;
	for(i = 0; i < N; i++) if(in[i] == 0) que.push(i);
	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		ans[v]++;  // (v,v) 自分自身
		for(auto &e : g[v])
		{
			in[e]--;
			ans[e] += ans[v];
			if(in[e] == 0) que.push(e);
		}
	}
	// サイクル内の頂点は、その連結成分数がans[]となる(上書きする)
	for(i = 0; i < N; i++)
	{
		if(incyc[i]) ans[i] = uf.size(i);
	}

	ll tot = 0;
	for(auto &e : ans) tot += e;
	cout << tot << endl;

	return 0;
}
