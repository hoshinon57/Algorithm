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

// 「分野別 初中級者が解くべき過去問精選 100問」の問題29より
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// ABC007 https://atcoder.jp/contests/abc007

/*
 * 幅優先探索で解く。
 * 
 * いわゆる「教科書通りの実装」であり、説明は省略する。
 * 
 * 幅優先探索の計算量は O(頂点数+辺数) であることから、
 * 計算量はO(RC)となる。
 * （辺数はRCに比例し、最大でも4*RCであるため）
 */

int main(void)
{
	// 0-indexed
	int i, j;
	int R, C;
	cin >> R >> C;
	int sy, sx, gy, gx;
	cin >> sy >> sx >> gy >> gx;
	sy--; sx--; gy--; gx--;
	vector<vector<int>> mp(R, vector<int>(C));  // mp[R][C] 0なら通路、1なら壁
	for(i = 0; i < R; i++)
	{
		string str;
		cin >> str;
		for(j = 0; j < C; j++)
		{
			if(str[j] == '#') mp[i][j] = 1;  // wall
		}
	}

	int dx[] = {1, -1, 0, 0};
	int dy[] = {0, 0, 1, -1};
	queue<pair<int, int>> que;
	vector<vector<int>> dist(R, vector<int>(C, INF32));  // dist[R][C]
	que.push({sy, sx});
	dist[sy][sx] = 0;
	while(!que.empty())
	{
		int y = que.front().first;
		int x = que.front().second;
		que.pop();
		for(i = 0; i < 4; i++)
		{
			int ny = y + dy[i];
			int nx = x + dx[i];
			// 周囲は壁に囲まれているため、{ny,nx}が範囲外かの判定は不要
			if(dist[ny][nx] != INF32) continue;  // 探索済み
			if(mp[ny][nx] == 1) continue;  // wall

			dist[ny][nx] = dist[y][x]+1;
			que.push({ny, nx});
		}
	}
	cout << dist[gy][gx] << endl;

	return 0;
}
