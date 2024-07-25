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

// ABC170 https://atcoder.jp/contests/abc170

/*
 * ダイクストラ法で解く。
 * 以下の思考の流れ、および実装を参照。
 * 
 * [ACまでの思考の流れ]
 * ・見た目が01BFS.
 *   ただ移動回数にあたる値を {水かき回数, 今回の水かきで何マス目の移動か} とすると
 *   同じ方向への移動であっても値が変化することになる。
 *   これは01BFSで表現できるのか？ と確信が持てなかった。
 *   ⇒01BFSで解ける問題はダイクストラで解ける、という考え方により、ダイクストラに方針変更。
 * ・ダイクストラの心は {距離, 頂点番号} を優先度付きキューに持たせるということ。
 *   今回は距離:{水かき回数, 今回の水かきで何マス目の移動か}, 頂点番号{y,x,dir}となり、全5次元でデータを持たせることになる。
 */

const int DIR = 4;
int dx[DIR] = {1, -1, 0, 0};
int dy[DIR] = {0, 0, 1, -1};

int main(void)
{
	// 0-indexed
	int i;
	int H, W, K; cin >> H >> W >> K;
	int y1, x1, y2, x2; cin >> y1 >> x1 >> y2 >> x2;  // 問題文とx,y逆にした
	y1--; x1--; y2--; x2--;
	vector<string> mp(H);
	for(i = 0; i < H; i++) cin >> mp[i];

	using pii = pair<int,int>;
	vector dist(H, vector(W, vector<pii>(DIR, {INF32, INF32})));  // dist[y][x][dir]
	using dist_v = array<int, 5>;
	priority_queue<dist_v, vector<dist_v>, greater<dist_v>> que;  // {{水かき回数, 今回の水かきで何マス目の移動か}, {y,x,dir}} の5次元

	for(i = 0; i < DIR; i++)
	{
		dist[y1][x1][i] = {1, 0};
		que.push({1, 0, y1, x1, i});  // 水かき回数は1, 0マス目の移動とする
	}

	while(!que.empty())
	{
		pii d = {que.top()[0], que.top()[1]};
		int y, x, dir;
		y = que.top()[2];
		x = que.top()[3];
		dir = que.top()[4];
		que.pop();

		if(d > dist[y][x][dir]) continue;

		for(i = 0; i < DIR; i++)
		{
			int ny = y + dy[i];
			int nx = x + dx[i];
			if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
			if(mp[ny][nx] == '@') continue;
			pii d_next = d;
			if(i == dir)  // 同じ方向
			{
				if(++d_next.second > K)  // 水かき1回でKマスまで
				{
					d_next.first++;
					d_next.second = 1;
				}
			}
			else  // 方向転換
			{
				d_next.first++;
				d_next.second = 1;
			}
			if(dist[ny][nx][i] > d_next)
			{
				dist[ny][nx][i] = d_next;
				que.push({d_next.first, d_next.second, ny, nx, i});
			}
		}
	}

	int ans = INF32;
	for(i = 0; i < DIR; i++)
	{
		chmin(ans, dist[y2][x2][i].first);
	}
	if(ans == INF32) ans = -1;
	cout << ans << endl;

	return 0;
}
