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

// ABC018 https://atcoder.jp/contests/abc018

/*
 * 多始点BFSで解いた。
 * 
 * 各マスごとにひし形を作ろうとすると、O(RCK^2)とかで間に合わない。
 * ひし形の中央を基点に考えるのではなく、xマスを基点に「ここはひし形の中央にできるか」を考える。
 * xからの距離がK未満なら、そこはひし形の中央にできない。
 * ⇒xがたくさんあり、多始点BFS.
**/

vector<string> mp;

// distは要素数[H][W]で、未探索部分はINF32で埋めていること。
void bfs_grid_graph(int H, int W, vector<vector<int>> &dist)
{
	queue<pair<int,int>> que;

	// 多始点BFS
	{
		int y, x;
		for(y = 0; y < H; y++)
		{
			for(x = 0; x < W; x++)
			{
				if(mp[y][x] != 'x') continue;
				dist[y][x] = 0;
				que.push({y, x});
			}
		}
	}

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
			// if(mp[ny][nx] == '#') continue;

			dist[ny][nx] = dist[y][x] + 1;
			que.push({ny, nx});
		}	
	}
	return;
}

int main(void)
{
	int i;
	int R, C, K; cin >> R >> C >> K;
	mp.resize(R);
	for(i = 0; i < R; i++) cin >> mp[i];
	vector<vector<int>> dist(R, vector<int>(C, INF32));  // dist[R][C]
	bfs_grid_graph(R, C, dist);

	int ans = 0;
	for(int y = 0; y < R; y++)
	{
		for(int x = 0; x < C; x++)
		{
			if(mp[y][x] == 'x') continue;
			if(y < K-1 || x < K-1 || y > R-K || x > C-K) continue;  // はみ出る
			if(dist[y][x] < K) continue;
			ans++;
		}
	}
	cout << ans << endl;

	return 0;
}
