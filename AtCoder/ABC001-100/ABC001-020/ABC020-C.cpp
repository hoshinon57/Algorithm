#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
#include <array>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC020 https://atcoder.jp/contests/abc020

/*
 * 二分探索＆グリッド上のダイクストラ。
 * 
 * 二分探索部分について、制約より初期値をok=1として良い。
 * また同じく制約の「1度は黒マスを通る」より、初期値をng=INFとして良い。
 */

int main(void)
{
	ll i, j;
	ll H, W, T; cin >> H >> W >> T;
	vector<string> mp(H);
	int sx, sy, gx, gy;
	for(i = 0; i < H; i++)
	{
		cin >> mp[i];
		for(j = 0; j < W; j++)
		{
			if(mp[i][j] == 'S')
			{
				sy = i;
				sx = j;
			}
			else if(mp[i][j] == 'G')
			{
				gy = i;
				gx = j;
			}
		}
	}

	const int DIR = 4;
	int dx[DIR] = {1, -1, 0, 0};
	int dy[DIR] = {0, 0, 1, -1};
	auto check = [&](ll t) -> bool
	{
		vector<vector<ll>> dist(H, vector<ll>(W, INF64));  // dist[H][W]
		dist[sy][sx] = 0;
		using dist_v = array<ll, 3>;  // {dist, y, x}
		priority_queue<dist_v, vector<dist_v>, greater<dist_v>> que;
		que.push({0, sy, sx});

		while(!que.empty())
		{
			auto [d, y, x] = que.top();
			que.pop();

			if(d > dist[y][x]) continue;

			for(i = 0; i < DIR; i++)
			{
				ll ny = y + dy[i];
				ll nx = x + dx[i];
				if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;

				ll nd = d;
				if(mp[ny][nx] == '#') nd += t;
				else nd++;
				if(dist[ny][nx] > nd)
				{
					dist[ny][nx] = nd;
					que.push({dist[ny][nx], ny, nx});
				}
			}
		}
		return dist[gy][gx] <= T;
	};
	ll ok = 1, ng = INF64;
	while(ng - ok > 1)
	{
		ll mid = (ok+ng)/2;
		if(check(mid)) ok = mid;
		else ng = mid;
	}
	cout << ok << endl;

	return 0;
}
