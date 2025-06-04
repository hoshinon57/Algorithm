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

// ABC405 https://atcoder.jp/contests/abc405

/*
 * 多始点BFS. 復元的な要素も含む。
 * ただ、コンテスト時はBFS後に復元処理を改めて実施して矢印を確定させていたが、
 *   (コンテスト時：https://atcoder.jp/contests/abc405/submissions/65659744)
 * "BFSしながら"矢印を確定させることも可能。
 * なぜなら、ある頂点のdistは1回しか更新されないので、
 * dist更新のタイミングで矢印が確定するため。
 * 
 * 詳細は実装を参照。
**/

vector<string> mp, ans;

void bfs_grid_graph(int H, int W, vector<vector<int>> &dist)
{
	int i, j;
	queue<pair<int,int>> que;

	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			if(mp[i][j] == 'E')
			{
				dist[i][j] = 0;
				que.push({i, j});
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
		for(i = 0; i < DIR; i++)
		{
			int ny = y + dy[i];
			int nx = x + dx[i];
			if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
			if(dist[ny][nx] != INF32) continue;

			// 壁など 問題によって書き換える
			if(mp[ny][nx] == '#') continue;

			dist[ny][nx] = dist[y][x] + 1;
			que.push({ny, nx});

			if(i == 0) ans[ny][nx] = '<';
			if(i == 1) ans[ny][nx] = '>';
			if(i == 2) ans[ny][nx] = '^';
			if(i == 3) ans[ny][nx] = 'v';
		}	
	}
	return;
}

int main(void)
{
	int i;
	int H, W; cin >> H >> W;
	mp.resize(H);
	for(i = 0; i < H; i++) cin >> mp[i];
	ans = mp;
	vector<vector<int>> dist(H, vector<int>(W, INF32));  // dist[H][W]
	bfs_grid_graph(H, W, dist);

	for(auto &e : ans) cout << e << endl;

	return 0;
}
