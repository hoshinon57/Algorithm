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

// ABC218 https://atcoder.jp/contests/abc218

/*
 * 最小全域木の考え方で解く。
 * 
 * いったん全ての辺を取り除いて報酬をもらっておき、
 * 最小全域木の要領で、辺を追加しながら報酬を返していくイメージ。
 * ただしC<=0, つまり辺を取り除いたら罰金の辺は、無条件で辺を追加していく。
 * 
 * [ACまでの思考の流れ]
 * ・辺の削除は一般的に難しいので、逆方向(最後の状態から辺を追加していく)に考えてみる。
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
	ll i;
	ll N, M; cin >> N >> M;
	vector<Edge> edge(M);
	ll ans = 0;
	for(i = 0; i < M; i++)
	{
		ll a, b, c; cin >> a >> b >> c;
		a--; b--;
		edge[i] = Edge(a, b, c);
		ans += c;
	}
	// この時点でのans：全ての辺を取り除いた場合の報酬(連結性は無視して考える)
	sort(edge.begin(), edge.end());

	UnionFind uf(N);
	for(auto &e : edge)
	{
		// MSTを構築していく
		// ただし取り除いたら罰金の辺は、無条件で辺を繋いでおく
		if(e.w <= 0 || !uf.issame(e.u, e.v))
		{
			uf.unite(e.u, e.v);
			ans -= e.w;  // 「辺を取り除かない」という選択をしたので、報酬から引く
		}
	}
	cout << ans << endl;
	return 0;
}
