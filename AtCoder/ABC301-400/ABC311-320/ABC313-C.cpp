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

// ABC313 https://atcoder.jp/contests/abc313

/*
 * Aの総和は操作によって変化しない。
 * よって例えばN=4, A={4,7,3,7}のとき(総和は21)、最終的なA'はA'={5,5,5,6}となる。
 * A'の決まり方は以下。
 *   総和/N：ベース部分5
 *   総和%N：+1される個数
 * 
 * よって、Aを昇順にソートしておき、A'との差分を加算していけばよい。
 * 差分の和を2で割ったものが答。
 */

int main(void)
{
	ll i;
	ll N; cin >> N;
	vector<ll> a(N);
	ll a_sum = 0;
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		a_sum += a[i];
	}
	sort(a.begin(), a.end());

	vector<ll> result(N, a_sum/N);  // 操作終了後のA'
	for(i = 0; i < a_sum%N; i++)  // この要素数だけ+1
	{
		result[N-i-1]++;
	}

	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		ans += abs(a[i] - result[i]);
	}
	cout << ans/2 << endl;  // 1操作ごとにAの値が2変化するので、2で割ったものが操作回数

	return 0;
}
