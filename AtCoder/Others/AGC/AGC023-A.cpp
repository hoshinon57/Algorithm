#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// AGC023 https://atcoder.jp/contests/agc023

/*
 * 累積和で解く。
 * 
 * 入力例1 "1 3 -4 2 2 -2" で考えたときに、累積和を取ってみると以下になる。(先頭に0を入れている)
 *   0, 1, 4, 0, 2, 4, 2
 * 同じ値が出ているところと、答となる部分列が一致している。
 * 
 * よって連想配列を使って、
 *   ある値に対し、それが累積和として出てくる添字の一覧
 * を計算しておく。
 * そして累積和の先頭から順に、同じ累積和が自分以降に出てくる数をupper_bound()で求めればよい。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	map<ll, vector<int>> mp;  // {累積和の値 -> その値がa_sum[]に登場する添字の一覧}
	mp[0].push_back(0);
	vector<ll> a_sum(N+1);
	for(i = 0; i < N; i++)
	{
		a_sum[i+1] = a_sum[i] + a[i];
		mp[a_sum[i+1]].push_back(i+1);  // 累積和sumは, i+1番目に登場する
	}

	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		// i番目の累積和sumに対し、その値がi番目以降に何回出てくるかを求める
		ll sum = a_sum[i];
		ans += mp[sum].end() - upper_bound(mp[sum].begin(), mp[sum].end(), i);
	}
	cout << ans << endl;

	return 0;
}
