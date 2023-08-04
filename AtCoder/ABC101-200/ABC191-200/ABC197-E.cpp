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

// ABC197 https://atcoder.jp/contests/abc197

/*
 * 動的計画法で解く。
 * 
 * まず、どういうルートでボールを取るのが最善かを考える。
 * 広義単調増加という条件があるので、「色1のボールを全部取る」、「色2のボールを全部取る」、…
 * という順序となる。
 * では色iのボール一式についてを考えると、
 *   (1)色iのボールのうち左端のものをまず取り、そこから右端に向かって取っていく
 *   (2)色iのボールのうち右端のものをまず取り、そこから左端に向かって取っていく
 * のどちらかが最善になるだろう、と考えた。
 * (1)(2)どちらが最短になるかは、前後の色のボールに依存しそうと考えると、
 * dp[i][j]を次のように定義すればよい。
 *   色iのボールまでを取り終わったときに、
 *   自分が色iのボールの j=0:左端, j=1:右端 にいるときの、トータルの移動量
 * 
 * 色iのボールのうち、左端の座標をpos_l[i], 同じく右端をpos_r[i]とすると、
 * dp[i][0]は
 *   色(i-1)を左端で取り終わった後、色iの右端に行き、そこから色iの左端へ行く
 *   色(i-1)を右端で取り終わった後、色iの右端に行き、そこから色iの左端へ行く
 * のうち小さい方となる。dp[i][1]も同様。
 * 色iのボールがない場合は、dpおよびpos_l,rにi-1の要素をコピーすればよい。
 * 
 * 最後に、dp[N][0], dp[N][1]からそれぞれ座標0に戻るまでの距離を加算して、小さい方が答。
 */

int main(void)
{
	// 1-indexed
	int i;
	int N; cin >> N;
	vector<ll> pos_l(N+1, INF64), pos_r(N+1, -INF64);  // pos_l[i]:色iのボールのうち、左端の座標
	for(i = 1; i <= N; i++)
	{
		ll x, c; cin >> x >> c;
		pos_l[c] = min(pos_l[c], x);
		pos_r[c] = max(pos_r[c], x);
	}
	pos_l[0] = pos_r[0] = 0;
	vector<vector<ll>> dp(N+1, vector<ll>(2, 0));  // dp[N+1][2]

	ll d1, d2;
	for(i = 1; i <= N; i++)
	{
		if(pos_l[i] == INF64)  // 色iのボールが無い
		{
			dp[i][0] = dp[i-1][0];
			dp[i][1] = dp[i-1][1];
			pos_l[i] = pos_l[i-1];
			pos_r[i] = pos_r[i-1];
			continue;
		}
		// 左端
		d1 = dp[i-1][0] + abs(pos_r[i] - pos_l[i-1]) + (pos_r[i] - pos_l[i]);  // 左端から行く場合
		d2 = dp[i-1][1] + abs(pos_r[i] - pos_r[i-1]) + (pos_r[i] - pos_l[i]);  // 右端から行く場合
		dp[i][0] = min(d1, d2);
		// 右端
		d1 = dp[i-1][0] + abs(pos_l[i] - pos_l[i-1]) + (pos_r[i] - pos_l[i]);  // 左端から行く場合
		d2 = dp[i-1][1] + abs(pos_l[i] - pos_r[i-1]) + (pos_r[i] - pos_l[i]);  // 右端から行く場合
		dp[i][1] = min(d1, d2);
	}

	d1 = dp[N][0] + abs(pos_l[N]);  // 最後のボールを左端にて回収したのち、座標0へ
	d2 = dp[N][1] + abs(pos_r[N]);  // 右端のケース
	cout << min(d1, d2) << endl;

	return 0;
}
