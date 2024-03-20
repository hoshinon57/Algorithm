#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:09 https://atcoder.jp/contests/typical90/tasks/typical90_i

/*
 * 自力で解けず、解説を見た。
 * 真ん中決め打ち＋偏角ソートで解く。
 *   https://twitter.com/e869120/status/1380290146340245505
 * 
 * 基本は公式解説の通り。
 * ・真ん中のPjについて全探索する。
 * ・Pj以外の各点について、Pjを起点とした偏角を計算し、リストに積む。(※)
 * ・リストをソート。
 * ・リストの各要素について、値+180度に近いものを求める。
 *   これはlower_bound()でヒットしたものと、その1つ手前から求められる。
 * 以上の手順で解ける。
 * 
 * 自分の実装では、上記(※)にて偏角+360度の値も番兵としてリストに入れた。
 * この場合、ソートした要素の先頭(N-1)個について「値+180度に近いものを求める」必要がある。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・要素が3つある場合、真ん中を固定して考える。
 * ・偏角ソート、という考え方。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<int> x(N), y(N);
	for(i = 0; i < N; i++) cin >> x[i] >> y[i];

	// Pjjを基点としたPkkの角度を返す
	auto calc = [&](int jj, int kk) -> double
	{
		double ang = atan2(double(y[kk]-y[jj]), double(x[kk]-x[jj]));
		ang = ang*180.0/M_PI;
		return ang;
	};
	double ans = 0.0;
	for(j = 0; j < N; j++)  // Pjについて全探索
	{
		vector<double> list;
		for(i = 0; i < N; i++)
		{
			if(i == j) continue;
			double tmp = calc(j, i);
			list.push_back(tmp);
			list.push_back(tmp + 360.0);  // 番兵として、1周増やしたもの
		}
		sort(list.begin(), list.end());
		for(i = 0; i < N-1; i++)  // i=jのときは積んでいないので、先頭の(N-1)個
		{
			// lower_bound(要素の値+180度)とその1つ前
			auto itr = lower_bound(list.begin(), list.end(), list[i]+180.0);
			double tmp = *itr - list[i];
			tmp = min(tmp, 360.0-tmp);  // 180度を超える場合、360度から引いたものが度になる
			chmax(ans, tmp);

			itr--;
			tmp = *itr - list[i];
			tmp = min(tmp, 360.0-tmp);
			chmax(ans, tmp);
		}
	}
	cout << std::fixed << std::setprecision(15);
	cout << ans << endl;

	return 0;
}
