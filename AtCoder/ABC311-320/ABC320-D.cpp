#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC320 https://atcoder.jp/contests/abc320

/*
 * グラフで解く。
 * 入力を元に頂点(A,B)に双方向の辺を張る。
 * 人1の座標は分かっているので、そこからDFS/BFSを行って各頂点の座標を求めていく。
 * (A,B)間の距離X,Yはmapで保持した。
 * 
 * コンテスト時は座標をint型で計算してしまい2WA.
 * 制約に10^9が見えたらlong long型にすべきだった、反省。
 */

map<pair<int,int>, pair<ll,ll>>mp;  // mp[{a,b}] = {x,y}

using Graph = vector<vector<int>>;
// 頂点sを起点にBFS.
// 探索結果として、distに頂点sを起点(=0)とした距離を格納する。
void bfs_graph(Graph &G, int s, vector<pair<ll,ll>> &dist)
{
	queue<int> que;
	dist[s] = {0, 0};
	que.push(s);

	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		for(auto &e : G[v])  // v->e
		{
			if(dist[e].first != INF64) continue;
			ll nx = dist[v].first  + mp[{v, e}].first;
			ll ny = dist[v].second + mp[{v, e}].second;
			dist[e] = {nx, ny};
			que.push(e);
		}
	}
}

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int a, b;
		ll x, y;
		cin >> a >> b >> x >> y;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
		mp[{a, b}] = {x, y};
		mp[{b, a}] = {-x, -y};
	}

	vector<pair<ll,ll>> dist(N, {INF64, INF64});  // 座標{x,y}
	bfs_graph(g, 0, dist);

	for(auto &e : dist)
	{
		if(e.first == INF64)
		{
			cout << "undecidable" << endl;
		}
		else
		{
			cout << e.first << " " << e.second << endl;
		}
	}

	return 0;
}
