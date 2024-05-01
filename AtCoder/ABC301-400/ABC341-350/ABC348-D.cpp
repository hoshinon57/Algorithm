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

// ABC348 https://atcoder.jp/contests/abc348

/*
 * BFSで解く。2回BFSすることとなる。
 * 
 * 入力Nに対して頂点を2つ増やし、N-2:スタート、N-1:ゴールとする。
 * 各薬およびスタートマスからグリッドグラフ上のBFSを行い、
 * 距離がE以下の頂点(ゴールを含む)に対して辺を張る。
 * スタートマスにおけるEは0とする。
 * 以上が1回目のBFS.
 * 
 * 上記で作成したグラフに対して2回目のBFSを行い、スタートからゴールへたどり着ければYes解答。
 */

using Graph = vector<vector<int>>;
vector<string> mp;

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

// 高さH, 幅Wのグリッドグラフについて、(y,x)=(sy,sx)を起点にBFS.
// distは要素数[H][W]で、未探索部分はINF32で埋めていること。
// 探索結果として、distに(sy,sx)を起点(=0)とした距離を格納する。
void bfs_grid_graph(int H, int W, int sy, int sx, vector<vector<int>> &dist)
{
	queue<pair<int,int>> que;

	dist[sy][sx] = 0;
	que.push({sy, sx});

	const int DIR = 4;
	int dx[DIR] = {1, -1, 0, 0};
	int dy[DIR] = {0, 0, 1, -1};
	while(!que.empty())
	{
		int y = que.front().first;
		int x = que.front().second;
		que.pop();
		for(int i = 0; i < DIR; i++)
		{
			int ny = y + dy[i];
			int nx = x + dx[i];
			if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
			if(dist[ny][nx] != INF32) continue;

			// 壁など 問題によって書き換える
			if(mp[ny][nx] == '#') continue;

			dist[ny][nx] = dist[y][x] + 1;
			que.push({ny, nx});
		}	
	}
	return;
}

bool solve(void)
{
	int i, j;
	int H, W; cin >> H >> W;
	mp.resize(H);
	for(i = 0; i < H; i++) cin >> mp[i];
	int N; cin >> N;
	N += 2;  // 頂点を2つ増やし、N-2:スタート、N-1:ゴールとする
	vector<int> r(N), c(N), e(N);
	for(i = 0; i < N-2; i++)
	{
		cin >> r[i] >> c[i] >> e[i];
		r[i]--; c[i]--;
	}
	// スタートとゴールマスを探す
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			if(mp[i][j] == 'S')
			{
				r[N-2] = i;
				c[N-2] = j;
			}
			else if(mp[i][j] == 'T')
			{
				r[N-1] = i;
				c[N-1] = j;
			}
		}
	}

	Graph g(N);
	for(i = 0; i < N-1; i++)  // 薬およびスタートマスを始点
	{
		vector<vector<int>> dist(H, vector<int>(W, INF32));  // dist[H][W]
		bfs_grid_graph(H, W, r[i], c[i], dist);
		for(j = 0; j < N; j++)
		{
			if(i == j) continue;
			if(dist[r[j]][c[j]] <= e[i]) g[i].push_back(j);
		}
	}

	// スタート(頂点:N-2)からゴール(頂点:N-1)へたどり着けるか
	vector<int> dist(N, INF32);
	bfs_graph(g, N-2, dist);
	return (dist[N-1]!=INF32);
}

int main(void)
{
	YesNo(solve());

	return 0;
}
