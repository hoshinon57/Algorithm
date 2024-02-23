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

// ABC341 https://atcoder.jp/contests/abc341

/*
 * 二分探索で解く。
 * 
 * まず、ある値X以下について、「NとMのうちちょうど一方のみで割り切れる」個数を考えてみる。(※)
 * 
 * Nで割り切れる数の集合をA, Mを同様にBとすると、
 * 「NとMのうちちょうど一方のみで割り切れる」集合は N∪M-(N∩M)*2 となる。
 * N∩M は「N,MのLCMで割り切れる数の集合」に等しい。
 * これで(※)は求められた。
 * 
 * (※)の個数はXについて広義単調増加になっている。
 * そのため「ある値X以下について、N,Mの片方でのみ割り切れる個数がK個未満か？」という判定問題を考えることで、
 * 小さい方からK番目の値を二分探索で求めることができる。
 */

// a,bの最大公約数を返す
// C++17から標準ライブラリに搭載されているとのこと
// 参考：https://cpprefjp.github.io/reference/numeric/gcd.html
long long gcd(long long a, long long b)
{
	if(b == 0) return a;
	else return gcd(b, a%b);
}

int main(void)
{
	ll N, M, K; cin >> N >> M >> K;
	ll lcm = N*M/gcd(N,M);

	auto check = [&](ll x) -> bool
	{
		ll cnt = x/N + x/M - x/lcm*2;
		return cnt < K;
	};
	ll l = 0, r = INF64;
	while(r - l > 1)
	{
		ll mid = (l+r)/2;
		if(check(mid)) l = mid;
		else r = mid;
	}
	cout << r << endl;

	return 0;
}
