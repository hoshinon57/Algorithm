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

// ABC304 https://atcoder.jp/contests/abc304

/*
 * 公式解説、およびkyopro_friends氏のツイートと同じ考え方。
 *   https://twitter.com/kyopro_friends/status/1665004494827016198
 * 
 * すべてのピースを扱うのではなく、イチゴに着目して考える。
 * イチゴごとに「x方向およびy方向で何番目のピースに乗っているか」を二分探索で求め、
 * 連想配列cntを用いてカウントしていく。
 * 
 * 最大：cntの要素のうちの最大
 * 最小：
 *   cntの要素数 != (A+1)*(B*1) であれば、イチゴが乗っていないピースが必ず存在するため、0.
 *   そうでない場合、cntの要素のうちの最小
 * となる。
 */
int main(void)
{
	int i;
	ll W, H, N;
	ll A, B;
	cin >> W >> H >> N;
	vector<ll> p(N), q(N);  // p:x, q:y
	for(i = 0; i < N; i++) cin >> p[i] >> q[i];

	cin >> A;
	vector<ll> a(A);  // a:x
	for(i = 0; i < A; i++) cin >> a[i];
	cin >> B;
	vector<ll> b(B);  // b:y
	for(i = 0; i < B; i++) cin >> b[i];

	map<pair<int,int>, int> cnt;
	for(i = 0; i < N; i++)
	{
		int p_idx, q_idx;  // p:x, q:y
		p_idx = lower_bound(a.begin(), a.end(), p[i]) - a.begin();
		q_idx = lower_bound(b.begin(), b.end(), q[i]) - b.begin();
		cnt[{p_idx, q_idx}]++;
	}

	int ans_max = -1, ans_min = INF32;
	for(auto &e : cnt)
	{
		ans_max = max(ans_max, e.second);
		ans_min = min(ans_min, e.second);
	}
	if((ll)cnt.size() != (A+1)*(B+1)) ans_min = 0;  // イチゴが乗っていないピースが必ず存在する

	cout << ans_min << " " << ans_max << endl;

	return 0;
}
