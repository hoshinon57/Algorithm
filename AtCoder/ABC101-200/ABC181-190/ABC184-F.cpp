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

// ABC184 https://atcoder.jp/contests/abc184

/*
 * 半分全列挙で解く。
 * ※N個の中から自由に選ぶ：2^N通り
 *   Nの最大は40であり、2^40は間に合わない。
 *   ⇒2^20なら間に合うので、2分割する
 * 
 * まず、数列Aを前半後半に分け、
 * それぞれについて自由に選ぶときの総和を、bit全探索で全列挙する。
 * また、このうち片方をソートしておく。
 * 
 * ソートしていない方について、ある値をa_sum[i]とすると、
 *   a_sum[i] + x <= T
 * であるxのうち最大のものを探したい。
 *   x <= T - a_sum[i]
 * であるため、upper_bound()でT-a_sum[i]の1つ前の要素を探せばよい。
 * a_sum[i]+xの最大値が答。
 * ※実装ではx側の要素に -∞ を番兵として入れた
 */

// 要素aから0個以上を選ぶ全ての選び方について、その和をa_sumに格納する
void sum(vector<ll> &a, vector<ll> &a_sum)
{
	int i, j;
	int n = (int)a.size();
	for(i = 0; i < 1<<n; i++)
	{
		ll sum = 0;
		for(j = 0; j < n; j++)
		{
			if( ((i>>j)&1) == 0 ) continue;
			sum += a[j];
		}
		a_sum.push_back(sum);
	}
}

int main(void)
{
	// 0-indexed
	ll i;
	ll N, T; cin >> N >> T;
	vector<ll> a1, a2;  // [0,N/2) と [N/2,N) に分ける
	for(i = 0; i < N; i++)
	{
		ll a; cin >> a;
		if(i < N/2) a1.push_back(a);
		else a2.push_back(a);
	}
	vector<ll> a1_sum, a2_sum;
	sum(a1, a1_sum);
	sum(a2, a2_sum);
	a2_sum.push_back(-INF64);  // 番兵
	sort(a2_sum.begin(), a2_sum.end());

	ll ans = 0;
	for(i = 0; i < (int)a1_sum.size(); i++)
	{
		// 番兵があるため不要  if(a1_sum[i] > T) continue;
		auto itr = upper_bound(a2_sum.begin(), a2_sum.end(), T-a1_sum[i]);
		itr--;
		ans = max(ans, a1_sum[i]+(*itr));
	}
	cout << ans << endl;

	return 0;
}
