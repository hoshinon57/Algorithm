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

// ABC218 https://atcoder.jp/contests/abc218

/*
 * 自力で解けず、解説を見た。
 * BFS/DFSにて「計算量をただしく見積れますか？」という観点。
 *   https://atcoder.jp/contests/abc218/editorial/2606
 *   https://ikatakos.com/pot/programming_algorithm/contest_history/atcoder/2021/0911_abc218
 * 
 * まず愚直にやるときの計算量を考えてみる。
 * 各辺ごとに「その辺を使わないグラフ」を構築してBFSすれば解けるが、
 *   辺が最大でN^2
 *   BFSの1回あたりの計算量はO(N+M) = O(M) = O(N^2)
 * より全体でO(N^4)でTLE.
 * 
 * ここで、「辺iが通れなくても答に影響ない」という辺が多そうだ、というイメージを持つ。
 * これは頂点1⇒Nの最短経路を1つ作ったときに、その経路以外の辺となる。
 * 最短経路の辺はN-1本になるので、逆に言えば、これだけはBFSをやり直したとしても
 * 全体でO(N^3)に抑えられるので十分に間に合う。
 * 
 * 以下の手順で実装する。
 * ・全ての辺が使えるときの、頂点1から各頂点への距離を求める。
 * ・最短経路を1つ求める。
 *   これは入力に対する逆順のグラフを用いて、頂点N⇒1への経路を求めていく。
 * ・各クエリについて、
 *   その辺が最短経路に含まれるなら、その辺を使わないグラフを再構築してBFS.
 *   含まれないなら、最初に求めた距離を使う。
 */

using Graph = vector<vector<int>>;
vector<int> ss, tt;

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

// 頂点(N-1)から頂点0へ辿り、使う辺をchkに設定
bool dfs(Graph &rg, int v, vector<int> &dist, vector<vector<bool>> &chk)
{
	if(v == 0) return true;

	for(auto &e : rg[v])  // 反転グラフなので、本来はe->v
	{
		if(dist[v] - dist[e] == 1)
		{
			chk[e][v] = true;
			if(dfs(rg, e, dist, chk)) return true;
			break;  // 最短経路は1つのみ復元する
		}
	}
	return false;
}

int main(void)
{
	int i, j;
	int N, M; cin >> N >> M;
	ss.resize(M);
	tt.resize(M);
	Graph g(N), rg(N);
	for(i = 0; i < M; i++)
	{
		cin >> ss[i] >> tt[i];
		ss[i]--; tt[i]--;
		g[ss[i]].push_back(tt[i]);
		rg[tt[i]].push_back(ss[i]);
	}

	vector<int> dist(N, INF32);
	bfs_graph(g, 0, dist);

	vector<vector<bool>> chk(N, vector<bool>(N, false));  // 最短経路にて2頂点を結ぶ辺を使う場合にtrue
	dfs(rg, N-1, dist, chk);  // 逆順グラフを使い、最短経路を1つ見つけて使う辺にチェックをつける

	for(i = 0; i < M; i++)
	{
		int ans;
		if(chk[ss[i]][tt[i]])  // i番目の辺を使わない
		{
			Graph g2(N);
			for(j = 0; j < M; j++)
			{
				if(i == j) continue;  // i番目の辺を使わない
				g2[ss[j]].push_back(tt[j]);
			}
			vector<int> dist2(N, INF32);
			bfs_graph(g2, 0, dist2);
			ans = dist2[N-1];
		}
		else
		{
			ans = dist[N-1];
		}
		if(ans == INF32) ans = -1;
		cout << ans << endl;
	}

	return 0;
}
