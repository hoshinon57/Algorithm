#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC254 https://atcoder.jp/contests/abc254

/*
 * 参考：https://twitter.com/kyopro_friends/status/1533082728446320640
 * 
 * iについて全探索する。
 * iを平方数で割っていったときに、残った値が最小のjになる。
 *   例：i=12(=2*2*3)のとき、平方数で割った残りが3となり、これが最小のj
 * また、最小のjに対して平方数をかけたものも、jとしての条件を満たす。
 * 
 * 計算量は、iの全探索がO(N)で、
 * 平方数で割ったりかけたりする部分がO(√N)となり、全体でO(N*√N)になると思われる。
 */
int main(void)
{
	ll answer = 0;
	ll i, k;
	ll N;
	cin >> N;
	// iについて全探索
	for(i = 1; i <= N; i++)
	{
		ll div_i = i;
		// 平方数で割れるだけ割る
		for(k = 2; k*k <= i; k++)
		{
			while(div_i%(k*k) == 0) div_i /= k*k;
		}
		ll min_j = div_i;  // 残った値が最小のjとなる
		
		// 最小のjに対して平方数をかけたものも、jの条件を満たす
		for(k = 1; min_j*k*k <= N; k++)
		{
			answer++;
		}
	}

	cout << answer << endl;

	return 0;
}
