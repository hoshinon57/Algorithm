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

// ABC215 https://atcoder.jp/contests/abc215

/*
 * 自力で解けず、解説を見た。
 * 二分探索＆平面走査で解く。
 *   https://x.com/kyopro_friends/status/1429080812134100997
 *   https://atcoder.jp/contests/abc215/editorial/2492
 * 
 * 以下、|xi-xj|=dx, 同じくyをdyと書く。
 * また、入力の頂点をX方向でソートしておく。
 * 
 * 基本は公式解説の通り。
 * 最小値の最大化なので二分探索を考えてみる。
 * min(dx,dy)>=KとなるKが存在するか？という部分問題を考えると、Kを増やしていったときに
 *   true, true, ..., true, false, false, ...
 * とどこかでt/fが切り替わる。
 * 
 * 部分問題については、dx>=Kかつdy>=Kを満たす頂点対が見つかればよいので、
 * ・x座標が小さい頂点から見ていって、x方向の差がK以上になった点を取り出し、yのmax,minを保持していく。
 * ・y座標とmax,minの差がm以上である点があれば、true
 * という手順でキューを用いて解ける。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・最小値の最大化なので二分探索を考えてみる。[典型]
 * ・そこから平面走査へたどり着くには…
 *   kyopro_friends氏のツイートより、平面上のイメージを作って、平面走査かなあ。
 */

int main(void)
{
	using pii = pair<int,int>;
	int i;
	int N; cin >> N;
	vector<pii> xy(N);
	for(i = 0; i < N; i++) cin >> xy[i].first >> xy[i].second;
	sort(xy.begin(), xy.end());  // x座標でソート

	// 距離の最大値がm以上ならtrue
	auto check = [&](int m) -> bool
	{
		queue<pii> que;
		for(i = 0; i < N; i++) que.push({xy[i].first, xy[i].second});  // x座標でソート済み
		int miny = INF32, maxy = -1;
		// x座標が小さい頂点から見ていって、x方向の差がm以上になった点を取り出してyのmax,minを保持していく。
		// y座標とmax,minの差がm以上である点があれば、true
		for(auto [px, py] : xy)
		{
			while(!que.empty())
			{
				if(px - que.front().first >= m)
				{
					int ty = que.front().second;
					chmin(miny, ty);
					chmax(maxy, ty);
					que.pop();
				}
				else break;
			}
			if(py-miny >= m || maxy-py >= m) return true;
		}
		return false;
	};

	int l, r;
	l = 0;
	r = INF32;
	while(r - l > 1)
	{
		int mid = (l+r)/2;
		if(check(mid)) l = mid;
		else r = mid;
	}
	cout << l << endl;

	return 0;
}
