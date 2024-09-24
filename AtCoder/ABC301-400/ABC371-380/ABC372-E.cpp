#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC372 https://atcoder.jp/contests/abc372

// コンテスト時に考えた実装。
// 連結成分ごとの頂点集合が必要なので、UnionFindと合わせてsetを用いる。
// マージテクを用いることで、クエリ全体でO(NlogN)でsetの処理が済むので間に合う。
// …と考えたが、kの制約よりsetでなくvectorで10個まで持たせる形で良かったのね。

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

// マージテク(※)を用いて、from->toへ要素をマージする
// (※)サイズが小さい方から大きい方へマージするようにすることで、クエリ全体でO(NlogN)に抑えられる
// [注意]fromは空にはしないが、swapにより中身が変化する可能性があるため使わない方が良い
template <typename T>
void set_merge(set<T> &from, set<T> &to)
{
	if(from.size() > to.size()) swap(from, to);
	for(auto &e : from) to.insert(e);
}

int main(void)
{
	int i;
	int N, Q; cin >> N >> Q;
	UnionFind uf(N);
	vector<set<int>> s(N);  // s[i]:根が頂点iである連結成分における、頂点番号の集合
	for(i = 0; i < N; i++) s[i].insert(i);
	while(Q > 0)
	{
		Q--;
		int kind; cin >> kind;
		if(kind == 1)
		{
			int u, v; cin >> u >> v;
			u--; v--;
			if(uf.issame(u, v)) continue;
			int ru = uf.root(u), rv = uf.root(v);
			uf.unite(u, v);
			// 新たな根の方へマージ
			if(uf.root(u) == ru) set_merge(s[rv], s[ru]);
			else set_merge(s[ru], s[rv]);
		}
		else
		{
			int v, k; cin >> v >> k;
			v--;
			int ans;
			if(uf.size(v) < k)
			{
				ans = -1;
			}
			else
			{
				auto itr = s[uf.root(v)].end();
				while(k > 0)
				{
					k--;
					itr--;
				}
				ans = *itr+1;
			}
			cout << ans << endl;
		}
	}

	return 0;
}
