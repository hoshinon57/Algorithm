#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC233 https://atcoder.jp/contests/abc233

/*
 * 0-indexedで処理する。
 * 
 * 累積和で考える。
 * まず、数列のa[0]～a[i]までの和をsum[i]に保持する。
 * すると a[2]+a[3] = sum[3]-sum[2-1] となる。
 * 
 * a[i]まで見たときに、
 * sum[i]-sum[x]=K となるようなxを探せばよい。
 * よって sum[x]=sum[i]-K となるようなsum[x]が何個あるか、という問題になる。
 * これは連想配列mapにてsum[]をカウントしていけばよい。
 * 
 * a[0]=sum[0]=K の場合を想定して、mapに0を事前に入れておくと処理が簡易になる。
 * sum[x](=0) = sum[0]-K = 0 というイメージ。
 * 
 * 余談として、最初はsum[]をmultisetにて登録していく手法を考えた。
 * ただこの場合、sum[i]-Kとなる要素数をカウントする処理に時間がかかり(※)、TLEとなった。
 * (※)ワーストでO(N)になると思われる
 */
int main(void)
{
	// 0-indexedで処理する
	ll i;
	ll N, K;
	cin >> N >> K;

	vector<ll> a(N);
	vector<ll> sum(N);  // sum[i]:a[0]～a[i]までの和
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		if(i == 0)
			sum[0] = a[0];
		else
			sum[i] = sum[i-1] + a[i];
	}

	map<ll, ll> map_sum;  // map_sum[x]:累積和sum[i]=x となるようなsum[]の要素数
	map_sum[0] = 1;  // a[0]=sum[0]=Kを踏まえたときに、map[0]を「存在する」ものとしておくと処理が簡易になる
	ll answer = 0;
	for(i = 0; i < N; i++)
	{
		answer += map_sum[sum[i]-K];  // sum[i]-sum[*]=K となるようなsum[*]の個数だけ、答になる
		map_sum[sum[i]]++;
	}

	cout << answer << endl;

	return 0;
}
