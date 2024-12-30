#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
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
 * 考え方はABC276-E-2.cppと同じ。
 * DFSで解いてみた版。
 * 
 * スタートマスの上下左右マスからそれぞれDFSし、
 * 他の上下左右マスに到達できていればOK.
 */

int H, W;
vector<string> mp;

const int DIR = 4;
int dx[DIR] = {1, -1, 0, 0};
int dy[DIR] = {0, 0, 1, -1};

void dfs(int y, int x, vector<vector<bool>> &seen)
{
	// DFS基点用にここで判定
	if(y < 0 || y >= H || x < 0 || x >= W) return;
	if(mp[y][x] == '#') return;

	if(seen[y][x]) return;
	seen[y][x] = true;

	int i;
	for(i = 0; i < DIR; i++)
	{
		int ny = y + dy[i];
		int nx = x + dx[i];
		dfs(ny, nx, seen);  // ny,nxの範囲外判定や壁判定は、関数先頭で実施
	}
}

bool solve(void)
{
	int i, j;
	cin >> H >> W;
	int sy, sx;
	mp.resize(H);
	for(i = 0; i < H; i++)
	{
		cin >> mp[i];
		auto idx = mp[i].find("S");
		if(idx != string::npos)
		{
			sy = i;
			sx = idx;
			mp[i][idx] = '#';  // 壁にしておく
		}
	}

	for(i = 0; i < DIR; i++)
	{
		int y = sy + dy[i];
		int x = sx + dx[i];
		vector<vector<bool>> seen(H, vector<bool>(W, false));
		dfs(y, x, seen);
		for(j = 0; j < DIR; j++)
		{
			if(i == j) continue;
			int gy = sy+dy[j];
			int gx = sx+dx[j];
			if(gy < 0 || gy >= H || gx < 0 || gx >= W) continue;
			if(seen[gy][gx]) return true;
		}
	}
	return false;
}

int main(void)
{
	YesNo(solve());
	return 0;
}
