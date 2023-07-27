#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC265 https://atcoder.jp/contests/abc265

/*
 * 二分探索と累積和を使って解く。
 * ABC265-D-2.cppではsetを使ったが、本ソースコードではbinary_search()を使う。
 * 
 * xについて全探索し、かつ累積和を事前に求めておくのはABC265-D-2.cppと同様。
 * 二分探索にてbinary_search()を使うのが変化点。
 * A(i)>=1 より、累積和を保持したa_sum[]は単調増加でソートされた状態になっているため、
 * そのままbinary_search()を使える。
 */
int main(void)
{
	// 0-indexed
	ll i;
	ll N, P, Q, R;
	cin >> N >> P >> Q >> R;
	// a[]の累積和(要素番号の-1までのa[]の和)
	// a_sum[i]:a[0]+a[1]+...+a[i-1]   i=0～N
	// 先頭部分の実装を簡単にするため、a_sum[i]はa[i-1]までの和、としている
	vector<ll> a_sum(N+1);
	for(i = 0; i < N; i++)
	{
		ll a;
		cin >> a;
		a_sum[i+1] = a_sum[i] + a;
	}
	// ここまで入力

	bool bOK = false;
	for(i = 0; i < N; i++)
	{
		if(binary_search(a_sum.begin(), a_sum.end(), a_sum[i]+P)
			&& binary_search(a_sum.begin(), a_sum.end(), a_sum[i]+P+Q)
			&& binary_search(a_sum.begin(), a_sum.end(), a_sum[i]+P+Q+R))
		{
			bOK = true;
			break;
		}
	}

	if(bOK)
	{
		cout << "Yes" << endl;
	}
	else
	{
		cout << "No" << endl;
	}

	return 0;
}
