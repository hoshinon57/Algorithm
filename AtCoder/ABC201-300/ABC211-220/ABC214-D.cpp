#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC214 https://atcoder.jp/contests/abc214

/*
 * 主客転倒とUnion-Findで解く。
 * 各種解説と同じ考え方だった。
 *   https://atcoder.jp/contests/abc214/editorial/2434
 *   https://blog.hamayanhamayan.com/entry/2021/08/15/034729
 * 
 * 頂点ごとに考えるのではなく、辺ごとにf(u,v)にて何回カウントされるかを考える。
 * 辺の重みを小さい順にソートすると、
 *   (1)---(2)     ※(x)は頂点
 * が最小のときに1回カウント。
 *   (1)---(2)---(3)
 * (2)(3)間が次に小さいとき、(1)(2)と(3)の組み合わせで2*1=2回カウント。
 * …というのを考えていくと、UnionFindで繋ぐ両端の要素数をかけたものが、f(u,v)でのカウント回数になる。
 * 
 * ans計算にてllキャストが無かったため1WA.
 * 
 * [ACまでの思考の流れ]
 * ・辺ごとにf(u,v)にて何回カウントされるかを考えるっぽい。
 * ・なら辺の重みでソートか、グラフのはじっこから順に計算とか…か？
 * ・辺の重みが大きい順に考えてみる…上手くいかない。
 * ・辺の重みが小さい順に考えてみる…UnionFindでできそう！
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
	UnionFind uf(N+1);
	vector<pair<ll, pair<int,int>>> edge(N-1);  // {w, {u,v}}
	for(i = 0; i < N-1; i++)
	{
		cin >> edge[i].second.first >> edge[i].second.second >> edge[i].first;
	}
	sort(edge.begin(), edge.end());

	ll ans = 0;
	for(auto [w,tmp] : edge)
	{
		auto [u,v] = tmp;
		ans += (ll)uf.size(u) * uf.size(v) * w;
		uf.unite(u, v);
	}
	cout << ans << endl;

	return 0;
}
