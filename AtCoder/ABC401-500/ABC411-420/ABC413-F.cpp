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

// ABC413 https://atcoder.jp/contests/abc413

/*
 * コンテスト時に解けず、解説を見た。
 * 多始点BFSっぽく解く。
 * 
 * ゴールに到達できるケース、できないケースがどういうものか考えてみる。
 * まずゴールが1つしかない場合、青木君が適切にジャマすることでゴールできない。
 * ゴールが2つで、2マス離れた位置にある場合、
 * これはゴールの間のマスからスタートすると、片方をジャマしてももう片方へゴールできる。
 * ゴールが2つで、3マス離れた位置にある場合、これはゴールできない。
 * 
 * などなど考えていくと、
 *   あるマスにいるときに、
 *   そこからゴールへの道筋が2通り以上あれば、ゴールできる。
 *   このときのゴールへの距離は、2番目に最短な距離。
 * となる。
 * これを元にゴールをdist=0とした多始点BFSをやると、解ける。
 * 
 * ※最初、4方向から来たときのdistをそれぞれ持つことで、distが3次元になっていた。
 *   ACはしたが実行時間が1700ms弱とかなり遅かった。
 *   distはいつも通りで2次元で、「何回目にこのマスに来たか」のcntを別に持たせることで、250msに改善した。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・どのマスから最短距離が確定していくかを考える。
**/

int main(void)
{
	// 0-indexed
	ll i, j;
	int H, W, K; cin >> H >> W >> K;
	vector<int> r(K), c(K); for(i = 0; i < K; i++) {cin >> r[i] >> c[i]; r[i]--; c[i]--;}

	vector<vector<int>> dist(H, vector<int>(W, INF32));  // dist[H][W]
	vector<vector<int>> cnt(H, vector<int>(W, 2));  // BFSでこのマスに来た回数をカウント

	auto bfs = [&](void) -> void
	{
		queue<pair<int,int>> que;

		for(i = 0; i < K; i++)
		{
			dist[r[i]][c[i]] = 0;
			cnt[r[i]][c[i]] = 0;  // 0にしておく
			que.push({r[i], c[i]});
		}
	
		const int DIR = 4;
		int dx[DIR] = {1, -1, 0, 0};
		int dy[DIR] = {0, 0, 1, -1};
		while(!que.empty())
		{
			auto [y,x] = que.front();
			que.pop();
			for(i = 0; i < DIR; i++)
			{
				int ny = y + dy[i];
				int nx = x + dx[i];
				if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;

				cnt[ny][nx]--;
				if(cnt[ny][nx] < 0) continue;
				if(cnt[ny][nx] == 0)  // 2回目にこのマスに来た
				{
					dist[ny][nx] = dist[y][x] + 1;
					que.push({ny, nx});
				}
			}	
		}
	};
	bfs();

	ll ans = 0;
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			if(dist[i][j] != INF32) ans += dist[i][j];
		}
	}
	cout << ans << endl;

	return 0;
}
