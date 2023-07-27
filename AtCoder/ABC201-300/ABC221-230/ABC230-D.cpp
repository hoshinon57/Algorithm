#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC230 https://atcoder.jp/contests/abc230

/*
 * 壁の右端を狙ってパンチするのが最適っぽい。
 * 壁の入力を{R座標, L座標}で受け取ってソートすることで、右端でソートされる。
 * 
 * 最後にパンチした座標をpunch_rとする。
 * ソートされた壁を見ていって、
 *   punch_r+(D-1) <= L座標 ならば、最後のパンチで一緒に壊れている。
 *   punch_r+(D-1) >  L座標 ならばまだ壊せていないので、この壁の右端を狙ってパンチする。
 */

int main(void)
{
	int i;
	int N, D; cin >> N >> D;
	vector<pair<int,int>> pos(N);  // {R座標, L座標}
	for(i = 0; i < N; i++)
	{
		cin >> pos[i].second >> pos[i].first;
	}
	sort(pos.begin(), pos.end());  // 右端でソート

	// 壁の右端を狙ってパンチする
	int ans = 0;
	ll punch_r = -INF64;  // 最後にパンチした座標
	for(auto &e : pos)
	{
		int l_pos = e.second;
		int r_pos = e.first;
		if(punch_r+(D-1) < l_pos)  // まだ壊せていない壁なので、この壁の右端を狙ってパンチする
		{
			punch_r = r_pos;
			ans++;
		}
	}
	cout << ans << endl;

	return 0;
}
