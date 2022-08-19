#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 章末問題15.2

/*
 * クラスカル法で解く。
 * 
 * 最小全域木を構成する辺の重みを列挙したときに、その辺の中央値が答となる。
 * よってクラスカル法で最小全域木を求めて、辺の中央値を出力すればよい。
 * 
 * 上記方針の証明は、以下参考URLにて。
 * 感覚的には、
 *   最小全域木の辺の重みが{1,2,3}とする。2が答となる。
 *   重み3の辺を交換して全域木を作った場合、重みは3以上になるため、中央値は変わらない。
 *   重み1または2の辺を交換して全域木を作った場合、重みはそれ以上になるため、中央値がより小さくなることは無い。
 *   よって最小全域木における辺の重みの中央値が、答となる。
 * 
 * 参考：
 *   https://atcoder.jp/contests/jag2012autumn の公式解説
 *   https://qiita.com/hidekikangeki/items/854e0a10cc4c02004ec2
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

// 辺e=(u,v)を{w(e),{u,v}}で表す
using Edge = pair<int, pair<int,int>>;

int main(void)
{
	// 0-indexed
	int i;
	int N, M;
	cin >> N >> M;
	vector<Edge> edges(M);
	for(i = 0; i < M; i++)
	{
		int u, v, w;
		cin >> u >> v >> w;
		u--;  // 0-indexedに変換
		v--;
		edges[i] = make_pair(w, make_pair(u, v));
	}

	// 辺の重みが小さい順にソート
	sort(edges.begin(), edges.end());

	// クラスカル法
	vector<int> MST_weight;  // 最小全域木の辺の重みを入れていく
	UnionFind uf(N);
	for(i = 0; i < M; i++)
	{
		int u = edges[i].second.first;
		int v = edges[i].second.second;
		int w = edges[i].first;

		// 辺を結ぶことでサイクルが発生する場合、スルー
		if(uf.issame(u, v)) continue;

		// (u,v)を繋ぐ
		MST_weight.push_back(w);  // 辺の重みを追加
		uf.unite(u, v);
	}

	// 最小全域木を構成する辺を、小さい順にソート
	sort(MST_weight.begin(), MST_weight.end());

	// 辺の数はN-1 (Nは偶数)
	// 真ん中の要素番号は(N-1)/2で求められる
	cout << MST_weight[(N-1)/2] << endl;

	return 0;
}
