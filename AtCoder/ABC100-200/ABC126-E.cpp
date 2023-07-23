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

// ABC126 https://atcoder.jp/contests/abc126

/*
 * Union-Findで解く。
 * 
 * 与えられたX,Yについて、この2頂点間に辺を張っていくことを考える。
 * ここで魔法にてXまたはYのどちらかが分かると、もう片方も確定する。
 * つまりグラフの連結成分について、何らか1つが分かれば、芋づる式に残りも全て確定する。
 * 
 * よってグラフの連結成分数がそのまま答となる。
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
	int i;
	int N, M; cin >> N >> M;
	UnionFind uf(N);
	for(i = 0; i < M; i++)
	{
		int x, y, z; cin >> x >> y >> z;
		x--; y--;
		uf.unite(x, y);
	}

	int ans = 0;
	for(i = 0; i < N; i++)
	{
		if(uf.root(i) == i) ans++;
	}
	cout << ans << endl;

	return 0;
}
