#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// code15.1の写経

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

// 辺e=(u,v)を{w(e),{u,v}}で表す
using Edge = pair<int, pair<int,int>>;

int main(void)
{
	int i;
	int N, M;
	cin >> N >> M;
	vector<Edge> edges(M);  // 辺の集合
	for(i = 0; i < M; i++)
	{
		int u, v, w;
		cin >> u >> v >> w;
		edges[i] = Edge(w, make_pair(u,v));
	}

	// 辺を重みが小さい順にソート
	sort(edges.begin(), edges.end());

	int res = 0;
	UnionFind uf(N);
	for(i = 0; i < M; i++)
	{
		int w = edges[i].first;
		int u = edges[i].second.first;
		int v = edges[i].second.second;

		// 辺の追加によってサイクルが形成される場合はスルー
		if(uf.issame(u, v)) continue;

		// 辺を追加する
		res += w;
		uf.unite(u, v);
	}
	cout << res << endl;

	return 0;
}
