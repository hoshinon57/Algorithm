#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
#include <set>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC385 https://atcoder.jp/contests/abc385

/*
 * ABC385-D.cppに対して、左右ないし上下に動くものを関数にまとめたもの。
 * X,Y方向もまとめられるが、コンテスト時にそこまでやるとバグったときが大変かな…という気もする。
 *   https://atcoder.jp/contests/abc385/submissions/61001590
 */

int main(void)
{
	ll i;
	ll N, M, sx, sy; cin >> N >> M >> sx >> sy;
	map<ll, set<ll>> x_ie, y_ie;  // x_ie[x] = {y,y,y,...}  x座標について、家があるy座標の一覧
	for(i = 0; i < N; i++)
	{
		ll x, y; cin >> x >> y;
		x_ie[x].insert(y);
		y_ie[y].insert(x);
	}
	ll ans = 0;
	// Y方向にfrom->toまで動くとき、通る家を消していく
	auto del_y = [&](ll x, ll from, ll to) -> void
	{
		if(from > to) swap(from, to);  // from<toにする
		auto itr = x_ie[x].lower_bound(from);
		while(itr != x_ie[x].end())
		{
			ll y = *itr;
			if(y > to) break;
			ans++;
			// X,Y方向それぞれについて、家(x,y)を消す
			itr = x_ie[x].erase(itr);
			y_ie[y].erase(x);
		}
	};
	// X方向について同様
	auto del_x = [&](ll y, ll from, ll to) -> void
	{
		if(from > to) swap(from, to);  // from<toにする
		auto itr = y_ie[y].lower_bound(from);
		while(itr != y_ie[y].end())
		{
			ll x = *itr;
			if(x > to) break;
			ans++;
			// X,Y方向それぞれについて、家(x,y)を消す
			itr = y_ie[y].erase(itr);
			x_ie[x].erase(y);
		}
	};

	for(i = 0; i < M; i++)
	{
		char d; cin >> d;
		ll c; cin >> c;
		if(d == 'U')  // sx,sy -> sx,sy+c
		{
			del_y(sx, sy, sy+c);
			sy += c;
		}
		else if(d == 'D')  // yの下方向を見る  sx,sy -> sx,sy-c
		{
			del_y(sx, sy, sy-c);
			sy -= c;
		}
		else if(d == 'R')  // xの右方向  sx,sy -> sx+c,sy
		{
			del_x(sy, sx, sx+c);
			sx += c;
		}
		else // 'L'  sx,sy -> sx-c,sy
		{
			del_x(sy, sx, sx-c);
			sx -= c;
		}
	}
	cout << sx << " " << sy << " " << ans << endl;

	return 0;
}
