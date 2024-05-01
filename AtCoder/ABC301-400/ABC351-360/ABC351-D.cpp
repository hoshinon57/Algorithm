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

// ABC351 https://atcoder.jp/contests/abc351

/*
 * DFSで解く。
 * (コンテスト時はUnionFindで解いた)
 * 
 * 以下の手順で解く。
 * ・磁石の隣マスをマークしておく。
 *   DFSにてこのマスに来たら探索終了となる。
 * ・「連結成分の番号」を持たせてDFS.
 *   磁石の隣マスをなら探索終了。
 *   そうでなければ、周囲4マスに「今見ている連結成分の番号」を元に探索/未探索判定を行い、再帰していく。
 *   探索できたマスすうが、その連結成分の自由度となる。
 * 
 * 「今見ている連結成分の番号」で探索/未探索判定を行うテクニック、は以下から学んだ。
 *   https://twitter.com/kyopro_friends/status/1784219334627492201
 */

int H, W;
vector<string> mp;
vector<vector<int>> chk;  // chk[y][x]:マス(y,x)をどの連結成分番号で探索したか -1は未探索
int cnt = 0;  // 現在見ている連結成分の「自由度」

const int DIR = 4;
int dx[DIR] = {1, -1, 0, 0};
int dy[DIR] = {0, 0, 1, -1};

void dfs(int y, int x, int id)
{
	chk[y][x] = id;
	cnt++;
	if(mp[y][x] == 'z') return;  // 動けないマスなので、これ以上探索しない
	for(int k = 0; k < DIR; k++)
	{
		int ny = y + dy[k];
		int nx = x + dx[k];
		if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
		// 'z'マスの存在により、
		// 「今見ている連結成分の番号」で探索/未探索判定を行うテクニック
		auto c = mp[ny][nx];
		if( (c=='.' || c=='z') && chk[ny][nx] != id)
		{
			dfs(ny, nx, id);
		}
	}
}

int main(void)
{
	int i;
	cin >> H >> W;
	chk.resize(H, vector<int>(W, -1));
	mp.resize(H);
	for(i = 0; i < H; i++) cin >> mp[i];

	// 磁石の隣マスをマーク
	int y, x;
	for(y = 0; y < H; y++)
	{
		for(x = 0; x < W; x++)
		{
			if(mp[y][x] != '.') continue;
			for(int k = 0; k < DIR; k++)
			{
				int ny = y + dy[k];
				int nx = x + dx[k];
				if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
				if(mp[ny][nx] == '#') mp[y][x] = 'z';
			}
		}
	}

	int ans = 1;
	int id = 0;  // 連結成分の番号
	for(y = 0; y < H; y++)
	{
		for(x = 0; x < W; x++)
		{
			if(mp[y][x] == '.' && chk[y][x] == -1)
			{
				cnt = 0;
				dfs(y, x, id);
				id++;
				chmax(ans, cnt);
			}
		}
	}
	cout << ans << endl;

	return 0;
}
