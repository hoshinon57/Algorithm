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

// ABC223 https://atcoder.jp/contests/abc223

/*
 * 二分探索で解く。
 * 左右両端からt秒燃やしたときに、
 *   燃えている点が交差しない
 *   燃えている点が交差する
 * がどこかの時刻で切り替わる。
 * これを二分探索で求めればよい。
 * 
 * ※公式解説の
 * 「左側からのみ火をつけたときに全部燃えるまでの時刻の半分が、答となる時刻になるので、
 *   左側からその時刻ぶん燃やしたときの地点が答」
 * の方がスマートな解法だった…
 */
int main(void)
{
	int i, j;
	int N; cin >> N;
	double a_sum = 0;  // 導火線全体の長さ
	vector<double> a(N), b(N); for(i = 0; i < N; i++) {cin >> a[i] >> b[i]; a_sum += a[i];}

	double l = 0.0;
	double r = INF32;
	double ans = 0.0;
	for(i = 0; i < 100; i++)
	{
		// t秒燃やしたときに、左右端からそれぞれ何cm燃えるかを計算し、
		// 左右からの火がぶつからない場合にtrueを返す
		// また、左からの火が何cmのところにあるかを、ansに保存する(答の出力用)
		auto check = [&](double t) -> bool
		{
			double t_left = t;  // 燃やす残り時間
			double l_pos = 0;  // 左からの火が何cmのところにあるか
			double r_pos = a_sum;

			// 左側からの火
			for(j = 0; j < N; j++)  // 導火線j
			{
				if(a[j]/b[j] < t_left)  // 導火線jを全て燃やしても、時間はまだ余る
				{
					t_left -= a[j]/b[j];
					l_pos += a[j];
				}
				else  // 導火線jが燃えている間に時間切れ
				{
					l_pos += t_left*b[j];
					break;
				}
			}
			// 右側からの火
			t_left = t;
			for(j = N-1; j >= 0; j--)
			{
				if(a[j]/b[j] < t_left)
				{
					t_left -= a[j]/b[j];
					r_pos -= a[j];
				}
				else
				{
					r_pos -= t_left*b[j];
					break;
				}
			}
			ans = l_pos;  // 答出力用
			return l_pos < r_pos;
		};
		double mid = (l+r)/2;
		if(check(mid)) l = mid;
		else r = mid;
	}
	cout << std::fixed << std::setprecision(15);
	cout << ans << endl;

	return 0;
}
