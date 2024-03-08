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

// 競プロ典型90問:68 https://atcoder.jp/contests/typical90/tasks/typical90_bp

/*
 * 公式解説を元に実装した。
 * クエリ先読みしつつ、隣との差分に着目して解く。
 * 
 * A2+A3=5 としたとき、A3=5-A2となる。
 * ここから、a[i]+a[i+1]=sum[i+1] としておき(T=0の入力)、
 * pot[i+1] = sum[i+1] - pot[i] をi=0から順に求めていく。(ポテンシャル)
 * ※a[i],a[i+1]で繋がらない部分も計算されるが、後の差分計算は上手くいく。
 * 
 * a[i]の値が1つ増えると、a[j]がどう変化するかを考えると、
 *   i,jの偶奇が同一  ：a[j]は1増える
 *   i,jの偶奇が異なる：a[j]は1減る
 * となる。
 * ここから、先ほどのpotと合わせてT=1のクエリに解答できる。
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
	// 1-indexed
	int i;
	int N, Q; cin >> N >> Q;
	vector<ll> t(Q), x(Q), y(Q), v(Q);
	vector<ll> sum(N+1), pot(N+1);
	for(i = 0; i < Q; i++)
	{
		cin >> t[i] >> x[i] >> y[i] >> v[i];
		if(t[i] == 0)
		{
			sum[y[i]] = v[i];
		}
	}
	for(i = 1; i <= N; i++)
	{
		pot[i] = sum[i] - pot[i-1];
	}

	UnionFind uf(N+1);
	for(i = 0; i < Q; i++)
	{
		if(t[i] == 0)
		{
			uf.unite(x[i], y[i]);
		}
		else
		{
			if(uf.issame(x[i], y[i]))
			{
				ll ret = 0;
				if((x[i]-y[i])%2 == 0)  // x,yの偶奇が同一
				{
					ret = pot[y[i]] + (v[i] - pot[x[i]]);  // v[i]が増えた分だけ、Ayも増える
				}
				else
				{
					ret = pot[y[i]] - (v[i] - pot[x[i]]);  // v[i]が増えた分だけ、Ayは減る
				}
				cout << ret << endl;
			}
			else
			{
				cout << "Ambiguous" << endl;
			}
		}
	}

	return 0;
}
