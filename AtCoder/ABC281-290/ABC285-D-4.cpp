#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC285 https://atcoder.jp/contests/abc285

/*
 * サイクル検出ができればよいので、UnionFindで解いた版。
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

bool solve(void)
{
	int i;
	int N; cin >> N;
	UnionFind uf(N*2);  // 全ユーザがS,T異なる場合、N*2頂点となる
	map<string, int> conv;  // ユーザー名 -> 頂点番号 の対応
	int idx = 0;
	vector<pair<int,int>> edge;  // {s,t} s->tを結ぶ辺
	for(i = 0; i < N; i++)
	{
		string s, t; cin >> s >> t;
		if(conv.count(s) == 0) conv[s] = idx++;
		if(conv.count(t) == 0) conv[t] = idx++;
		edge.push_back({conv[s], conv[t]});  // s->t
	}

	for(auto &e : edge)
	{
		int v1 = e.first;
		int v2 = e.second;
		if(uf.issame(v1, v2)) return false;
		uf.unite(v1, v2);
	}

	return true;
}

int main(void)
{
	YesNo(solve());

	return 0;
}
