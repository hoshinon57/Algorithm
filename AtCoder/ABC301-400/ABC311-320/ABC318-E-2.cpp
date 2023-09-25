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

// ABC318 https://atcoder.jp/contests/abc318

/*
 * コンテスト中に解けず、解説を見た。
 * nok0氏の解説を元に解く。
 *   https://atcoder.jp/contests/abc318/editorial/7068
 * 
 * 真ん中jを固定して考えてみる。
 *   lef[x]:今見ている要素より左側にて、Ai=xとなる要素数
 *   rig[x]:同様に右側
 * と定義すると、jを固定したときの答は
 *   (x:1～N)Σ(lef[x]*rig[x]) - lef[a[j]]*rig[a[j]]
 * となる。減算部分はAi=Aj=Akとなってしまう部分を除外している。
 * 
 * Σ部分はjごとにO(N)かかりそうだが、差分に着目してみる。
 * jが1つ右に動くと、
 *   (1)lef[aj]は1つ増えるため、lef[aj]*rig[aj]は rig[aj] だけ増える。
 *   (2)rig[a(j+1)]は1つ減るため、lef[a(j+1)]*rig[a(j+1)]は lef[a(j+1)] だけ減る。
 * これがΣ部分の変化量であり、jごとにO(1)で計算できる。
 * 
 * よって、最初のjのときのΣだけ事前計算しておくことで、
 * 全体でO(N)で処理することができる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・3つ組を求める問題は、真ん中を固定してみる。
 * ・Σなど、各jごとにO(N)かかってしまいそうなときに、
 *   jが変化する際の差分に着目して、jごとにO(1)で計算できないかを考えてみる。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	vector<ll> lef(N+1), rig(N+1);  // lef[x]:今見ている要素より左側にて、Ai=xとなる要素数
	lef[a[0]]++;
	for(i = 2; i < N; i++)
	{
		rig[a[i]]++;
	}
	ll sum = 0;  // 解説におけるΣ部分
	for(i = 0; i <= N; i++)
	{
		sum += lef[i]*rig[i];
	}

	ll ans = 0;
	for(i = 1; i < N-1; i++)
	{
		ans += sum - lef[a[i]]*rig[a[i]];

		// 次iのための更新
		// iが1つ右にずれるとlef[a[i]]が1つ増えるため、lef[a[i]]*rig[a[i]]は rig[a[i]] ぶん増える
		lef[a[i]]++;
		sum += rig[a[i]];
		// iが1つ右にずれるとrig[a[i+1]]が1つ減るため、lef[a[i+1]]*rig[a[i+1]]は lef[a[i+1]] ぶん減る
		rig[a[i+1]]--;
		sum -= lef[a[i+1]];
	}
	cout << ans << endl;

	return 0;
}
