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

// ABC302 https://atcoder.jp/contests/abc302

/*
 * 二分探索で解く。
 * 
 * 考え方はevima氏やkyopro_friends氏の解説と同じ。
 *   https://atcoder.jp/contests/abc302/editorial/6412
 *   https://twitter.com/kyopro_friends/status/1659920748440588288
 * 
 * 配列Bをソートしておく。
 * 配列Aから値を1つ取り、aとする。
 *   配列Bのうち、a+D以下の最大値をupper_bound()で探す。
 *   そのような値が見つかれば、aとの和が答の候補になる。
 * 上記を配列Aについて全探索し、和の最大値が答となる。
 */
int main(void)
{
	int i;
	ll N, M, D;
	cin >> N >> M >> D;
	vector<ll> a(N), b(M);
	for(i = 0; i < N; i++) cin >> a[i];
	for(i = 0; i < M; i++) cin >> b[i];
	// sort(a.begin(), a.end());  // こちらはソート不要
	sort(b.begin(), b.end());

	ll ans = -1;
	for(i = 0; i < N; i++)
	{
		// 差がDより大きくなる最初の要素を探す
		// 「和の最大値」を求めるので、+D側で調べる
		auto itr = upper_bound(b.begin(), b.end(), a[i] + D);

		// チェックしたい対象はitrの1つ手前
		// 対象について、A側との差がD以下になっているか
		if(itr == b.begin()) continue;
		itr--;
		if(a[i] - *itr > D) continue;
//		if(abs(a[i] - *itr) > D) continue;  // 大小関係を考える手間が不要なので、こちらでもOK

		ans = max(ans, a[i] + *itr);
	}
	cout << ans << endl;

	return 0;
}
