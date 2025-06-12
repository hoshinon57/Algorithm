#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <cstdint>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC400 https://atcoder.jp/contests/abc400

/*
 * 異なる(a,b)の組み合わせで同じXが出てくるのが厄介。
 * 重複をどうにかしたい、となる。
 * 
 * 試行錯誤すると、a=1,a=2の2パターンのみ考えれば十分かつ重複しない、となる。
 * 例えばa=3のとき、2^3=2*2^2として2^2のぶんをb^2に追いやれば、a=1のときと同じになることが分かる。
 * a=4のときも同様にa=2と同じになる。
 * 
 * a=1のときは√(N/2), a=2のときは√(N/4)がbの個数。
 * sqrtを使うのは怖かったのでお借りしたライブラリにした。
 * 
 * ※解説では「bは奇数だけ考える」という方針もあった。なるほど。
 * 
 * [ACまでの思考の流れ]
 * ・重複が出ないような数え上げを考える。
**/

// Luzhiled's Library より
// https://ei1333.github.io/library/math/number-theory/kth-root-integer.hpp.html
// aのk乗根を整数(切り捨て)で返す
// 制約：0<=a<2^64, 1<=k<=64
// 計算量：O(k*loglog(a))
// 補足：制約のk上限を外したい場合、先頭の#ifを有効化する
// #include <cstdint> が必要かも。AtCoderでは問題なかったが、Library CheckerではCEとなった。
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
	ll N; cin >> N;
	ll ans = 0;
	ans += kth_root_integer(N/2, 2);
	ans += kth_root_integer(N/4, 2);	
	cout << ans << endl;

	return 0;
}
