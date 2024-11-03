#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC378 https://atcoder.jp/contests/abc378

/*
 * コンテスト時に解けず、解説を見た。
 * evima氏の方法で解く。
 *   https://atcoder.jp/contests/abc378/editorial/11304
 * 
 * 次数が3の頂点をまとめて、その隣に次数が2の頂点がx個あれば、
 * その次数2の頂点同士を結ぶと条件を満たす。
 * これはx*(x-1)/2個。
 * 
 * よって以下の手順にて解く。
 *   UnionFindeで次数3の頂点をまとめる。
 *   次数が3の連結成分ごとに、隣に次数が2である頂点数をカウントする
 *   ⇒木なので、同じ連結成分で重複カウントされることは無いので、ここの判定は省ける
 *   あらためて次数が3の連結成分ごとに、前述のカウントから x(x-1)/2 の総和が答。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・入力に対して答となるグラフはどういう形になっているか？ をぐっとにらんで特徴をとらえる。
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

using Graph = vector<vector<int>>;

int main(void)
{
	int i;
	int N; cin >> N;
	UnionFind uf(N);
	Graph g(N);
	vector<int> deg(N);
	for(i = 0; i < N-1; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
		deg[u]++;
		deg[v]++;
	}

	// 次数3の頂点をまとめる
	for(i = 0; i < N; i++)
	{
		if(deg[i] != 3) continue;
		for(auto &e : g[i])
		{
			if(deg[e] == 3) uf.unite(i, e);
		}
	}

	// 次数が3の連結成分ごとに、隣に次数が2である頂点数をカウントする
	vector<ll> cnt(N);  // cnt[i]:連結成分ごと、隣に次数2の頂点の個数
	for(i = 0; i < N; i++)
	{
		if(deg[i] != 3) continue;
		int r = uf.root(i);  // 連結成分ごと
		for(auto &e : g[i])
		{
			if(deg[e] == 2) cnt[r]++;
		}
	}

	// あらためて次数が3の連結成分ごとに、前述のカウントから x(x-1)/2 の総和が答。
	ll ans = 0;
	for(auto &e : cnt)
	{
		ans += e*(e-1)/2;
	}
	cout << ans << endl;

	return 0;
}
