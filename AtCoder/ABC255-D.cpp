#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC255 https://atcoder.jp/contests/abc255

/*
 * 累積和で解く。 
 * (単純に数列Aの要素ごとにXとの差分を足していく方式では、計算量がO(N*Q)となり間に合わない)
 * 
 * 以下、0-indexedとする。
 * まず、数列Aはソートしても問題ない。昇順ソートしておく。
 * sum[i]:a[0]～a[i]までの累積和 とする。
 * lower_bound()を用いて、X未満とX以上に分ける。
 * 
 * (1)X未満の要素については、A(i)に1加算する処理を行う。
 * X未満の要素がA[0]～A[K-1]とすると、
 *   (X-A[0])+(X-A[1])+...+(X-A[K-1])
 *   =X*K - (A[0]+...A[K-1])
 *   =X*K - sum[K-1]
 * が処理の回数となる。
 * 
 * (2)X以上の要素については、1減算する処理を行う。
 * X以上の要素がA[K]～A[N-1]とすると、
 *   (A[K]-X)+(A[K+1]-X)+...+(A[N-1]-K)
 *   =(A[K]+...+A[N-1]) - X*K
 *   =sum[N-1]-sum[K-1] - X*K
 * が処理の回数となる。
 * 
 * 後は、lower_boundで得られた境界値が0や数列末尾、の場合に注意して実装すればよい。
 */
int main(void)
{
	ll i;
	ll N, Q;
	cin >> N >> Q;
	vector<ll> a(N), sum(N);  // sum[i]:a[0]～a[i]までの累積和
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
	}
	sort(a.begin(), a.end());  // 昇順ソート
	for(i = 0; i < N; i++)  // 累積和の計算
	{
		if(i == 0)
		{
			sum[i] = a[i];
		}
		else
		{
			sum[i] = sum[i-1]+a[i];
		}
	}

	for(i = 0; i < Q; i++)
	{
		ll answer = 0;
		ll x;
		cin >> x;

		// X未満とX以上で分ける
		ll borderNo = lower_bound(a.begin(), a.end(), x) - a.begin();

		// X未満
		if(borderNo > 0)
		{
			ll tmp = borderNo * x - sum[borderNo-1];
			answer += tmp;
		}
		// X以上
		if(borderNo < a.size())
		{
			ll tmp;
			if(borderNo == 0)
			{
				tmp = (sum[a.size()-1]                ) - (a.size()-borderNo)*x;
			}
			else
			{
				tmp = (sum[a.size()-1]-sum[borderNo-1]) - (a.size()-borderNo)*x;
			}
			answer += tmp;
		}
		cout << answer << endl;
	}

	return 0;
}
