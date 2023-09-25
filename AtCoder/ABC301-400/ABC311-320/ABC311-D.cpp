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
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC311 https://atcoder.jp/contests/abc311

/*
 * 幅優先探索で解く。
 * 
 * dist[y][x][dir]を以下のように定義する。
 *   地点{y,x}にdir方向で到達することができれば1, できなければINF
 *   ※自作のグリッドグラフのライブラリを持ってきており、
 *     求めたいのは距離ではなく到達判定であるため、1 or INF と定義した。(楽な実装にした)
 * dist[y][x][*]が1となるような要素数が答となる。
 * 
 * BFSは以下のように実装する。
 * (1)キューは{{y,x},移動方向}の情報を持たせる。
 * (2)キューから取り出したときに、まずは移動方向のまま次のマスへ移動することを考える。
 * (3)次のマスが、
 * (3-1)岩のとき：
 *      方向転換できるので、{y,x}から上下左右の4方向へ進む。
 *      来た方向に戻っても問題ない。
 * (3-2)氷のとき：
 *      そのまま次のマスへのみ進む。
 */

vector<string> mp;  // 氷/壁情報

// 高さH, 幅Wのグリッドグラフについて、(y,x)=(sy,sx)を起点にBFS.
// distは要素数[H][W]で、未探索部分はINF32で埋めていること。
void bfs_grid_graph(int H, int W, int sy, int sx, vector<vector<vector<int>>> &dist)
{
	int i;
	queue<pair<pair<int,int>,int>> que;  // {{y,x},移動方向}

	// スタート地点から上下左右方向へキュー積み
	for(i = 0; i < 4; i++)
	{
		dist[sy][sx][i] = 0;
		que.push({{sy,sx},i});
	}

	const int DIR = 4;
	int dx[DIR] = {1, -1, 0, 0};
	int dy[DIR] = {0, 0, 1, -1};
	while(!que.empty())
	{
		int y = que.front().first.first;
		int x = que.front().first.second;
		int d = que.front().second;
		que.pop();

		int ny = y + dy[d];
		int nx = x + dx[d];
		if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;

		// そのままの方向へ移動した場合に、次のマスをチェック
		if(mp[ny][nx] == '#')  // 岩
		{
			// {y,x}から上下左右の4方向へ進む
			// 来た方向に戻っても問題ない
			for(i = 0; i < 4; i++)
			{
				ny = y + dy[i];  // ny,nx計算しなおし
				nx = x + dx[i];
				// 行き先が壁、または探索済みはスルー
				if(mp[ny][nx] == '#') continue;
				if(dist[ny][nx][i] != INF32) continue;
				dist[ny][nx][i] = 1;
				que.push({{ny, nx}, i});
			}
		}
		else  // 氷
		{
			// そのまま{ny,nx}へ行く
			if(dist[ny][nx][d] != INF32) continue;  // 探索済み
			dist[ny][nx][d] = 1;
			que.push({{ny, nx}, d});
		}
	}
	return;
}

int main(void)
{
	// 0-indexed
	int i, j, k;
	int N, M; cin >> N >> M;
	mp.resize(N);
	for(i = 0; i < N; i++)
	{
		cin >> mp[i];
	}
	// dist[N][M][dir]
	vector<vector<vector<int>>> dist(N, vector<vector<int>>(M, vector<int>(4, INF32)));
	bfs_grid_graph(N, M, 1, 1, dist);

	int ans = 0;
	// dist[i][j][*]について、INF32以外のマス数が答
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < M; j++)
		{
			for(k = 0; k < 4; k++)
			{
				if(dist[i][j][k] != INF32)
				{
					ans++;
					break;
				}
			}
		}
	}
	cout << ans << endl;
	
	return 0;
}
