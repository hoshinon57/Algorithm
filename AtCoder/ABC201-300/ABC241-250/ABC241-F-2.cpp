#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
#include <set>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC241 https://atcoder.jp/contests/abc241

/*
 * ABC241-F.cppにてメモしていた以下、
 * ＞丁寧にGraphを構築してからBFSしたが、Graph構築は不要だったかな。
 * ＞BFSのキューごとに、4方向を見て止まれるマスを探索してそこへ移動する、という実装で十分だったかも。
 * こちらを元に実装した内容。
 * 
 * 詳細は実装を参照。
 * 実行時間はABC241-F.cppが850ms, これが300msほどと、かなり改善した。
 */

int main(void)
{
	// 0-indexed
	// y,xを逆転して入力
	int i;
	int H, W, N; cin >> H >> W >> N;
	int sy, sx; cin >> sy >> sx; sy--; sx--;
	int gy, gx; cin >> gy >> gx; gy--; gx--;
	map<int, set<int>> gyou, retu;  // gyou[y] = {y行目にある障害物のx座標}
	for(i = 0; i < N; i++)
	{
		int y, x; cin >> y >> x;
		y--; x--;
		gyou[y].insert(x);
		retu[x].insert(y);
	}

	using pii = pair<int,int>;
	map<pii, int> dist;  // dist[{y,x}]:マス(y,x)への距離
	dist[{sy, sx}] = 0;
	queue<pii> que;
	que.push({sy, sx});

	auto chk = [&](int yy, int xx, int d) -> void
	{
		if(dist.count({yy,xx}) == 0)
		{
			dist[{yy,xx}] = d+1;
			que.push({yy,xx});
		}
	};
	while(!que.empty())
	{
		auto [y,x] = que.front();
		int d = dist[{y,x}];
		que.pop();
		int yy, xx;
		// 右
		auto itr = gyou[y].lower_bound(x);
		if(itr != gyou[y].end())
		{
			yy = y;
			xx = *itr-1;
			chk(yy, xx, d);
		}
		// 左
		if(itr != gyou[y].begin())
		{
			itr--;
			yy = y;
			xx = *itr+1;
			chk(yy, xx, d);
		}
		// 下
		itr = retu[x].lower_bound(y);
		if(itr != retu[x].end())
		{
			yy = *itr-1;
			xx = x;
			chk(yy, xx, d);
		}
		// 上
		if(itr != retu[x].begin())
		{
			itr--;
			yy = *itr+1;
			xx = x;
			chk(yy, xx, d);
		}
	}
	int ans;
	if(dist.count({gy,gx}) != 0) ans = dist[{gy,gx}];
	else ans = -1;
	cout << ans << endl;

	return 0;
}
