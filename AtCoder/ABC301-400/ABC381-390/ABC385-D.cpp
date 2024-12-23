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
 * 家の座標をX,Y方向それぞれについてsetで管理するやつ。
 * 類題：ABC370-D(Cross Explosion)
 * 座標が±10^9と大きいので、mapとsetの重ね技で管理する。
 * 
 * (x,y)->(x,y+C)へ移動なら、x_ie[x]について[y,y+C]の区間にある家を消していく。
 * その際y_ie[]の情報も消す。
 * (x,y)->(x,y-C)への移動なら、[y-C,y]という区間で考えると良い。
 * 
 * 実装が大変。
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
	for(i = 0; i < M; i++)
	{
		char d; cin >> d;
		ll c; cin >> c;
		ll x, y;  // 見つけた家の座標
		if(d == 'U')  // sx,sy -> sx,sy+c
		{
			auto itr = x_ie[sx].lower_bound(sy);
			while(itr != x_ie[sx].end())  // sy～sy+cの区間にある家を消していく
			{
				x = sx;
				y = *itr;
				if(y <= sy+c) {;}
				else break;
				ans++;
				// X,Y方向それぞれについて、家(x,y)を消す
				y_ie[y].erase(x);
				itr = x_ie[sx].erase(itr);
			}
			sy += c;
		}
		else if(d == 'D')  // yの下方向を見る  sx,sy -> sx,sy-c
		{
			auto itr = x_ie[sx].lower_bound(sy-c);
			while(itr != x_ie[sx].end())  // sy～sy-cの区間にある家を消していく 逆方向に見て[sy-c,sy]の区間で見る
			{
				x = sx;
				y = *itr;
				if(y <= sy) {;}
				else break;
				ans++;
				y_ie[y].erase(x);
				itr = x_ie[sx].erase(itr);
			}
			sy -= c;
		}
		else if(d == 'R')  // xの右方向  sx,sy -> sx+c,sy
		{
			auto itr = y_ie[sy].lower_bound(sx);
			while(itr != y_ie[sy].end())
			{
				x = *itr;
				y = sy;
				if(x <= sx+c) {;}
				else break;
				ans++;
				x_ie[x].erase(y);
				itr = y_ie[sy].erase(itr);
			}
			sx += c;
		}
		else // 'L'  sx,sy -> sx-c,sy
		{
			auto itr = y_ie[sy].lower_bound(sx-c);
			while(itr != y_ie[sy].end())
			{
				x = *itr;
				y = sy;
				if(x <= sx) {;}
				else break;
				ans++;
				x_ie[x].erase(y);
				itr = y_ie[sy].erase(itr);
			}
			sx -= c;
		}
	}
	cout << sx << " " << sy << " " << ans << endl;

	return 0;
}
