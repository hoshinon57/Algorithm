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

// ABC182 https://atcoder.jp/contests/abc182

/*
 * 累積和および累積和のMAXの考え方で解く。
 * 
 * まず、A1からAiまでの移動を「動作i」とする。
 * 動作iまで終わった後の位置を last[i] とすると、Aの累積和を用いて
 *   last[i] = last[i-1] + a_sum[i]
 * となる。(今回、Aiまでの累積和をa_sum[i]としている)
 * 
 * 動作i-1から動作iの間の最大値は
 *   last[i-1] + max(a_sum[1],a_sum[2],..,a_sum[i])
 * と、累積和の最大値を使って求めることができる。
 * これを各iについて求めていけば、その最大値が答。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> a_sum(N);  // a_sum[i]:a[1]～a[i]までの累積和
	for(i = 0; i < N; i++)
	{
		if(i == 0) a_sum[i] = a[i];
		else a_sum[i] = a_sum[i-1] + a[i];
	}
	ll a_sum_max = 0;  // 累積和の中の最大値
	ll ans = 0;
	vector<ll> last(N);  // last[i]:A1～Aiまで進むフェーズの、終端位置
	for(i = 0; i < N; i++)
	{
		chmax(a_sum_max, a_sum[i]);  // これまでの累積和の最大
		if(i == 0)
		{
			chmax(ans, a_sum_max);
			last[i] = a_sum[i];
		}
		else
		{
			chmax(ans, last[i-1] + a_sum_max);
			last[i] = last[i-1] + a_sum[i];
		}
	}
	cout << ans << endl;

	return 0;
}
