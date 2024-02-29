#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:54 https://atcoder.jp/contests/typical90/tasks/typical90_bb

/*
 * 超頂点によって辺を減らす考え方で解く。
 * 
 * 各人を頂点にし、共著となる人同士に辺を張ってみる。
 * すると人1から各人への最短距離が求める値となる。
 * 
 * ただし共著ごとに、K(K-1)の辺が必要となる。
 * N人全員の共著であればN^2レベルの辺が必要となり、愚直にやるとTLE.
 * 
 * そこで"共著"という超頂点Xを用意し、Xと研究者rとの間に双方向の辺を張る。
 * すると共著ごとにK本の辺に削減できる。
 * 人1からBFSし、各人への最短距離を2で割ったものが答となる。
 * 
 * 実装では0-indexedとし、
 *   頂点0～N-1：人
 *   頂点N～N+M-1：共著(超頂点)
 * としている。
 */

using Graph = vector<vector<int>>;

// 頂点sを起点にBFS.
// distは未探索部分をINF32で埋めていること。
// 探索結果として、distに頂点sを起点(=0)とした距離を格納する。
void bfs_graph(Graph &G, int s, vector<int> &dist)
{
	queue<int> que;
	dist[s] = 0;
	que.push(s);

	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		for(auto &e : G[v])
		{
			if(dist[e] != INF32) continue;
			dist[e] = dist[v] + 1;
			que.push(e);
		}
	}
	return;
}

int main(void)
{
	// 0-indexed
	int i, j;
	int N, M; cin >> N >> M;
	Graph g(N+M);  // 先頭N個が人、後半M個が共著(超頂点)
	for(i = 0; i < M; i++)  // 共著i 頂点i+N
	{
		int K; cin >> K;
		for(j = 0; j < K; j++)
		{
			int r; cin >> r;  // 人r
			r--;
			g[i+N].push_back(r);
			g[r].push_back(i+N);
		}
	}

	vector<int> dist(N+M, INF32);
	bfs_graph(g, 0, dist);
	for(i = 0; i < N; i++)
	{
		int ans = -1;
		if(dist[i] != INF32) ans = dist[i] / 2;
		cout << ans << endl;
	}

	return 0;
}
