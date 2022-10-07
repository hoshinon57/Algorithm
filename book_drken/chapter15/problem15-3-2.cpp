#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 章末問題15.3
// AOJ 1350
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1350
// https://onlinejudge.u-aizu.ac.jp/problems/1350

/*
 * クラスカル法とサイクル検出で自力で解いた後、
 * 以下の解説を読んで実装した内容。
 * https://drken1215.hatenablog.com/entry/2020/11/04/221100
 * 
 * クラスカル法の関数に、「この辺は使わない」という引数を追加する。
 * まずはその引数を-1にして、全ての辺を使って最小全域木を作る。
 * 次に最小全域木の辺1つずつに対し、「その辺を使わない」ようにして最小全域木を求める。
 * このときの重みが増えるのであれば、必ず含まれる辺と判断できる。
 * 
 * 辺を除外したとき、そもそも全域木を作れない可能性がある点には注意。
 * 辺の数が(頂点数-1)より小さいなら全域木を作れていないと判断し、
 * 無限大の重みとすればよい。
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

using Edge = pair<int, pair<int,int>>;  // 辺e=(u,v)を{w(e),{u,v}}で表す

vector<Edge> edges;  // 入力の辺

int N, M;

// exclusion_edge:-1なら除外無し
int Kruskal(int exclusion_edge, vector<int> &MST_edges)
{
	int cost = 0;
	UnionFind uf(N);

	for(int i = 0; i < M; i++)
	{
		if(i == exclusion_edge) continue;  // 除外する辺

		int u = edges[i].second.first;
		int v = edges[i].second.second;
		int w = edges[i].first;

		// 辺の追加によってサイクルが形成される場合はスルー
		if(uf.issame(u, v)) continue;

		// 辺を追加する
		cost += w;
		uf.unite(u, v);
		MST_edges.push_back(i);
	}

	// 辺除外したことにより、そもそも全域木を作れなかった場合
	if((int)MST_edges.size() < N-1)
	{
		cost = INF32;  // 除外した辺はMSTにて必ず含まれるため、無限大のコストを返しておく
	}

	return cost;
}

int main(void)
{
	// 0-indexed
	int i;
	cin >> N >> M;
	edges.assign(M, Edge());
	vector<int> MST_edges;  // クラスカル法により求めたMSTの辺の番号 重みでソート後の番号になる点には注意
	for(i = 0; i < M; i++)
	{
		int u, v, w;
		cin >> u >> v >> w;
		u--;  // 0-indexedに変換
		v--;
		edges[i] = make_pair(w, make_pair(u, v));
	}
	// ここまで入力

	// 辺を重みが小さい順にソート
	sort(edges.begin(), edges.end());

	int MST_cost = Kruskal(-1, MST_edges);

	int ans_num = 0;
	int ans_cost = 0;
	for(auto &e : MST_edges)
	{
		vector<int> dummy;
		// e番目の辺を除外してクラスカル法
		// 第2引数は使わないためダミー
		if(Kruskal(e, dummy) > MST_cost)
		{
			ans_num++;
			ans_cost += edges[e].first;
		}
	}
	cout << ans_num << " " << ans_cost << endl;

	return 0;
}
