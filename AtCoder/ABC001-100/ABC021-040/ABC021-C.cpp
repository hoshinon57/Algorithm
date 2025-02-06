#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC021 https://atcoder.jp/contests/abc021

/*
 * BFSで頂点ごと最短距離を求めながら、そこへの経路(cnt)を計算していく。
 * 
 * v->eについて、頂点eが未探索であれば、cnt[e]+=cnt[v]してeをキューに積む。いつものBFS処理。
 * 頂点eが探索済みだが今見ているルートとdistが同じ、つまり最短距離で別ルートが見つかった場合、
 * cnt[e]+=cnt[v]のみ実施し、キューには積まない。
 */

#include <atcoder/modint>
using namespace atcoder;
using mint = modint1000000007;

using Graph = vector<vector<int>>;
// 頂点sを起点にBFS.
void bfs_graph(Graph &G, int s, vector<int> &dist, vector<mint> &cnt)
{
	queue<int> que;
	dist[s] = 0;
	que.push(s);
	cnt[s] = 1;

	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		for(auto &e : G[v])
		{
			if(dist[e] == dist[v]+1)  // 最短距離で別ルートがあれば、キューには積まないがcntは更新する
			{
				cnt[e] += cnt[v];
				continue;
			}
			else if(dist[e] == INF32)
			{
				dist[e] = dist[v] + 1;
				cnt[e] += cnt[v];
				que.push(e);
			}
		}
	}
	return;
}

int main(void)
{
	// 0-indexed
	int i;
	int N; cin >> N;
	int a, b; cin >> a >> b;
	a--; b--;
	Graph g(N);
	int M; cin >> M;
	for(i = 0; i < M; i++)
	{
		int x, y; cin >> x >> y;
		x--; y--;
		g[x].push_back(y);
		g[y].push_back(x);
	}

	vector<int> dist(N, INF32);
	vector<mint> cnt(N, 0);
	bfs_graph(g, a, dist, cnt);
	cout << cnt[b].val() << endl;	

	return 0;
}
