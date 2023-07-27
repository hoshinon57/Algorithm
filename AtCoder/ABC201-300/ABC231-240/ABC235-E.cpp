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

// ABC235 https://atcoder.jp/contests/abc235

/*
 * 自力で解けず、解説を見た。
 * クエリ先読みの最小全域木で解く。
 * 
 * グラフGの辺およびクエリの辺をまとめて持ち、クラスカル法を進めていく。
 * 各クエリについて、
 *   クエリの辺がMSTに採用されたら、Yes (※)
 *   クエリの辺がMSTに採用されなかったら、No
 * となる。
 * (※)クエリがYesであっても、実際に木には追加しない点に注意。
 * 
 * クエリの辺か否かを判断できるよう、Edge構造体にその情報を持たせておく。
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

// MSTに使う用の辺
// 辺番号など情報を追加したい場合は、メンバ変数およびコンストラクタ、"<"演算子に手を入れること
struct Edge
{
	int u, v;  // 頂点
	int w;  // 辺の重み
	int no;  // クエリの番号(0-indexed) 入力の辺なら-1
	Edge(void) {}
	Edge(int u_, int v_, int w_, int no_) : u(u_), v(v_), w(w_), no(no_) {}
	bool operator<( const Edge &right ) const
	{
		return this->w < right.w;
	}
};

int main(void)
{
	int i;
	int N, M, Q; cin >> N >> M >> Q;
	vector<Edge> edge(M+Q);
	for(i = 0; i < M; i++)
	{
		int s, t, w; cin >> s >> t >> w;
		s--; t--;
		edge[i] = Edge(s, t, w, -1);
	}
	for(i = 0; i < Q; i++)
	{
		int s, t, w; cin >> s >> t >> w;
		s--; t--;
		edge[i+M] = Edge(s, t, w, i);
	}
	sort(edge.begin(), edge.end());  // "<"演算子を元にソート

	UnionFind uf(N);
	vector<int> ans(Q, false);
	for(i = 0; i < M+Q; i++)
	{
		auto &e = edge[i];
		if(uf.issame(e.u, e.v)) continue;
		// 以下、edge[i]をMSTに採用する

		if(e.no >= 0)  // クエリの辺
		{
			ans[e.no] = true;
			// MSTには追加しない
		}
		else  // グラフGの辺
		{
			uf.unite(e.u, e.v);
		}
	}
	for(auto &e : ans)
	{
		string s = "No";
		if(e) s = "Yes";
		cout << s << endl;
	}

	return 0;
}
