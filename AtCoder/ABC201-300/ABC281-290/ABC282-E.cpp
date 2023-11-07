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

// ABC282 https://atcoder.jp/contests/abc282

/*
 * コンテスト時に解けず、解説を見た。
 * 最小全域木の考え方で解く。
 * 
 * 公式解説やけんちょん氏の解説の通り、ボールを頂点、x^y+y^xを辺の重みとみなし、全N頂点の完全グラフを考える。
 * このグラフの最大全域木が答となる。
 *   https://atcoder.jp/contests/abc282/editorial/5398
 *   https://drken1215.hatenablog.com/entry/2022/12/29/102900
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・操作回数はN-1回になりそう。
 *   入力例を元に表や図を書いてみると、選んだボールについてループが存在したらダメっぽい。
 *   ⇒全域木っぽい。
 * ・よく出るアルゴリズムは最小全域木だが、本問は最大値を求めたい。
 *   とはいえアルゴリズムの仕組みより、最大値を選んでいく方針でも成り立ちそう。
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
	int u, v;  // 頂点
	int w;  // 辺の重み
	Edge(void) {}
	Edge(int u_, int v_, int w_) : u(u_), v(v_), w(w_) {}
	bool operator>( const Edge &right ) const
	{
		return this->w > right.w;
	}
};

// a^n (mod) を計算する
// 例えば3^5=3^1*3^4であり、5をビットごとに見ていって最下位ビットが1のときに積算する
// ※modは素数といった制約は無い
// 以下URLのコードをほぼそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modpow(long long a, long long n, long long mod)
{
	// a %= mod;  // a*aにてオーバーフローしそうな場合に有効化
	long long ans = 1;
	while(n > 0)
	{
		if((n&1) != 0)  // nの最下位ビットが立っているなら、答に掛ける
			ans = (ans * a) % mod;
		a = (a * a) % mod;
		n >>= 1;
	}
	return ans;
}

int main(void)
{
	int i, j;
	ll N, M; cin >> N >> M;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<Edge> edge;
	for(i = 0; i < N; i++)
	{
		for(j = i+1; j < N; j++)  // 2頂点(i,j)を結ぶ辺を作る
		{
			ll x = a[i], y = a[j];
			ll w = (modpow(x, y, M) + modpow(y, x, M)) % M;
			edge.push_back(Edge(i, j, w));
		}
	}
	sort(edge.begin(), edge.end(), greater<Edge>());  // 降順ソート

	ll ans = 0;
	UnionFind uf(N);
	for(auto &e : edge)
	{
		if(uf.issame(e.u, e.v)) continue;

		ans += e.w;
		uf.unite(e.u, e.v);
	}
	cout << ans << endl;

	return 0;
}
