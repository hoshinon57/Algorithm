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
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC339 https://atcoder.jp/contests/abc339

/*
 * BFSで解く。
 * 
 * 2人の位置をそれぞれ状態として持つ。
 * つまり、dp[y1][x1][y2][x2]を初期状態から(y1,x1)および(y2,x2)のマスへ移動するまでの操作回数とする。
 * 後はグリッドグラフ上のBFSを通常の2次元から4次元へ拡張すればよい。
 * 
 * [ACまでの思考の流れ]
 * ・最初、プレイヤー2をプレイヤー1の方に近づけるような操作を考えたがうまく行かず。
 * ・N=60より、O(N^4)まで行けそう。
 * ・状態として{y1,x1,y2,x2}を持てば、後はグリッドグラフ上のBFSを2次元⇒4次元にしたものになりそう。
 * ・プレイヤーが同じマス(i,j)にいるのは、dist[i][j][i][j]で求められる。
 */

vector<string> mp;

void bfs_grid_graph(int H, int W, vector<int> sy, vector<int> sx, vector<vector<vector<vector<int>>>> &dist)
{
	queue<vector<int>> que;  // [0]～[3]にy1,x1,y2,x2

	dist[sy[0]][sx[0]][sy[1]][sx[1]] = 0;
	que.push({sy[0], sx[0], sy[1], sx[1]});

	const int DIR = 4;
	int dx[DIR] = {1, -1, 0, 0};
	int dy[DIR] = {0, 0, 1, -1};
	while(!que.empty())
	{
		auto d = que.front();
		int y1, x1, y2, x2;
		y1 = d[0];
		x1 = d[1];
		y2 = d[2];
		x2 = d[3];
		que.pop();
		for(int i = 0; i < DIR; i++)
		{
			int ny1 = y1 + dy[i];
			int nx1 = x1 + dx[i];
			int ny2 = y2 + dy[i];
			int nx2 = x2 + dx[i];
			if(ny1 < 0 || ny1 >= H || nx1 < 0 || nx1 >= W || mp[ny1][nx1] == '#')  // 移動先が壁なので、移動を戻す
			{
				ny1 -= dy[i];
				nx1 -= dx[i];
			}
			if(ny2 < 0 || ny2 >= H || nx2 < 0 || nx2 >= W || mp[ny2][nx2] == '#')
			{
				ny2 -= dy[i];
				nx2 -= dx[i];
			}
			if(dist[ny1][nx1][ny2][nx2] != INF32) continue;

			dist[ny1][nx1][ny2][nx2] = dist[y1][x1][y2][x2] + 1;
			que.push({ny1, nx1, ny2, nx2});
		}	
	}
	return;
}

int main(void)
{
	int i, j;
	int N; cin >> N;
	mp.resize(N);
	vector<int> x, y;
	for(i = 0; i < N; i++)
	{
		cin >> mp[i];
		for(j = 0; j < N; j++)
		{
			if(mp[i][j] == 'P')
			{
				x.push_back(j);
				y.push_back(i);
			}
		}
	}
	vector dist(N, vector(N, vector(N, vector<int>(N, INF32))));
	bfs_grid_graph(N, N, y, x, dist);

	int ans = INF32;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)  // マス(i,j)に集める
		{
			chmin(ans, dist[i][j][i][j]);
		}
	}
	if(ans == INF32) ans = -1;
	cout << ans << endl;

	return 0;
}
