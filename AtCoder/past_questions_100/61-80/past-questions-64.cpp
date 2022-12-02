#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題64
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_2_A&lang=ja

/*
 * クラスカル法を用いて最小全域木を求める。
 * 実装自体はいわゆる「教科書通りの実装」であり、説明は省略する。
 * 
 * ※
 * 今回、けんちょん氏の本での実装と違い、辺を pair<int, pair<int,int>> ではなく構造体を用いて定義した。
 * sort()で自作構造体を扱えるように、比較関数を定義する実装を試すのが目的。
 * 
 * 計算量はO(ElogE), または(ElogV).
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

struct Edge
{
	int s, t, w;
	Edge(int s_, int t_, int w_) : s(s_), t(t_), w(w_) {}
};
bool cmp(const Edge &a, const Edge &b)
{
	return a.w < b.w;
}

int main(void)
{
	int i;
	int V, E;
	cin >> V >> E;
	UnionFind uf(V);
	vector<Edge> edges;
	for(i = 0; i < E; i++)
	{
		int s, t, w;
		cin >> s >> t >> w;
		edges.push_back(Edge(s, t, w));
	}
	sort(edges.begin(), edges.end(), cmp);

	ll answer = 0;
	for(i = 0; i < E; i++)
	{
		// この辺を採用することでサイクルが発生するならば、使わない
		if(uf.issame(edges[i].s, edges[i].t)) continue;

		// 辺を追加し、辺の重みを答に加算
		uf.unite(edges[i].s, edges[i].t);
		answer += edges[i].w;
	}
	cout << answer << endl;

	return 0;
}
