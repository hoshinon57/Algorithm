#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC075 https://atcoder.jp/contests/abc075

/*
 * Union-Findで解く。
 * 
 * M本の辺についてそれぞれ、i番目の辺が無いものとしてUnion-Findを用いる。
 * 連結の個数が1以外のときグラフは非連結と判断できるので、i番目の辺は橋となる。
 * 連結の個数は、「根が自分自身」となる頂点を数えればよい。
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
	int i, j;
	int N, M;
	cin >> N >> M;
	vector<int> a(M), b(M);
	for(i = 0; i < M; i++)
	{
		cin >> a[i] >> b[i];
		a[i]--;  // 0-indexedに変換
		b[i]--;
	}

	int bridgeNum = 0;
	for(i = 0; i < M; i++)  // i番目の辺が無いものとしてUnion-Findを用いる
	{
		UnionFind uf(N);
		for(j = 0; j < M; j++)
		{
			if(i == j) continue;  // i番目の辺は無し
			uf.unite(a[j], b[j]);
		}

		int rootNum = 0;
		// 連結の個数を求める
		for(j = 0; j < N; j++)
		{
			if(uf.root(j) == j) rootNum++;  // 「根が自分自身」となる頂点の数が、グラフ全体の連結の個数になる
		}
		if(rootNum != 1) bridgeNum++;  // i番目の辺は橋である
	}

	cout << bridgeNum << endl;

	return 0;
}
