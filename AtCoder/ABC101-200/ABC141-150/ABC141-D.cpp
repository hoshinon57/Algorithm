#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC141 https://atcoder.jp/contests/abc141

/*
 * 貪欲法で解く。
 * 
 * 割引券は、現時点で最も高い品物に使う、としてしまって良い。
 * (他のどの品物に使うケースであっても、最も高い品物に使う方が悪くない結果にできるため)
 * 
 * よって
 *   ・最大の値を取得し、削除
 *   ・取得した値/2 を要素に追加
 * をM回繰り返せばよい。
 * 自分はpriority_queueではなくmultisetを用いた。
 * 
 * [ACまでの思考の流れ]
 * ・割引券は最高額の品物に使うしかないっしょ…！
 * ・なら貪欲法か
 */

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	multiset<int> a;
	for(i = 0; i < N; i++)
	{
		int c; cin >> c;
		a.insert(c);
	}

	for(i = 0; i < M; i++)
	{
		// 最も高い品物に割引券を使う
		int m = *a.rbegin();
		a.erase(a.find(m));
		a.insert(m/2);
	}
	ll ans = 0;
	for(auto &e : a) { ans += e; }
	cout << ans << endl;

	return 0;
}
