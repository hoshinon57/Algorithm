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

// ABC317 https://atcoder.jp/contests/abc317

/*
 * BFSで解く。実装問題寄りか。
 * 
 * 人およびその視線について考える。
 * 視線の範囲を別途chk[y][x]に保持するようにし、各人から伸ばしていく。
 * 壁および人にて視線は途切れるので、そこで探索を打ち切ることで
 * 視線チェックは全体でO(HW)とかO(N*max(H,W))とかそんな感じ？で収まる。
 * 全ての視線チェックが終わったら、chk[y][x]=trueのマスは壁にしてしまう。
 * 
 * 後はいつものBFSで最短距離を求める。
 * 
 * [ACまでの思考の流れ]
 * ・視線の対応をどうするか。
 * ・各人について愚直に視線を伸ばすと、人の配置によってはTLEしないか？
 *   ⇒視線は人で途切れるので、そこで探索を打ち切れば計算量は間に合いそう。
 */

vector<vector<int>> mp;

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
//			if(mp[ny][nx] == '#') continue;
			if(mp[ny][nx] != 0) continue;

			dist[ny][nx] = dist[y][x] + 1;
			que.push({ny, nx});
		}	
	}
	return;
}

int main(void)
{
	int i, j;
	int H, W; cin >> H >> W;
	mp.resize(H, vector<int>(W, 0));
	int sx, sy, gx, gy;
	for(i = 0; i < H; i++)
	{
		string s; cin >> s;
		// ., #, >, v, <, ^, S, G 
		for(j = 0; j < W; j++)
		{
			switch(s[j])
			{
			case '#':
				mp[i][j] = 1;
				break;
			case '>':
				mp[i][j] = 2;
				break;
			case 'v':
				mp[i][j] = 3;
				break;
			case '<':
				mp[i][j] = 4;
				break;
			case '^':
				mp[i][j] = 5;
				break;
			case 'S':
				sy = i; sx = j;
				break;
			case 'G':
				gy = i; gx = j;
				break;
			}
		}
	}

	vector<vector<bool>> chk(H, vector<bool>(W, false));  // chk[H][W]
	const int DIR = 4;
	int dy[DIR] = {0, 1, 0, -1};
	int dx[DIR] = {1, 0, -1, 0};
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			if(mp[i][j] >= 2)
			{
				int d = mp[i][j] - 2;
				int y = i, x = j;
				while(true)
				{
					y += dy[d];
					x += dx[d];
					if(y < 0 || y >= H || x < 0 || x >= W) break;
					if(mp[y][x] != 0) break;
					chk[y][x] = true;
				}
			}
		}
	}
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			if(chk[i][j]) mp[i][j] = 1;
		}
	}

	vector<vector<int>> dist(H, vector<int>(W, INF32));  // dist[H][W]
	bfs_grid_graph(H, W, sy, sx, dist);
	int ans = -1;
	if(dist[gy][gx] != INF32) ans = dist[gy][gx];
	cout << ans << endl;

	return 0;
}
