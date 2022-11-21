#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1000000007;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題76
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/nikkei2019-final/tasks/nikkei2019_final_a

/*
 * 累積和で解く。
 * 
 * k個の区画を選んだ時に、その総和をa[i]～a[i+k-1]まで1つずつ足していく場合、
 * 計算量がO(N^3)となりTLE.
 * ⇒(1)kについて1～Nまでそれぞれ調べるためO(N),
 *   (2)1つ定めたkに対して、a[0]～a[N-1]まで始点をずらしていく部分にO(N),
 *   (3)1つ定めた区画に対して総和を計算する部分にO(N).
 * 
 * そのため累積和を用いる。
 * a_sum[i+1]を、a[0]～a[i]までの累積和とする。
 * すると区画[l,r)の総和をa_sum[r]-a_sum[l]でO(1)で計算できる。(上記(3)の部分)
 * 
 * 計算量はO(N^2).
 */

int main(void)
{
	// 0-indexed
	int i;
	int N;
	cin >> N;
	vector<ll> a(N), a_sum(N+1);  // a_sum[i+1]:a[0]～a[i]までの累積和
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		a_sum[i+1] = a_sum[i] + a[i];
	}

	for(int k = 1; k <= N; k++)  // kについてそれぞれ解く
	{
		ll k_answer = 0;  // 連続するk個の区間についての最大値
		for(i = 0; i < N-k+1; i++)
		{
			// [i,i+k)の累積和をO(1)で計算
			ll sum = a_sum[i+k] - a_sum[i];
			k_answer = max(k_answer, sum);
		}
		cout << k_answer << endl;
	}

	return 0;
}
