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

// ABC308 https://atcoder.jp/contests/abc308

/*
 * BFSで解く。
 * スタートマス(0, 0)からの各マスへの距離と、"snuke"の文字番号が一致するかを見ながらBFSする。
 * スタートマスが's'でないケースだけ、BFSする前にチェックしておく。
 */

bool solve(void)
{
	int i;
	int H, W; cin >> H >> W;
	vector<string> s(H); for(i = 0; i < H; i++) {cin >> s[i];}

	string snuke = "snuke";
	vector<vector<int>> dist(H, vector<int>(W, INF32));  // dist[H][W]
	queue<pair<int,int>> que;

	// スタートマスが's'でないケースは、ここで排除
	if(s[0][0] != snuke[0]) return false;

	dist[0][0] = 0;
	que.push({0, 0});

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

			// "snuke"の順になっているか
			if(snuke[(dist[y][x]+1)%5] != s[ny][nx]) continue;

			dist[ny][nx] = dist[y][x] + 1;
			que.push({ny, nx});
		}
	}

	if(dist[H-1][W-1] != INF32) return true;
	else return false;
}

int main(void)
{
	YesNo(solve());

	return 0;
}
