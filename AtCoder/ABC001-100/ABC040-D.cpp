#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <array>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC040 https://atcoder.jp/contests/abc040

/*
 * Union-Findで解く。
 * 
 * 道路と国民の情報をまとめて管理する。
 * 年が大きい方(未来の方)から順に
 *   道路：都市a,bを繋ぐ
 *   国民：都市vの連結成分数を答える
 * ようにすれば上手くいく。
 * 国民について「w年以前がNG」なので、同じ年なら道路より国民の方を先に処理する必要がある。
 * 詳細は実装を参照。array<int,4>を用いた。
 * 
 * [ACまでの思考の流れ]
 * ・過去から順に処理しようとすると、連結を切る作業が必要になりそう。
 * ・グラフにて辺を削除したり、連結を切ったりするのは難しい。
 *   よって逆方向に考えて辺の追加や連結追加になるようにする。[典型]
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
	int N, M; cin >> N >> M;
	vector<array<int,4>> q;
	for(i = 0; i < M; i++)
	{
		int a, b, y; cin >> a >> b >> y;
		a--; b--;
		q.push_back({y, 1, a, b});
	}
	int Q; cin >> Q;
	for(i = 0; i < Q; i++)
	{
		int v, w; cin >> v >> w;
		v--;
		q.push_back({w, 2, i, v});
	}
	sort(q.begin(), q.end(), greater<array<int,4>>());  // 降順ソート

	UnionFind uf(N);
	vector<int> ans(Q);
	for(auto [y,k,a,b] : q)
	{
		if(k == 1)  // {年, k=1, 都市a, 都市b}
		{
			uf.unite(a, b);
		}
		else  // {年, k=2, クエリ番号, 都市b}
		{
			ans[a] = uf.size(b);
		}
	}
	for(auto &e : ans) cout << e << endl;

	return 0;
}
