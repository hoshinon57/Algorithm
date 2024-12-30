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

// ABC276 https://atcoder.jp/contests/abc276

/*
 * BFSで解く。
 * 
 * 長さ4以上で、スタート地点を除き同じマスを通らないという条件は、
 * スタート地点の上下左右マスを基点にBFSし、他の上下左右マスにたどり着けたらOK. となる。
 */

using Graph = vector<vector<int>>;

vector<string> mp;

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
	int y, i, j;
	int sy, sx;
	int H, W; cin >> H >> W;
	H+=2; W+=2;  // 周囲に番兵を入れる
	mp.resize(H);
	mp[0].assign(W, '#');
	mp[H-1].assign(W, '#');
	for(y = 1; y < H-1; y++)
	{
		cin >> mp[y];
		mp[y] = "#" + mp[y] + "#";
		auto idx = mp[y].find("S");
		if(idx != string::npos)
		{
			sy = y;
			sx = idx;
			mp[sy][sx] = '#';  // スタート地点を壁にしておく
		}
	}

	const int DIR = 4;
	int dx[DIR] = {1, -1, 0, 0};
	int dy[DIR] = {0, 0, 1, -1};
	// スタート地点の上下左右マスを基点にBFS.
	// 他の上下左右マスにたどり着けたらYes.
	for(i = 0; i < DIR; i++)
	{
		vector<vector<int>> dist(H, vector<int>(W, INF32));
		// sy+dy[i], sx+dx[i]が"BFSの"スタート地点
		if(mp[sy+dy[i]][sx+dx[i]] == '#') continue;
		bfs_grid_graph(H, W, sy+dy[i], sx+dx[i], dist);
		for(j = 0; j < DIR; j++)
		{
			if(i == j) continue;
			int d = dist[sy+dy[j]][sx+dx[j]];
			if(d != INF32) return true;
		}
	}
	return false;
}

int main(void)
{
	YesNo(solve());
	return 0;
}
