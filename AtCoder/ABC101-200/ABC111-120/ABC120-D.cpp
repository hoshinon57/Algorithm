#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC120 https://atcoder.jp/contests/abc120

/*
 * ★各所の解説にて：「グラフから辺を削除していくのは難しいことが多い」
 * 
 * Union-Findで解く。
 * 
 * 1番目の橋から順に崩落することを考えるのではなく、
 * 全て崩落した状態から、1本ずつ復活していく場合を考える。
 * 
 * 全て崩落した状態では、各島は全て非連結なので、N*(N-1)/2が不便さとなる。
 * ここから最後の橋が復活した場合、橋をつなぐ島をa,bとすると、
 * ・a,bがもともと連結の場合：
 *   不便さは改善しない(変わらない)。
 * ・a,bがもともと非連結の場合：
 *   (島aと繋がる島の総数)*(島bと繋がる島の総数)
 *   だけ不便さが改善する。
 * 
 * あとは上記を実装し、出力は先頭から行えばよい。
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
	ll i;
	ll N, M;
	cin >> N >> M;
	vector<ll> a(M), b(M);
	for(i = 0; i < M; i++)
	{
		cin >> a[i] >> b[i];
		a[i]--;  // 0-indexedに変換
		b[i]--;
	}
	// ans[M-1]:M本の橋が全て崩落した後の不便さ
	// ans[0]:最初の橋が崩落した後の不便さ
	// ans[i]:i番目までの橋が崩落した後の不便さ  0-indexed
	vector<ll> ans(M);
	ans[M-1] = N*(N-1)/2;  // 全崩落時の不便さは、Combinationを使ってN_C_2となる

	UnionFind uf(N);
	// 逆方向から、1本ずつ橋を復活させていく
	for(i = M-1; i > 0; i--)  // i番目の橋を復活
	{
		// すでに連結であれば、橋を復活させても不便さは変わらない
		if(uf.issame(a[i], b[i]))
		{
			ans[i-1] = ans[i];
			continue;
		}

		// 橋を復活させた場合、(島aと繋がる島の総数)*(島bと繋がる島の総数) だけ不便さが改善する
		ll sizeA, sizeB;
		sizeA = uf.size(a[i]);
		sizeB = uf.size(b[i]);

		// i番目の橋を復活
		uf.unite(a[i], b[i]);

		// i番目の橋が復活したので、(i-1)番目までの橋が崩落したときの不便さを求められる
		ans[i-1] = ans[i] - sizeA*sizeB;
	}
	
	for(i = 0; i < M; i++)
	{
		cout << ans[i] << endl;
	}

	return 0;
}
