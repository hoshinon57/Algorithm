#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC334 https://atcoder.jp/contests/abc334

/*
 * 以下の手順で解く。
 * ・まず、緑のマスを連結成分ごとに番号を振っていく。
 * ・各赤マスについて、緑色にしたときに連結成分数がどう変化するかを考える。
 *   これは「上下左右の連結成分の番号」の種類をKとすると、K-1だけ減ることになる。(1は緑色に塗るマスの分)
 *   (種類を求めるのはsetを用いた)
 *   期待値なのでこれを赤マスの数で割り、各マスについて足していったものが答。
 */

vector<string> mp;
// 高さH, 幅Wのグリッドグラフについて、(y,x)=(sy,sx)を起点にBFS.
// distには距離ではなく連結成分の番号(=idx)を設定していく
void bfs_grid_graph(int H, int W, int sy, int sx, vector<vector<int>> &dist, int idx)
{
	queue<pair<int,int>> que;

	dist[sy][sx] = idx;
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
			if(mp[ny][nx] == '.') continue;

			dist[ny][nx] = idx;
			que.push({ny, nx});
		}	
	}
	return;
}

// mod mの世界におけるaの逆元を返す
// 以下URLのコードをそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modinv(long long a, long long m)
{
	long long b = m, u = 1, v = 0;
	while (b) {
        long long t = a / b;
        a -= t * b; swap(a, b);
        u -= t * v; swap(u, v);
    }
    u %= m; 
    if (u < 0) u += m;
    return u;
}

int main(void)
{
	ll i, j;
	ll H, W; cin >> H >> W;
	mp.resize(H);
	ll dot = 0;  // 赤色マスの数
	for(i = 0; i < H; i++)
	{
		cin >> mp[i];
		for(auto &e : mp[i]) if(e == '.') dot++;
	}
	vector<vector<int>> dist(H, vector<int>(W, INF32));
	// 連結成分ごとに番号(=group)を設定していく
	int group = 0;
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			if(mp[i][j] != '#') continue;
			if(dist[i][j] != INF32) continue;
			bfs_grid_graph(H, W, i, j, dist, group);
			group++;
		}
	}

	const int DIR = 4;
	int dx[DIR] = {1, -1, 0, 0};
	int dy[DIR] = {0, 0, 1, -1};
	ll dotinv = modinv(dot, MOD);
	ll ans = 0;
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			if(mp[i][j] != '.') continue;
			set<int> k;  // マス(i,j)の4周辺の領域番号
			for(int d = 0; d < DIR; d++)
			{
				int ny = i+dy[d];
				int nx = j+dx[d];
				if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
				if(mp[ny][nx] != '#') continue;
				k.insert(dist[ny][nx]);
			}	
			ll tmp = group-(k.size()-1);  // (i,j)を緑にしたとき、(周囲の領域の種類数)-1 だけ連結成分数が減る
			ans += tmp * dotinv % MOD;
			ans %= MOD;
		}
	}
	cout << ans << endl;

	return 0;
}
