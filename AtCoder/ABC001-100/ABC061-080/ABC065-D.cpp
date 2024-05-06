#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題67より
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc065

/*
 * クラスカル法で最小全域木を構築して解く。
 * 
 * 単純にN頂点どうしで辺を作るとN^2必要となり、TLEとなる。
 * いったんx座標のみを考えたときに、
 * 頂点0から順にx座標が 0, 2, 5 であった場合、
 * 頂点0,1の辺(コスト2)、頂点1,2の辺(コスト3)の2辺で事足りるのではないか、と考えた。
 * つまり頂点0,2を結ぶ辺を作る必要は無い。
 * 
 * 同じようにy方向でも辺を作ると、辺の数は全部で 2*(N-1) 本となる。
 * これなら実行時間制限に間に合う。
 * 
 * 実装では、街(頂点)を表す構造体Cityを用意して、
 * ・x方向でソートし、辺を構築
 * ・y方向でソートし、辺を構築
 * とした後、クラスカル法で最小全域木を構築した。
 * 
 * 計算量は、ソート部分に影響してO(NlogN)かと考える。
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

// 街(頂点)を表す構造体
struct City
{
	int x, y;
	int id;  // 0-indexedで、街の番号
	City(int x_, int y_, int id_) : x(x_), y(y_), id(id_) {}
};
bool cmp_x(const City &a, const City &b) { return a.x < b.x; }
bool cmp_y(const City &a, const City &b) { return a.y < b.y; }

// 辺を表す構造体
struct Edge
{
	int s, t;  // 辺の両端の街
	int cost;  // 道を作るコスト
	Edge(int s_, int t_, int cost_) : s(s_), t(t_), cost(cost_) {}
};
bool cmp_cost(const Edge &a, const Edge &b) { return a.cost < b.cost; }

int main(void)
{
	// 0-indexed
	int i;
	int N;
	cin >> N;
	vector<City> c;
	for(i = 0; i < N; i++)
	{
		int x, y;
		cin >> x >> y;
		c.push_back({x, y, i});
	}

	vector<Edge> edge;

	// cityをx方向でソート
	sort(c.begin(), c.end(), cmp_x);
	for(i = 1; i < N; i++)
	{
		// x座標でソートされた状態で、隣の頂点と辺を構築する
		int cost = abs(c[i].x - c[i-1].x);
		// memo:以下のように、y座標も含めた実際のコストにする必要は無かった
		// int cost = min(abs(c[i].x - c[i-1].x), abs(c[i].y - c[i-1].y));
		edge.push_back({c[i-1].id, c[i].id, cost});
	}

	// 次はy方向でソート
	sort(c.begin(), c.end(), cmp_y);
	for(i = 1; i < N; i++)
	{
		int cost = abs(c[i].y - c[i-1].y);
		edge.push_back({c[i-1].id, c[i].id, cost});
	}

	// クラスカル法で最小全域木を構築し、その辺のコスト総和を求める
	sort(edge.begin(), edge.end(), cmp_cost);  // 辺をコストで昇順ソート
	UnionFind uf(N);
	ll answer = 0;
	for(auto &e : edge)  // クラスカル法
	{
		if(uf.issame(e.s, e.t)) continue;

		answer += e.cost;
		uf.unite(e.s, e.t);
	}
	cout << answer << endl;

	return 0;
}
