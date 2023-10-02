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

// ABC171 https://atcoder.jp/contests/abc171

/*
 * 考え方はけんちょん氏の以下の解説と同じ。
 * https://drken1215.hatenablog.com/entry/2020/06/21/224900
 * 
 * 値が数列Aの中に何回登場するかをcnt[]でカウントしておく。
 * クエリごとに総和は
 *   cnt[b]*b 減り、
 *   cnt[c]*b 増える
 * ことになるので、差分を計算していけばよい。
 */

int main(void)
{
	int i;
	ll N; cin >> N;
	vector<ll> cnt(100000+5);
	ll sum = 0;
	for(i = 0; i < N; i++)
	{
		ll a; cin >> a;
		cnt[a]++;
		sum += a;
	}
	
	ll Q; cin >> Q;
	for(i = 0; i < Q; i++)
	{
		ll b, c; cin >> b >> c;
		sum -= cnt[b] * b;
		sum += cnt[b] * c;
		cout << sum << endl;
		cnt[c] += cnt[b];
		cnt[b] = 0;
	}

	return 0;
}
