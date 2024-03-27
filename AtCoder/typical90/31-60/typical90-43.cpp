#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
#include <array>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:43 https://atcoder.jp/contests/typical90/tasks/typical90_aq

/*
 * 0-1 BFSで解く。公式解説とほぼ同じ考え方、実装だった。
 *   https://twitter.com/e869120/status/1394787605099601923
 *   https://github.com/E869120/kyopro_educational_90/blob/main/sol/043.cpp
 * 
 * 状態として {y,x,今向いている方向} の3種を持たせる。
 * 同じ方向に進むならコスト0, 方向を変えるならコスト1として0-1BFSを行えばよい。
 * 
 * ※dequeにはarray<int,3>の形で持たせたところ、ローカル環境では#include <array>が無くてもコンパイル通るのに対し、
 *   AtCoder環境ではCEとなった。
 *
 * dist[gy][gx][*]の最小値が答。
 */

int H, W;
vector<string> mp;
int sy, sx, gy, gx;  // スタートとゴール

void bfs(vector<vector<vector<int>>> &dist)
{
	int i;
	deque<array<int,3>> que;  // {y,x,dir}
	vector seen(H, vector(W, vector<bool>(4, false)));

	const int DIR = 4;
	int dx[DIR] = {-1, 1, 0, 0};
	int dy[DIR] = {0, 0, -1, 1};
	// 初期状態をキューに積む (y,x)で4方向
	for(i = 0; i < DIR; i++)
	{
		dist[sy][sx][i] = 0;
		que.push_back({sy, sx, i});
	}
	while(!que.empty())
	{
		auto [y,x,d] = que.front();
		que.pop_front();
		int di = dist[y][x][d];
		
		if(seen[y][x][d]) continue;
		seen[y][x][d] = true;

		for(i = 0; i < DIR; i++)
		{
			int ny = y+dy[i];
			int nx = x+dx[i];
			if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
			if(mp[ny][nx]=='#') continue;
			if(i == d)  // 同じ方向 コスト0
			{
				if(dist[ny][nx][i] > di)
				{
					dist[ny][nx][i] = di;
					que.push_front({ny, nx, i});
				}
			}
			else  // 方向変える コスト1
			{
				if(dist[ny][nx][i] > di + 1)
				{
					dist[ny][nx][i] = di + 1;
					que.push_back({ny, nx, i});
				}
			}
		}
	}
}

int main(void)
{
	int i;
	cin >> H >> W;
	cin >> sy >> sx >> gy >> gx;
	sy--; sx--; gy--; gx--;
	mp.resize(H);
	for(i = 0; i < H; i++) cin >> mp[i];

	vector dist(H, vector(W, vector<int>(4, INF32)));
	bfs(dist);
	int ans = INF32;
	for(i = 0; i < 4; i++)
	{
		chmin(ans, dist[gy][gx][i]);
	}
	cout << ans << endl;

	return 0;
}
