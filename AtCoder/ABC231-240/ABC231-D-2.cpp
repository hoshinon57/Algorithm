#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;
using Graph = vector<vector<int>>;

// ABC231 https://atcoder.jp/contests/abc231

/*
 * Union-Findで解く。
 * 
 * 隣り合っている人どうしを統合していく。
 * このとき、統合前にすでに同じグループであった場合、サイクルが発生することになる。
 * サイクルは横一列という条件を満たさなくなるのでNG.
 * 
 * 同じく横一列のため、入力にて3回以上同じ人が登場した場合もNGとなる。
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
	// 0-indexedに変換して解く
	int i;
	int N, M;
	cin >> N >> M;
	Graph graph(N);
	vector<int> a(M), b(M);
	vector<int> count(N);  // 各人にて、入力に登場した数
	for(i = 0; i < M; i++)
	{
		cin >> a[i] >> b[i];
		a[i]--;  // 0-indexedに変換
		b[i]--;
		count[a[i]]++;
		count[b[i]]++;
	}

	// UnionFindにて統合していく。
	// 統合前にすでに同じグループであった場合、サイクルが発生することになる。
	// この場合、横一列という条件を満たさなくなるのでNG.
	UnionFind uf(N);
	for(i = 0; i < M; i++)
	{
		if(uf.issame(a[i], b[i]))
		{
			cout << "No" << endl;
			return 0;
		}
		uf.unite(a[i], b[i]);
	}

	// 入力にて3回以上登場した人がいればNG(横一列を満たさなくなるため)
	for(i = 0; i < N; i++)
	{
		if(count[i] >= 3)
		{
			cout << "No" << endl;
			return 0;
		}
	}

	cout << "Yes" << endl;

	return 0;
}
