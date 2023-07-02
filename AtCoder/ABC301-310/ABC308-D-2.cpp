#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC308 https://atcoder.jp/contests/abc308

/*
 * DFSで解く。
 * 
 * 連想配列nxtにて、"snuke"の各文字の次が何の文字かを管理する。
 * 後は一般的なグリッドグラフ上の探索と同じ。
 */

int H, W;
vector<string> s;
vector<vector<bool>> seen;
string snuke = "snuke";
map<char, char> nxt;  // "snuke"の次の文字をmapで管理

void dfs(int y, int x)
{
	int i;
	if(seen[y][x]) return;
	seen[y][x] = true;

	int dy[4] = {1, -1, 0, 0};
	int dx[4] = {0, 0, 1, -1};
	for(i = 0; i < 4; i++)
	{
		int ny = y + dy[i];
		int nx = x + dx[i];
		if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
		if(nxt[s[y][x]] != s[ny][nx]) continue;  // "snuke"の順になっているか
		dfs(ny, nx);
	}	

	return;
}

bool solve(void)
{
	// スタートマスが's'でないケースは、ここで排除
	if(s[0][0] != snuke[0]) return false;

	dfs(0, 0);
	return seen[H-1][W-1];
}

int main(void)
{
	int i;
	cin >> H >> W;
	s.resize(H);
	for(i = 0; i < H; i++)
	{
		cin >> s[i];
	}
	seen.resize(H, vector<bool>(W, false));
	for(i = 0; i < 5; i++)
	{
		nxt[snuke[i]] = snuke[(i+1)%5];
	}

	YesNo(solve());

	return 0;
}
