#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC292 https://atcoder.jp/contests/abc292

/*
 * Union-Findで解く。
 * 
 * edge_num[i]:Union-Findで根がiの連結成分について、辺の数
 * として定める。
 * 頂点u,vを結ぶ辺が与えられたときに、
 * ・u,vが同じ連結成分に属する場合：
 *   edge_num[(uの根)] を+1する。
 * ・u,vが異なる連結成分に属する場合：
 *   edge_num[(uの根)] + edge_num[(vの根)] + 1 がu,vを統合した後の辺の数になる。
 * 
 * 最後に、各連結成分について頂点数と辺数が一致するかを調べればよい。
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

int main(void)
{
	// 0-indexed
	int i;
	int N, M;
	cin >> N >> M;
	UnionFind uf(N);
	vector<int> edge_num(N);  // edge_num[i]:UFで根がiの連結成分について、辺の数
	for(i = 0; i < M; i++)
	{
		int u, v;
		cin >> u >> v;
		u--;
		v--;
		if(uf.issame(u, v))
		{
			edge_num[uf.root(u)]++;
		}
		else
		{
			int n1 = edge_num[uf.root(u)];
			int n2 = edge_num[uf.root(v)];
			uf.unite(u, v);
			edge_num[uf.root(u)] = n1 + n2 + 1;
		}
	}

	auto check = [&](void) -> bool
	{
		for(i = 0; i < N; i++)
		{
			if(uf.root(i) != i) continue;
			if(uf.size(i) != edge_num[uf.root(i)]) return false;
		}
		return true;
	};

	YesNo(check());

	return 0;
}
