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

// ABC287 https://atcoder.jp/contests/abc287

/*
 * 条件を満たすのは
 * ・グラフが連結であること
 * ・頂点の次数が1である頂点が2個、2である頂点がN-2個
 * となる。
 * 
 * 連結性はUnionFindで確認し、
 * 次数は入力時に頂点ごとに管理する実装とした。
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
	int i;
	int N, M; cin >> N >> M;
	UnionFind uf(N);
	vector<int> edge(N);  // [i]:頂点iから出る辺の数
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		edge[u]++;
		edge[v]++;
		uf.unite(u, v);
	}

	if(uf.size(0) != N) return false;  // グラフが非連結はNG

	// 次数が1の頂点が2個、2の頂点がN-2個であること
	int cnt_1 = 0, cnt_2 = 0;
	for(i = 0; i < N; i++)
	{
		if(edge[i] == 1) cnt_1++;
		else if(edge[i] == 2) cnt_2++;
	}
	if(cnt_1 == 2 && cnt_2 == N-2) return true;
	else return false;
}

int main(void)
{
	YesNo(solve());

	return 0;
}
