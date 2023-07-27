#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC267 https://atcoder.jp/contests/abc267

/*
 * 累積和で解く。
 * 
 * 愚直に全ての部分文字列を計算すると、計算量がO(MN)となりTLE.
 * 
 * 例としてM=4で考えてみると、先頭2つの部分文字列は以下になる。
 * A1*1 + A2*2 + A3*3 + A4*4
 *        A2*1 + A3*2 + A4*3 + A5*4
 * よって上記を順にX1,X2とすると、
 *   X2=X1-(A1+A2+A3+A4)+A5*4
 * となる。
 * これにより、「次の部分文字列」を計算量O(1)で求めることができる。
 * 
 * よって、
 * ・A1～Aiの累積和
 * ・最初の部分文字列A1+A2+...+AM に対するΣの値
 * だけ事前に求めて、残りの部分文字列を順番に計算していけばよい。
 */

int main(void)
{
	ll i;
	ll N, M;
	cin >> N >> M;
	vector<ll> a(N);  // 0-indexed
	vector<ll> a_sum(N+1);  // a_sum[i]:a[0]～a[i-1]までの累積和 i:0～N
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		a_sum[i+1] = a_sum[i] + a[i];
	}

	// まず、最初の部分文字列に対するΣの計算をしておく
	ll now_sum = 0;
	for(i = 0; i < M; i++)
	{
		now_sum += (i+1)*a[i];
	}
	ll answer = now_sum;

	// 次の部分文字列に対するΣの値は、以下で求められる
	//   now_sum - (sum[i+M]-sum[i]) + a[i+M]*M
	for(i = 0; i < N-M; i++)  // ループ回数はN-M回
	{
		now_sum = now_sum - (a_sum[i+M] - a_sum[i]) + a[i+M]*M;
		answer = max(answer, now_sum);
	}
	cout << answer << endl;

	return 0;
}
