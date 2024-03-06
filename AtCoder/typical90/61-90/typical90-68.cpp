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

// クエリ先読みしてわちゃわちゃやれば解けた。

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
	vector<ll> sum(N+1, INF64), base(N+1);
	vector<ll> t(Q), x(Q), y(Q), v(Q);
	for(i = 0; i < Q; i++)
	{
		cin >> t[i] >> x[i] >> y[i] >> v[i];
		if(t[i] == 0)  // a[x]+a[y]=v
		{
			sum[y[i]] = v[i];
		}
	}
	for(i = 1; i <= N; i++)
	{
		if(sum[i] == INF64)
		{
			base[i] = i;
			sum[i] = 0;
		}
		else
		{
			base[i] = base[i-1];  // 前のやつ
			if(i%2 != base[i]%2)  // 偶奇が異なる
			{
				sum[i] += sum[i-1];
			}
			else
			{
				sum[i] = sum[i-1] - sum[i];
			}
		}
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
			if(!uf.issame(x[i], y[i]))
			{
				cout << "Ambiguous" << endl;
				continue;
			}
			ll abase = base[x[i]];
			ll a;
			// AxからAbaseを求める
			if(x[i]%2 != abase%2)  // 偶奇が異なる
			{
				a = sum[x[i]] - v[i];
			}
			else
			{
				a = sum[x[i]] + v[i];
			}
			// AbaseからAyを求める
			ll ay;
			if(y[i]%2 != abase%2)
			{
				ay = sum[y[i]] - a;
			}
			else
			{
				ay = -(sum[y[i]] - a);
			}
			cout << ay << endl;
		}
	}

	return 0;
}
