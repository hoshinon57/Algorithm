#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC233 https://atcoder.jp/contests/abc233

/*
 * ABC233-D.cppにて記載した以下の実装。
 * ＞余談として、最初はsum[]をmultisetにて登録していく手法を考えた。
 * ＞ただこの場合、sum[i]-Kとなる要素数をカウントする処理に時間がかかり(※)、TLEとなった。
 * ＞(※)ワーストでO(N)になると思われる
 * 
 * 上記の通りTLEとなるが、参考として残しておく。
 */
int main(void)
{
	// 0-indexedで処理する
	ll i;
	ll N, K;
	cin >> N >> K;

	vector<ll> a(N);
	vector<ll> sum(N);  // sum[i]:a[0]～a[i]までの和
	multiset<ll> set_sum;
	// sum[0]=a[0]という決め方により、0を事前に入れておくと処理が簡易になる。
	// a[0]+a[1]+...+a[r]=Kのときのイメージ。
	set_sum.insert(0);
	ll answer = 0;
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		if(i == 0)
		{
			sum[0] = a[0];
		}
		else
		{
			sum[i] = sum[i-1] + a[i];
		}

		auto itr = set_sum.find(sum[i]-K);
		while(itr != set_sum.end() && *itr == sum[i]-K)
		{
			answer++;
			itr++;
		}

		set_sum.insert(sum[i]);
	}

	cout << answer << endl;

	return 0;
}
