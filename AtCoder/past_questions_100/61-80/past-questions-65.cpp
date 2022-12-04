#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題65
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joisc2010/tasks/joisc2010_finals

/*
 * クラスカル法で最小全域木を構築して解く。
 * 
 * まず開催都市が1つのとき、「一度に複数人を通行させても料金は同じ」という条件から
 * 開催する都市はどこでも同じではないか、と考えた。
 * つまり最小全域木の辺の総和が答。
 * 
 * 次に開催都市Kが複数のとき。
 * 例としてK=2で、以下のような最小全域木が求められたとする。
 *   o-(3)-o-(10)-o-(1)-o    ※oは都市、数値はコスト
 * このとき、コスト10の辺を使わず、その両端に開催都市を用意するのが最適と考える。
 * K=3以上も同様。
 * 
 * よってK>=2の場合においては、
 *   まず最小全域木を求めて、
 *   辺のコストが大きい方から(K-1)個を無視したときの総和が答
 * となる。
 * (K=1も同じ方針として良い)
 * 
 * 計算量はO(MlogM).
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
	int a, b;  // 都市
	int c;  // 料金
	Edge(int a_, int b_, int c_) : a(a_), b(b_), c(c_) {}
};
bool cmp(const Edge &a, const Edge &b)
{
	return a.c < b.c;
}

int main(void)
{
	int i;
	int N, M, K;
	cin >> N >> M >> K;
	UnionFind uf(N);
	vector<Edge> e;
	for(i = 0; i < M; i++)
	{
		int a, b, c;
		cin >> a >> b >> c;
		a--;  // 0-indexedに変換
		b--;
		e.push_back(Edge(a, b, c));
	}
	sort(e.begin(), e.end(), cmp);

	// 最小全域木を作り、そのコストをroadに入れていく
	vector<int> road;
	for(i = 0; i < M; i++)
	{
		if(uf.issame(e[i].a, e[i].b)) continue;

		uf.unite(e[i].a, e[i].b);
		road.push_back(e[i].c);
	}
	// roadの要素は昇順ソートされた状態になっている

	// 例えば開催都市が2つなら、最小全域木のうちコスト最大のものを「コスト0にする」と置き換えられる。
	// よって (K-1) 個無視してコストの総和を求めれば、それが答になる。
	ll answer = 0;
	for(i = 0; i < (int)road.size() - (K-1); i++)
	{
		answer += road[i];
	}
	cout << answer << endl;

	return 0;
}
