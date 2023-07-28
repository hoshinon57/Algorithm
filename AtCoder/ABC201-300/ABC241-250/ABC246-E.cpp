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

// ABC246 https://atcoder.jp/contests/abc246

/*
 * 0-1 BFSで解く。
 * 
 * ビショップは同じ方向なら何マスでも一気に進めるが、
 * これを頂点(x,y)から(x+a, y+a)の最大N通りについて辺を張ってしまうと、
 * 辺数がO(N^3)になりTLEとなってしまう。
 * よって1マスずつ考えることとする。
 * 
 * スタートから各頂点への最短手数をdistとしたときに、
 * ここに座標と「どの方向から来たか」も次元に追加する。dist[N][N][4]のイメージ。
 * (1)同じ方向に進むときは、dist[y'][x'][d]  に dist[y][x][d]と同じ値を設定し、
 * (2)別の方向に進むときは、dist[y'][x'][d'] に dist[y][x][d]+1を設定する。
 * これは両端キューを使って、(1)はキューの先頭、(2)はキューの末尾に追加する。
 * 
 * ゴールの座標を(gy,gx)とすると、
 * dist[gy][gx][0～3]のうち最小値が答。
 */

vector<string> mp;

void bfs01_grid_graph(int H, int W, int sy, int sx, vector<vector<vector<int>>> &dist)
{
	int i;
	deque<pair<pair<int,int>,int>> que;  // {{y,x},dir}
	vector seen(H, vector(W, vector<int>(4, false)));  // seen[N][N][dir] 処理済みならtrue

	const int DIR = 4;
	for(i = 0; i < DIR; i++)
	{
		dist[sy][sx][i] = 0;
		que.push_front({{sy, sx}, i});
	}

	int dx[DIR] = {1, 1, -1, -1};
	int dy[DIR] = {1, -1, 1, -1};  // ナナメ方向
	while(!que.empty())
	{
		int y = que.front().first.first;
		int x = que.front().first.second;
		int d = que.front().second;
		que.pop_front();

		// 同じ頂点が「コスト0のケース」と「コスト1のケース」で2回キューに積まれることがあるため、ここで処理を1回に抑える
		// ただしこの処理は無くてもACする
		// あった方が無駄な探索が減るので実行時間は早くなる
		if(seen[y][x][d]) continue;
		seen[y][x][d] = true;

		for(i = 0; i < DIR; i++)
		{
			int ny = y + dy[i];
			int nx = x + dx[i];
			if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
			// 壁など 問題によって書き換える
			if(mp[ny][nx] == '#') continue;

			if(i == d)  // 同じ方向 コスト0
			{
				// 距離を更新できるなら、更新してキューに積む
				if(dist[ny][nx][i] > dist[y][x][d])
				{
					dist[ny][nx][i] = dist[y][x][d];
					que.push_front({{ny, nx}, i});  // コスト0は先頭に追加
				}
			}
			else  // 異なる方向 コスト1
			{
				// 距離を更新できるなら、更新してキューに積む
				if(dist[ny][nx][i] > dist[y][x][d] + 1)
				{
					dist[ny][nx][i] = dist[y][x][d] + 1;
					que.push_back({{ny, nx}, i});  // コスト1は末尾に追加
				}
			}
		}
	}
	return;
}

int main(void)
{
	// 0-indexed
	int i;
	int N; cin >> N;
	int sy, sx; cin >> sy >> sx;  // スタート、ゴールマスのxy表記が感覚と逆なのが分かりづらい…
	sy--; sx--;
	int gy, gx; cin >> gy >> gx;
	gy--; gx--;
	mp.resize(N);
	for(i = 0; i < N; i++)
	{
		cin >> mp[i];
	}
	vector dist(N, vector(N, vector<int>(4, INF32)));  // dist[N][N][dir]
	bfs01_grid_graph(N, N, sy, sx, dist);

	int ans = INF32;
	for(i = 0; i < 4; i++)
	{
		ans = min(ans, dist[gy][gx][i]);
	}
	if(ans == INF32) ans = -1;
	else ans++;  // 初手が0手目になっているので、+1
	cout << ans << endl;

	return 0;
}
