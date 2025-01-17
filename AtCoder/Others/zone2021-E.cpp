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

// https://atcoder.jp/contests/zone2021/tasks/zone2021_e

/*
 * 頂点を増やすダイクストラ法で解く。
 * 
 * ダイクストラで解くのだろう、とまず考える。
 * ここで(r,c)->(r-i,c)への移動が厄介。
 * 単純に全ての辺を作ろうとすると列ごとにO(R^2)となり、全体でO(CR^2)とかになる。
 * ダイクストラの計算量はO(ElogV)なので、きっと間に合わない。  ※実装によっては間に合うらしい
 * 
 * 上方向の移動をぐっとにらむと、
 *   最初の上移動はコスト2
 *   連続の上移動にて、2回目以降ならコスト1
 * とみなせる。
 * 上移動が初回か否か、を情報に持たせるとよさそう。
 * 
 * dist[y][x][f]を
 *   座標(y,x)にいて、
 *   前回の移動が f=0:上ではない、f=1:上である
 * としたときの距離とする。
 * すると上方向の移動にて、fによってコストが決まる。
 * 頂点は2倍になるが辺はO(RC)に抑えられ、全体でO(RC*log(RC))となり間に合う。
 * 
 * 詳細は実装を参照。
 * 答はdist[R-1][C-1][0]. 答のマスは右下にあるのでf=1はありえないため。
 */

int main(void)
{
	ll i, j;
	ll R, C; cin >> R >> C;
	vector<vector<ll>> a(R, vector<ll>(C, INF64));  // a[R][C]
	for(i = 0; i < R; i++)
	{
		for(j = 0; j < C-1; j++) cin >> a[i][j];
	}
	vector<vector<ll>> b(R, vector<ll>(C, INF64));
	for(i = 0; i < R-1; i++)
	{
		for(j = 0; j < C; j++) cin >> b[i][j];
	}

	using dist_v = pair<ll, array<ll,3>>;  // {距離, {y,x,f}}
	priority_queue<dist_v, vector<dist_v>, greater<dist_v>> que;
	vector dist(R, vector(C, vector<ll>(2, INF64)));  // dist[y][x][f]
	dist[0][0][0] = 0;
	que.push({dist[0][0][0], {0, 0, 0}});

	auto upd = [&](ll ny, ll nx, ll nf, ll d_next) -> void
	{
		if(dist[ny][nx][nf] > d_next)
		{
			dist[ny][nx][nf] = d_next;
			que.push({dist[ny][nx][nf], {ny, nx, nf}});
		}
	};
	while(!que.empty())
	{
		ll d = que.top().first;
		auto [y,x,f] = que.top().second;
		que.pop();

		if(d > dist[y][x][f]) continue;

		// 右へ
		if(x < C-1) upd(y, x+1, 0, d+a[y][x]);
		// 左
		if(x > 0) upd(y, x-1, 0, d+a[y][x-1]);
		// 下
		if(y < R-1) upd(y+1, x, 0, d+b[y][x]);
		// 上
		if(y > 0)
		{
			if(f == 0) upd(y-1, x, 1, d+2);
			else upd(y-1, x, 1, d+1);
		}
	}

	cout << dist[R-1][C-1][0] << endl;

	return 0;
}
