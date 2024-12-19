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

// 最小全域木のメモ、ライブラリ

/*
 * [使い方]
 * UnionFindを本ファイルまたはunion_find.cppからコピーする。
 * Edge構造体およびmain()の内容をコピーする。
 * 辺に辺番号など情報を追加したい場合は、メンバ変数およびコンストラクタ、"<"演算子に手を入れること。
 * 
 * [関連する問題]
 * AOJ GRL_2_A https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_2_A&lang=ja
 * ABC210-E MSTだが本質は数学の問題かなと
 * ABC218-E
 * ABC235-E
 * ABC270-F 超頂点使用 面白かった
 * ABC282-E
 * ABC352-E 愚直に頂点間の全辺を張るとTLEするので、クラスカル法の気持ちを踏まえて必要な辺のみを張る
 * ABC364-F MSTそのものではないが、「MSTの気持ち」を要求される
 * ABC383-E MSTそのものではないが、近い考え方
 * 典型90-49
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
	Edge(void) {}
	Edge(int u_, int v_, int w_) : u(u_), v(v_), w(w_) {}
	bool operator<( const Edge &right ) const
	{
		return this->w < right.w;
	}
};

int main(void)
{
	// 以下はAOJのGRL_2_Aを解く内容
	// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_2_A&lang=ja
	int i;
	int N, E; cin >> N >> E;
	vector<Edge> edge;
	for(i = 0; i < E; i++)
	{
		int s, t, w; cin >> s >> t >> w;
		// s--; t--;  // 必要に応じて
		edge.push_back({s, t, w});
	}
	sort(edge.begin(), edge.end());  // "<"演算子を元にソート

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
