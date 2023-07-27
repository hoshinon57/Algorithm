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

// ABC157 https://atcoder.jp/contests/abc157

/*
 * UnionFind(以下UF)で解く。
 * 
 * まず友達関係の入力について、入力を元にUFで繋いでいき、
 * また各人の友達の数もカウントしておく。
 * 
 * ブロックについて一旦無視して考えると、人iの友達候補の数は、
 *   UFで頂点iと同じグループの頂点数 - 1(=人i自身) - 人iの友達の数
 * となる。
 * 
 * 次にブロックについては、人iとブロックしている人の一覧を保持しておき、
 * ブロック対象xとiがUFで同じグループに属するかを判定する。
 * 同じグループであれば答から1を引く。
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
	int N, M, K; cin >> N >> M >> K;
	UnionFind uf(N);
	vector<int> f_num(N);  // [i]:人iの友達の数
	for(i = 0; i < M; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		f_num[a]++;
		f_num[b]++;
		uf.unite(a, b);
	}
	vector<vector<int>> b_list(N);  // [i]:人iとブロックしている人の一覧
	for(i = 0; i < K; i++)
	{
		int c, d; cin >> c >> d;
		c--; d--;
		b_list[c].push_back(d);
		b_list[d].push_back(c);
	}

	for(i = 0; i < N; i++)
	{
		int ans = uf.size(i);
		ans = ans - 1 - f_num[i];  // 自身と友達の数を減らす
		for(auto &e : b_list[i])
		{
			if(uf.issame(i, e)) ans--;  // ブロックしている人が繋がっていても、友達にはならない
		}
		cout << ans << " ";
	}
	cout << endl;

	return 0;
}
