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

// ABC256 https://atcoder.jp/contests/abc256

/*
 * 公式解説を参考に、UnionFindで解いてみた。
 *   https://atcoder.jp/contests/abc256/editorial/4135
 * 
 * 各人iについて i->X[i] への有向辺を考える。
 * この辺を追加するとサイクルが発生するとき、サイクル内の不満度の最小値を採用すればよい。
 * このとき、iは必ずサイクル内に存在するので、iからX[i]へと順にたどっていき、またiに戻ってくるまで探索する。
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
	// 0-indexed
	int i;
	int N; cin >> N;
	UnionFind uf(N);
	vector<int> x(N); for(i = 0; i < N; i++) {cin >> x[i]; x[i]--;}
	vector<ll> c(N); for(i = 0; i < N; i++) {cin >> c[i];}

	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		// i->x[i] への有向辺
		if(uf.issame(i, x[i]))
		{
			// サイクル検出
			// iからiに戻ってくるまで探索し、最小のC[i]を採用する
			ll m = c[i];
			int nxt = x[i];
			while(nxt != i)
			{
				m = min(m, c[nxt]);
				nxt = x[nxt];
			}
			ans += m;
		}
		else
		{
			uf.unite(i, x[i]);
		}
	}
	cout << ans << endl;

	return 0;
}
