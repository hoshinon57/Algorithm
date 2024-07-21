#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC363 https://atcoder.jp/contests/abc363

/*
 * BFSで解く。基本は公式解説と同じ方針。
 *   https://atcoder.jp/contests/abc363/editorial/10482
 * 
 * lst[y]を、y年後に沈むマスの一覧として定義する。
 * 入力時、周囲のマスについてlst[a[y][x]]を登録していく。
 * 
 * クエリyについては、ここに登録されたマスを基点にBFSしていく。
 * キューから取り出したマスについて、その周辺4マスを見て、その高さが
 *   現在の海面以下：クエリyにて沈むので、現在のキューに積む
 *   現在の海面より高い：周囲のそのマスを{ny,nx}として、a[ny][nx]年後に沈むので、そのlstに積む
 * とすればよい。
 * 
 * ansの初期値をH*Wとしておき、
 * キューから取り出して処理するごとに-1していけば、残っている面積を求められる。
 */

const int MAXA = 1e5;

int main(void)
{
	int i, j;
	int H, W, Y; cin >> H >> W >> Y;
	vector<vector<int>> a(H, vector<int>(W, 0));  // a[H][W]
	using pii = pair<int,int>;
	vector<vector<pii>> lst(MAXA+5);  // lst[y]:y年後に沈むマス{y,x}の一覧
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			cin >> a[i][j];
			if(i == 0 || i == H-1 || j == 0 || j == W-1)  // 周囲マス
			{
				lst[a[i][j]].push_back({i, j});
			}
		}
	}

	ll ans = H*W;
	vector<vector<bool>> chk(H, vector<bool>(W, false));  // chk[H][W]
	const int DIR = 4;
	int dx[DIR] = {1, -1, 0, 0};
	int dy[DIR] = {0, 0, 1, -1};
	for(i = 1; i <= Y; i++)  // クエリi
	{
		queue<pii> que;
		for(auto [y,x] : lst[i]) que.push({y, x});  // キュー初期値

		while(!que.empty())
		{
			auto [y,x] = que.front();
			que.pop();
			if(chk[y][x]) continue;
			chk[y][x] = true;
			ans--;

			for(int d = 0; d < DIR; d++)
			{
				int ny = y + dy[d];
				int nx = x + dx[d];
				if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
				if(chk[ny][nx]) continue;

				// 行き先が現在の海面以下なら、沈むので現在のキューへ
				// 海面より上なら、沈むのは将来なのでlstへ
				if(a[ny][nx] <= i) que.push({ny, nx});
				else lst[a[ny][nx]].push_back({ny, nx});
			}
		}
		cout << ans << endl;
	}

	return 0;
}
