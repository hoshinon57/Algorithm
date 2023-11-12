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

// ABC178 https://atcoder.jp/contests/abc178

/*
 * 各点について x+y および x-y をそれぞれ考えると、
 *   x+yの最大値と最小値の差
 *   x-yの最大値と最小値の差
 * のうち大きい方が答となる。
 * 
 * [各種解説などを踏まえた考え方]
 * z=x+y, w=x-y のように座標変換すると、
 *   z方向の差の最大値 と w方向の差の最大値
 * のうち大きい方が、マンハッタン距離の最大値になる。
 * 
 * [ACまでの思考の流れ]
 * ・x+y=k となる直線を考えたときに、
 *   これが最も右上側にあるものと左下側にあるものが、マンハッタン距離の最大になりそう。
 * ・同じような直線はもう1つ x-y=k があり、
 *   これも最も右下側にあるものと左上側にあるものが、マンハッタン距離の最大になりそう。
 * ・上記2つの大きい方が答になると考えた。
 */

int main(void)
{
	int i;
	ll N; cin >> N;
	ll min1 = INF64, max1 = -INF64, min2 = INF64, max2 = -INF64;
	for(i = 0; i < N; i++)
	{
		ll x, y; cin >> x >> y;
		min1 = min(min1, x+y);
		max1 = max(max1, x+y);
		min2 = min(min2, x-y);
		max2 = max(max2, x-y);
	}
	cout << max(max1-min1, max2-min2) << endl;

	return 0;
}
