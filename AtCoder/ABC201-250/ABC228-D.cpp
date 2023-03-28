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

// ABC228 https://atcoder.jp/contests/abc228

/*
 * Union-Findにて解く。
 * 
 * ある数xが与えられたときに、h=xから始めて
 * A[h]!=-1であるような最小のhを、idx_list[]にて管理する。
 * すると例えばx=3のとき、A[idx_list[3]]に代入すればよい。
 * またidx_list[3]の次の値をx'とすると、
 * idx_list[x']が、その次に代入すべき値となる。
 * 
 * 以上をUnion-Findにて管理することで、t=1のクエリに対してO(α(N))で回答できる。
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
	const ll N = 1 << 20;
	vector<ll> a(N, -1);

	// idx_list[i]:iに最も近い、a[x]!=-1となるxを保持
	// 実際にはUnionFindにて uf.root(i) の形で参照する
	vector<ll> idx_list(N);
	for(i = 0; i < N; i++) idx_list[i] = i;

	UnionFind uf(N);

	ll Q;
	cin >> Q;
	while(Q > 0)
	{
		ll t, x;
		cin >> t >> x;
		if(t == 1)
		{
			ll idx = idx_list[uf.root(x%N)];
			a[idx] = x;

			// xで書き換えた右の要素と結合する
			idx = (idx+1) % N;
			ll nxt_idx = idx_list[uf.root(idx)];  // 次はa[nxt_idx]にxを設定する
			uf.unite(x%N, idx);
			idx_list[uf.root(x%N)] = nxt_idx;
		}
		else
		{
			cout << a[x%N] << endl;
		}
		Q--;
	}

	return 0;
}
