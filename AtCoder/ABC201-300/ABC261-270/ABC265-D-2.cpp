#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC265 https://atcoder.jp/contests/abc265

/*
 * 公式解説を元にした実装。
 * 二分探索と累積和を使って解く。
 * その際setを用いて累積和を追加しておくことで、二分探索の実装を省略する。
 * 
 * xについて全探索する。
 * あるxについて、
 *   (1) (a[0]+...+a[x-1]) + P
 *   (2) (a[0]+...+a[x-1]) + P + Q
 *   (3) (a[0]+...+a[x-1]) + P + Q + R
 * がそれぞれ存在するかを判定すればよい。
 * (a[0]+...+a[x-1]) の部分は事前に累積和を求めておく。
 * また累積和をsetにて保持しておくことで、上記(1)～(3)の存在をそれぞれO(logN)にて判定できる。
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
	set<ll> a_set;
	a_set.insert(0);  // x=0の場合を想定
	for(i = 0; i < N; i++)
	{
		ll a;
		cin >> a;
		a_sum[i+1] = a_sum[i] + a;
		a_set.insert(a_sum[i+1]);
	}
	// ここまで入力

	// xについて全探索する
	// x=iとし、a[x]をスタートとして、そこからP, P+Q, P+Q+Rとなる累積和が存在するか判定する
	bool bOK = false;
	for(i = 0; i < N; i++)
	{
		if( a_set.find(a_sum[i]+P) != a_set.end()  // a_sum[i]:a[0]～a[i-1]までの累積和
			&& a_set.find(a_sum[i]+P+Q) != a_set.end()
			&& a_set.find(a_sum[i]+P+Q+R) != a_set.end() )
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
