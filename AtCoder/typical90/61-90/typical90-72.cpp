#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:72 https://atcoder.jp/contests/typical90/tasks/typical90_bt

/*
 * DFS/バックトラックで解く。ほぼ公式解説と同じ実装だった。
 *   https://github.com/E869120/kyopro_educational_90/blob/main/sol/072.cpp
 * 
 * スタート地点について全探索し、DFSで1手ずつ探索していく。
 * 始点に戻ってきたら、条件を満たすかをチェックした上で通ったマス数の最大値を保持する。
 * 
 * 計算量の見積もりが悩ましいが、各マスで最大3通りしか進めないので、
 * どんなに多くても各スタート地点ごとに 3^15=1.4*10^7 通りほど。
 * スタート地点はHW通りなので HW*3^(HW) 通りとしても、
 * 探索の大多数は途中で打ち切られることを考えると、十分に間に合うだろう…という感じだろうか。
 *   https://twitter.com/0x3b800001/status/1407258591308881921
 */

int H, W;
vector<string> mp;
vector<vector<bool>> seen;
int ans = -1;

void dfs(int sy, int sx, int y, int x, int cnt)
{
	// 始点に戻ってきたらマス数チェック
	if(y == sy && x == sx && cnt != 0)  // cnt=0:main()からのDFS呼び出し時
	{
		if(cnt >= 3) chmax(ans, cnt);
		return;
	}

	int i;
	const int DIR = 4;
	int dy[4] = {-1, 1, 0, 0};
	int dx[4] = {0, 0, -1, 1};
	for(i = 0; i < DIR; i++)
	{
		int ny = y+dy[i];
		int nx = x+dx[i];
		if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
		if(mp[ny][nx] == '#') continue;
		if(seen[ny][nx]) continue;

		seen[ny][nx] = true;
		dfs(sy, sx, ny, nx, cnt+1);
		seen[ny][nx] = false;  // バックトラックのため、戻す
	}
}

int main(void)
{
	int i, j;
	cin >> H >> W;
	mp.resize(H);
	for(i = 0; i < H; i++) cin >> mp[i];
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)  // マス(i,j)スタート
		{
			seen.assign(H, vector<bool>(W, false));
			dfs(i, j, i, j, 0);
		}
	}
	cout << ans << endl;

	return 0;
}
