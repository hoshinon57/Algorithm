#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC361 https://atcoder.jp/contests/abc361

/*
 * 公式解説動画での方法。
 * https://www.youtube.com/live/5pxjBcAn3mQ
 * 
 * a^8やa^6, a^4で表現できるxは、全てa^2でも表現できる。
 * よってans[b]を
 *   a^bにて表現できる値の種類
 *   ただし複数のbにて表せる場合は最大のbのみカウントする
 * として定義すると、例えばb=2は
 *   ans[2] = √N - ans[4] - ans[6] - ... - ans[60]
 * となる。
 * bの大きい方から求めていくことができる。
 */

// Luzhiled's Library より
// https://ei1333.github.io/library/math/number-theory/kth-root-integer.hpp.html
// aのk乗根を整数(切り捨て)で返す
// 制約：0<=a<2^64, 1<=k<=64
// 計算量：O(k*loglog(a))
// 補足：制約のk上限を外したい場合、先頭の#ifを有効化する
uint64_t kth_root_integer(uint64_t a, int k) {
	if (k == 1) return a;
#if 1	// Libraryから追加 kを上限クリップしたい場合はこちらを有効
	if (a <= 1 || k == 1) return a;	// a=0時に0を返したいため、必要
	if (k >= 64) return 1;
#endif
	auto check = [&](uint32_t x) {
		uint64_t mul = 1;
		for (int j = 0; j < k; j++) {
			if (__builtin_mul_overflow(mul, x, &mul)) return false;
		}
		return mul <= a;
	};
	uint64_t ret = 0;
	for (int i = 31; i >= 0; i--) {
		if (check(ret | (1u << i))) ret |= 1u << i;
	}
	return ret;
}

int main(void)
{
	ll i, j;
	ll N; cin >> N;

	const ll M = 60;
	vector<ll> ans(M);
	for(i = M; i >= 2; i--)
	{
		ans[i] = kth_root_integer(N, i);
		for(j = i*2; j <= M; j += i) ans[i] -= ans[j];
	}
	cout << reduce(ans.begin(), ans.end()) << endl;

	return 0;
}
