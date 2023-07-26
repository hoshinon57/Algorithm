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

// AOJ 0516 https://onlinejudge.u-aizu.ac.jp/problems/0516

/*
 * 累積和で解く。
 * 
 * 数列aの累積和a_sumを計算したときに、連続して並ぶk個の整数の和は
 * a_sum[i+k] - a_sum[i] で求められる。
 * 各iについて上記を計算し、その最大値が答。
 */

int main(void)
{
	int i;
	while(true)
	{
		int N, K; cin >> N >> K;
		if(N == 0 && K == 0) break;

		vector<int> a_sum(N+1);
		for(i = 0; i < N; i++)
		{
			int a; cin >> a;
			a_sum[i+1] = a_sum[i] + a;
		}

		int ans = -INF32;
		for(i = 0; i < N+1-K; i++)
		{
			// [i,i+K)
			ans = max(ans, a_sum[i+K] - a_sum[i]);
		}
		cout << ans << endl;
	}

	return 0;
}
