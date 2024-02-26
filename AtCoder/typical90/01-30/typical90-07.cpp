#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:07 https://atcoder.jp/contests/typical90/tasks/typical90_g

/*
 * 各種解説の通り。
 *   https://twitter.com/e869120/status/1379565222541680644
 *   https://drken1215.hatenablog.com/entry/2022/03/24/091500
 * 
 * 入力Aをソートしておき、
 * lower_bound()にてB以上とB未満であるAiを探し、差が小さい方が答。
 * Aに番兵としてINF,-INFを登録しておくと処理が楽。
 * 
 * [ACまでの思考の流れ]
 * ・Aをソートしても答には影響ない。
 *   ⇒要素の検索はソートして二分探索[典型]
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	a.push_back(-INF64);  // 番兵
	a.push_back(INF64);
	sort(a.begin(), a.end());

	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		ll b; cin >> b;
		int idx = lower_bound(a.begin(), a.end(), b) - a.begin();
		ll ans = min(a[idx] - b, b - a[idx-1]);
		cout << ans << endl;
	}

	return 0;
}
