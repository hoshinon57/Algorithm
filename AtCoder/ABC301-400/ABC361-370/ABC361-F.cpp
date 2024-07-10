#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
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
 * 自力で解けず、解説を見た。
 * 包除原理を用いて解く。
 *   https://drken1215.hatenablog.com/entry/2024/07/07/020716
 *   https://atcoder.jp/contests/abc361/editorial/10358
 * 
 * bについて全探索する。
 * このとき素数であるbについてのみ考えれば良さそう。
 * (上記の公式解説も、最終的には素数のみ考慮しているっぽい。後半の「平方因子が含まれる場合は無視する」の部分)
 * 
 * あるbについて、a^b<=N より a=(Nのb乗根) となり、この数だけ答が存在する。
 * ⇒Luzhiled's Libraryのkth_root_integer()をお借りした
 * ただし a^6 = (p^2)^3 = (q^3)^2 のように、異なる素数から同一のxが計算される、
 * つまり重複があるため、包除原理を用いて除外していく。
 * 
 * 10^18 < 2^60 より、60以下の素数(17個)についてbit全探索にて選ぶ個数を全探索する。
 * 選んだ素数が奇数個なら答に加算し、偶数なら減算する。
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
	vector<ll> pr = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59};
	ll ps = pr.size();

	ll ans = 0;
	for(i = 1; i < (1<<ps); i++)  // bitが立っているpr[*]を選ぶ
	{
		ll k = 1;  // k乗
		for(j = 0; j < ps; j++)
		{
			if(((i>>j)&1) == 0) continue;
			k *= pr[j];
			if(k >= 64) break;  // やりすぎるとオーバーフローするので
		}
		// if(k >= 64) continue;  これはNG 例えばa^3とa^31の重なり(x=1のみ)は減算必要のため
		ll tmp = kth_root_integer(N, k);
		if(__builtin_popcount(i)%2 == 1) ans += tmp;
		else ans -= tmp;
	}
	cout << ans << endl;

	return 0;
}
