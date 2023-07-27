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

// ABC229 https://atcoder.jp/contests/abc229

/*
 * UnionFindで解く。
 * 
 * 辺や頂点の削除は難しいので、逆から見ていく。
 * つまり頂点N, N-1, ..., 1 へその頂点を復活させていくことを考える。
 * 
 * 以下のアルゴリズムで解ける。
 * ---------------------------
 * 連結成分数をansとして、最初ans=0としておく。
 * 頂点i(N,N-1,...,1)について、
 *   ansを答のリスト(ans_list)へ追加。
 *   頂点iを復活。ansを+1.
 *   頂点iと繋がる頂点eについてそれぞれ、
 *     頂点eが未復活ならばスルー。
 *     頂点i,eが異なる連結成分ならば、UnionFindで統合して、ansを-1
 * ans_listを逆順にしたものが答。
 * ---------------------------
 */
	// uf
	// 後ろから
	// 頂点iを復活
	//   ans+1
	//   for &e  頂点iからの辺
	//     i,eが非連結なら、uniteして、ans-1


using Graph = vector<vector<int>>;

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
	int N, M; cin >> N >> M;
	Graph g(N);
	UnionFind uf(N);
	for(i = 0; i < M; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}

	int ans = 0;
	vector<int> ans_list;
	vector<bool> live(N, false);  // その頂点が生きていればtrue
	for(i = N-1; i >= 0; i--)  // 後ろの頂点から復活させていく
	{
		ans_list.push_back(ans);

		// 頂点iを復活
		ans++;
		live[i] = true;

		for(auto &e : g[i])  // 頂点iと繋がる頂点e
		{
			if(!live[e]) continue;  // その頂点は生きていない
			if(!uf.issame(i, e))
			{
				ans--;
				uf.unite(i, e);
			}
		}
	}
	reverse(ans_list.begin(), ans_list.end());
	for(auto &e : ans_list)
	{
		cout << e << endl;
	}

	return 0;
}
