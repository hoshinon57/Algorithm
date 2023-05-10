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

// ABC238 https://atcoder.jp/contests/abc238

/*
 * 自力で解けず、解説を見た。
 * 
 * 当初考えたこと：
 * 各[l,r]について区間を+1する累積和やいもす法を考えたときに、区間の偶奇を見て解けないか。
 * ⇒解けなかった。
 * 
 * 解法：
 * 入力l,rを[l,r+1)の半開区間で考えると、
 * 例えば[1,4), [2,4)の情報を得ると[1,2)が求められる。
 * 求めたいのは[0,N).
 * 
 * 頂点1-4および2-4がそれぞれ接続されていれば、頂点1-2も4経由で辿れるという考えから、
 * 頂点l-(r+1)を辺で接続していって、
 * 最終的に頂点0とN+1が同じ連結成分に属しているかで判定できる。
 * 
 * 実装にはUnion-Findを用いた。
 * 
 * 反省：
 * ある区間の情報を持っているときに、
 * それを頂点とみなして辺で接続するという考えを今後持つようにしたい。
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

bool solve(void)
{
	// 0-indexed
	int i;
	int N, Q;
	cin >> N >> Q;
	UnionFind uf(N+1);  // 頂点数N+1を考える
	for(i = 0; i < Q; i++)
	{
		// 入力l=1,r=2 -> a1+a2 -> [0,2) -> 頂点0,2を結ぶ
		int l, r;
		cin >> l >> r;
		l--; r--;  // 0-indexedへの変換
		r++;  // 半開区間への変換
		uf.unite(l, r);
	}

	// a1+a2+...+an -> [0,N)
	return uf.issame(0, N);
}

int main(void)
{
	YesNo(solve());

	return 0;
}
