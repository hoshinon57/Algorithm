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
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC314 https://atcoder.jp/contests/abc314

/*
 * 自力で解けず、解説を見た。
 * 
 * UnionFindを用いながらグラフを構築していく。
 * ほぼ公式解説の画像の通りの実装。
 *   https://atcoder.jp/contests/abc314/editorial/6953
 * 
 * 頂点数が2N-1のグラフを考える。
 * 葉は各プレイヤーに対応し、試合ごとに親頂点へと併合していく。
 * 頂点番号0～(N-1)が初期状態のプレイヤーに対応させ、
 * N～(2N-1-1)をそれぞれi=1,2,...,N-1試合目の後に併合された頂点に対応する。
 * 
 * N以降の頂点の作り方は、次の頂点番号をcntとしたときに、
 *   cnt->(pが属する頂点番号), cnt->(qが属する頂点番号)
 * への辺を張ることになる。
 * ここをUnionFindを使って実装する。
 * このとき、各頂点にいるプレイヤー数sz[]も計算しておく。
 * 
 * 最後にDFSを用いて、頂点(2N-1-1)を根として勝ち数の期待値を加算していく。
 * ある辺について、(子のsz)/(親のsz) が勝ち数になる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・「マージ過程の木」という考え方。
 * ・何らかの要素があり(プレイヤー)、要素同士が併合されていくようなとき、
 *   葉から根の方向へグラフを構築していくテクニック。
 * ・頂点をプレイヤー、辺を繋がった頂点同士の試合と考えると、辺の重みが勝ちの期待値になる。
 *   すると期待値の線形性より、根からの辺の重みの総和が、各プレイヤーの勝ち数になる。
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

// mod mの世界におけるaの逆元を返す
// 以下URLのコードをそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modinv(long long a, long long m)
{
	long long b = m, u = 1, v = 0;
	while (b) {
        long long t = a / b;
        a -= t * b; swap(a, b);
        u -= t * v; swap(u, v);
    }
    u %= m; 
    if (u < 0) u += m;
    return u;
}

// 木であるグラフについて、頂点vを(部分木の)根としてDFS
// w:これまでの勝ち数
// win[v]:根から見たときに、頂点vの時点での勝ち数
void dfs_tree(Graph &g, int v, vector<int> &sz, ll w, vector<ll> &win, int p = -1)
{
	win[v] = w;
	for(auto &e : g[v])
	{
		if(e == p) continue;  // 親への逆流を禁止
		ll tmp = sz[e] * modinv(sz[v], MOD);  // a/(a+b)の部分
		tmp %= MOD;
		dfs_tree(g, e, sz, (w+tmp)%MOD, win, v);
	}
}

int main(void)
{
	// 0-indexed
	int i;
	int N; cin >> N;
	Graph g(2*N-1);
	UnionFind uf(N);

	int cnt = N;
	vector<int> t(N);  // プレイヤーが今属している頂点番号
	vector<int> sz(2*N-1);  // その頂点にいるプレイヤー数
	for(i = 0; i < N; i++)
	{
		t[i] = i;
		sz[i] = 1;
	}
	// 試合ごとにグラフを構築していく
	for(i = 0; i < N-1; i++)
	{
		int p, q; cin >> p >> q;
		p--; q--;
		g[cnt].push_back(t[uf.root(p)]);
		g[cnt].push_back(t[uf.root(q)]);
		uf.unite(p, q);
		t[uf.root(p)] = cnt;  // プレイヤーp(とその仲間)がいる頂点番号
		sz[cnt] = uf.size(p);
		cnt++;
	}

	vector<ll> win(2*N-1);
	dfs_tree(g, 2*N-1-1, sz, 0, win);

	for(i = 0; i < N; i++)
	{
		cout << win[i];
		if(i != N-1) cout << " ";
	}
	cout << endl;

	return 0;
}
