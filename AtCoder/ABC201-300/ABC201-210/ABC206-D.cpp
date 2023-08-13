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

// ABC206 https://atcoder.jp/contests/abc206

/*
 * 例えばA={1,3,5,1,3}のとき、
 * 回文のペアとなるA1とA5は異なる数値のため、同じ値に置き換えたい。
 * これを「値1と3は同じグループにする」と考える。
 * A2とA4は異なる値だが、先ほどの操作にて同じグループになったので、さらなる操作は不要。
 * 
 * これをUnionFindで実現すればよい。
 * 
 * (※)
 * 同じグループになる値どうしに辺を張ったグラフを考えたときに、
 * 連結成分ごとに 連結成分数-1 が操作回数となる。
 * これによりDFS/BFSなどでも解ける。
 * https://twitter.com/kyopro_friends/status/1406249601183621121
 * https://motsu-xe.hatenablog.com/entry/2021/06/19/224020
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
	// 1-indexed
	int i;
	int N; cin >> N;
	vector<int> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}
	UnionFind uf(200005);

	int ans = 0;
	for(i = 1; i <= (N+1)/2; i++)
	{
		if(!uf.issame(a[i], a[N+1-i]))  // 回文のペアが別グループであれば、統合操作
		{
			uf.unite(a[i], a[N+1-i]);
			ans++;
		}
	}
	cout << ans << endl;

	return 0;
}
