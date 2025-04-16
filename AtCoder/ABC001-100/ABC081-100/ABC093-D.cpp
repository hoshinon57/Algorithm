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

// ABC093 https://atcoder.jp/contests/abc093

/*
 * 自力で解けず、解説をチラ見した。
 *   https://drken1215.hatenablog.com/entry/2018/09/08/195000
 *   https://kyopro.hateblo.jp/entry/2018/09/10/165511
 *   https://atcoder.jp/contests/abc093/editorial/943
 * 
 * いくつか実験してみて、こうなるんだろうな、となった。
 * 
 * 入力a,bに対し、(ab-1)以下のスコアを取りたい。
 * 1回目の方が順位が高い(aが小さい)ケースと、ひっくり返して2回目の方が高い(bが小さい)ケースを考えると、
 * aについて√(ab-1)以下の範囲で考えてよさそう、となる。
 * X=√(ab-1) とする。
 * 
 * 1)入力a=bのとき
 * X=a-1に等しい。
 * 自身のスコアとして、aを1～Xの範囲で考えればよい。
 * またbも1～Xの範囲で考えればよく、これは"a=1～X"のaとは被らない。
 * (a^2-1)=(a+1)(a-1) より、a=Xのときb=X+2.
 * 
 * などなどを考えると、このときの答は2X.
 * 
 * 2)入力a!=bで、(ab-1)/X=Xのとき
 * つまりa=Xに対しb=Xがマッチングする。
 * 入力a<bとすると、a=1～Xの範囲で順位被りで使えないのが1つ出てくる。逆にbは順位被りしない。
 * 
 * などなどを考えると、このときの答は2X-2.
 * 
 * 3)入力a!=bで、(ab-1)/X!=Xのとき
 * つまりa=Xに対してbはX以外がマッチングする。
 * このときの答は2X-1.
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
	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		ll a, b; cin >> a >> b;
		ll ab1 = a*b-1;
		ll X = kth_root_integer(ab1, 2);
		ll ans;
		if(a == b)
		{
			ans = 2*X;
		}
		else if(ab1/X == X)
		{
			ans = 2*X-2;
		}
		else
		{
			ans = 2*X-1;
		}
		cout << ans << endl;
	}

	return 0;
}
