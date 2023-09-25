#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC304 https://atcoder.jp/contests/abc304

/*
 * UnionFindで解く。
 * kyopro_friends氏の解説と同じ考え方。
 *   https://twitter.com/kyopro_friends/status/1665004188445687809
 * 
 * 例えばグラフが
 *   1-2  3-4-5
 * の2つの連結成分があり、
 * 良いグラフの条件として x=1, y=3 とする。
 * すると2-4や2-5の辺も追加すると「良いグラフ」ではなくなる。
 * よって連結成分でまとめて考えてしまえばよい。
 * 
 * UnionFindで連結成分ごとにまとめ、
 * x,yについてUnionFindの根のペアをsetで管理する。
 * そして各クエリについて、p,qの根のペアが前述のsetに含まれているか否かで判断すればよい。
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
	// {xの親, yの親}をsetで管理 -> good_list
	// {pの親, qの親}がgood_listに存在する場合、Noとなる
	int i;
	int N, M;
	cin >> N >> M;
	UnionFind uf(N);
	for(i = 0; i < M; i++)
	{
		int u, v;
		cin >> u >> v;
		u--; v--;
		uf.unite(u, v);
	}
	set<pair<int,int>> good_list;  // 良いグラフの条件「頂点x,y」のペアを登録
	int K;
	cin >> K;
	for(i = 0; i < K; i++)
	{
		int x, y;
		cin >> x >> y;
		x--; y--;
		good_list.insert({uf.root(x), uf.root(y)});
		good_list.insert({uf.root(y), uf.root(x)});  // 大小関係があるので、{x,y},{y,x}どちらも登録しておく
	}

	int Q;
	cin >> Q;
	while(Q > 0)
	{
		Q--;
		int p, q;
		cin >> p >> q;
		p--; q--;

		YesNo(good_list.count({uf.root(p), uf.root(q)}) == 0);
	}

	return 0;
}
