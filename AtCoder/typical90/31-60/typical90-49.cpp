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

// 競プロ典型90問:49 https://atcoder.jp/contests/typical90/tasks/typical90_aw

/*
 * 最小全域木で解く。
 * 
 * 各文字について、その文字単体だけ0/1切り替えられる必要があるっぽいと分かる。
 * 試行錯誤してみたところ、区間[l,r+1)を反転させる行為を、頂点(l,r+1)に辺を張る行為に置き換えたとき、
 * 頂点(i,i+1)に辺が張られていれば、文字iは独立で0/1切り替えられる模様。
 * 
 * よって全ての頂点iについて上記を満たすようにしたい。
 * つまり頂点1～頂点N+1まで連結にすることであり、つまり最小全域木を求める問題になる。
 * 
 * ※公式解説の、区間[l,r)の反転を、
 *   隣り合う要素の差分をd[]としたときに、d[l]とd[r]を反転させるという趣旨は理解できた。
 *   ただそこから2^(N+1)通りとかの話は理解できなかった。
 *   https://twitter.com/e869120/status/1397322487554199553
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・区間[l,r)を反転させる問題は、頂点(l,r)に辺を張る問題に置き換えてみる。
 *   類題：ABC238-E(Range Sums)
 * ・区間[l,r)を反転させる
 *   ⇒隣り合う要素の差分d[]を考えたときに、d[l]とd[r]を反転させる
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
	// 0-indexed
	int i;
	int N, M; cin >> N >> M;
	vector<Edge> edge(M);
	for(i = 0; i < M; i++)
	{
		int c, l, r; cin >> c >> l >> r;
		l--; r--;
		edge[i] = {l, r+1, c};
	}
	sort(edge.begin(), edge.end());

	ll ans = 0;
	UnionFind uf(N+1);
	for(auto &e : edge)
	{
		if(uf.issame(e.u, e.v)) continue;
		ans += e.w;
		uf.unite(e.u, e.v);
	}
	// 頂点0～NのN+1個が連結していればtrue
	auto check = [&](void) -> bool
	{
		bool ok = true;
		for(i = 0; i < N; i++)
		{
			if(!uf.issame(i, i+1)) ok = false;
		}
		return ok;
	};
	if(!check()) ans = -1;
	cout << ans << endl;
	
	return 0;
}
