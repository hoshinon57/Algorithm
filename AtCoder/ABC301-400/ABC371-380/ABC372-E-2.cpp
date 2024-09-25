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

// ABC372 https://atcoder.jp/contests/abc372

// setではなくvectorで頂点集合を管理。
// クエリはkが最大10なので、愚直にマージしてサイズを10に抑える処理で間に合う。

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
	int i;
	int N, Q; cin >> N >> Q;
	UnionFind uf(N);
	vector<vector<int>> vv(N, vector<int>(10, -2));  // vv[i]:頂点iを根とする連結成分について、頂点番号が大きい順に最大k個
	for(i = 0; i < N; i++) vv[i][0] = i;
	while(Q > 0)
	{
		Q--;
		int kind; cin >> kind;
		if(kind == 1)
		{
			int u, v; cin >> u >> v;
			u--; v--;
			if(uf.issame(u, v)) continue;
			int ru = uf.root(u), rv = uf.root(v);
			uf.unite(u, v);
			// u->vの方向へマージ
			if(uf.root(u) == ru) swap(ru, rv);
			for(auto &e : vv[ru]) vv[rv].push_back(e);
			sort(vv[rv].begin(), vv[rv].end(), greater<int>());
			vv[rv].resize(10, -2);  // 最大10個
		}
		else
		{
			int v, k; cin >> v >> k;
			v--;
			cout << vv[uf.root(v)][k-1]+1 << endl;
		}
	}

	return 0;
}
