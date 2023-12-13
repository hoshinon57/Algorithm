#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1e9+7;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC211 https://atcoder.jp/contests/abc211

/*
 * BFSのライブラリに少し手を加える。
 * cnt:その頂点へ最短距離で行ける経路数 とし、頂点v->eへの移動を考えたときに、
 * ・頂点eが未探索：cnt[e] = cnt[v]
 * ・頂点eが探索済みだが、同じく最短経路：cnt[e] += cnt[v]
 * とすればよい。
 */

using Graph = vector<vector<int>>;
// 頂点sを起点にBFS.
// distは未探索部分をINF32で埋めていること。
// 探索結果として、distに頂点sを起点(=0)とした距離を格納する。
void bfs_graph(Graph &G, int s, vector<int> &dist, vector<ll> &cnt)  // cnt:その頂点へ最短距離で行ける経路数
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
			if(dist[e] == INF32)
			{
				dist[e] = dist[v] + 1;
				cnt[e] = cnt[v];
				que.push(e);
			}
			else if(dist[e] == dist[v] + 1)  // 別ルートでも最短距離で行ける場合
			{
				cnt[e] += cnt[v];
				cnt[e] %= MOD;
			}
		}
	}
	return;
}

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	vector<int> dist(N, INF32);
	vector<ll> cnt(N, 0);
	bfs_graph(g, 0, dist, cnt);
	cout << cnt[N-1] << endl;

	return 0;
}
