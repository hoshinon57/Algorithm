#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC226 https://atcoder.jp/contests/abc226

/*
 * Union-Findで解く。
 * 
 * 【考えたこと、思考の流れ】
 * 試しにグラフをいくつか作ってみる。
 * 頂点数をv, 辺数をmとしたときに、ある連結成分について
 *   v>mのとき：条件を満たせない(必要な辺が足りない)
 *   v<mのとき：条件を満たせない(ジャマな辺がある)
 * となり、v=mである必要があるっぽい。
 * 
 * ではv=mのときに、条件を満たす向きの付け方はどう決まるかを試したところ、
 * どうもループ部分にのみ、2通り決められるっぽい。
 * ループ以外の部分は1通りしか決められない。
 * 
 * そうすると、v=mのとき、必ずループは1つだけできるので、
 * 連結成分ごとに見ていって、
 *   v!=mのとき：答は0
 *   v=mのとき：答に2をかける
 * とすれば良さそう。
 * 
 * 以上をUnion-Findを用いて実装した。
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

int N, M;
vector<int> edge_num;  // [i]:頂点iの辺数
vector<int> total;  // [i]:連結成分ごとに、辺の総数をUnionFindのrootを使って管理 1つの辺を2回カウントするため、2倍の値が設定される
ll answer;

ll solve(UnionFind &uf)
{
	answer = 1;

	for(int i = 0; i < N; i++)
	{
		if(i != uf.root(i)) continue;  // 連結成分ごと

		// 連結成分の頂点数と辺の数が等しいか
		// 1つの辺を2回カウントしているため、2で割る
		if(uf.size(i) == total[i]/2)
		{
			answer *= 2;
			answer %= MOD;
		}
		else
		{
			// 条件を満たす辺の付け方は存在しない
			return 0;
		}
	}
	return answer;
}

int main(void)
{
	// 0-indexed
	int i;
	cin >> N >> M;
	UnionFind uf(N);
	edge_num.resize(N);
	total.resize(N);
	for(i = 0; i < M; i++)
	{
		int u, v;
		cin >> u >> v;
		u--; v--;
		edge_num[u]++;
		edge_num[v]++;
		uf.unite(u, v);
	}

	// 連結成分ごとの辺の総数total[]を計算する
	for(i = 0; i < N; i++)
	{
		total[uf.root(i)] += edge_num[i];
	}

	cout << solve(uf) << endl;

	return 0;
}
