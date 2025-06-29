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

// ABC412 https://atcoder.jp/contests/abc412

/*
 * 後に考えた方針。
 *   (1)[L+1,R]の全素数
 *   (2)[L+1,R]の範囲で、√Rまでの素数pに対してp^n (n>=2) となる値
 * の合計が答となる。(1)と(2)は被らない。
 * 
 * (1)はエラトステネスの区間篩にて。
 * (2)は√Rまでの素数pを列挙し、p^nが[L+1,R]の範囲になるかをチェックする。
 *    n=60ともなると最低でも2^60≒10^18になって範囲超えるので、十分に間に合う。
 * 
 * 今回の自分の実装だと、(1)のコーナーケースにちょっと苦戦した。
 * [L,R]に√Rまでの素数がそもそも含まれているケース、かな。
**/

// a/b以上の最小の整数(天井関数) ceil(5,2)=3, ceil(-5,2)=-2
template <typename T> T ceil_div(T a, T b){ if(b<0) {a=-a; b=-b;} if(a>0){return (a+b-1)/b;} else {return a/b;}}

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

// 素数列挙(エラトステネスの篩)
// N以下の素数を列挙してprime_listに入れる
// 計算量はO(NloglogN)とのこと。以下2つの記事を参考。
//   https://algo-method.com/descriptions/64
//   https://manabitimes.jp/math/992
void make_prime_list(long long N, vector<long long> &prime_list)
{
	vector<bool> isPrime(N+1, true);
	isPrime[0] = isPrime[1] = false;
	for(long long i = 2; i <= N; i++)
	{
		if(!isPrime[i]) continue;
		// iは素数
		prime_list.push_back(i);
		// iの倍数を素数から外す
		for(long long j = i+i; j <= N; j += i)
		{
			isPrime[j] = false;
		}
	}
	return;
}

// limitを上限として、a*bを返す
// 乗算でオーバーフローを防ぐ用途で用いる
// -> min(a*b, limit)  a,b,u>=0
// hamamuさんの以下コードをほぼそのまま持ってきている
//   https://twitter.com/hamamu_kyopro/status/1612015305110454274/
long long limitmul(long long a, long long b, long long limit)
{
	if(b == 0) return 0;  // a*b=0

	if(a <= limit / b)
		return a*b;
	else
		return limit;
}

int main(void)
{
	ll i;
	ll L, R; cin >> L >> R;
	ll rR = kth_root_integer(R, 2);  // √R
	vector<ll> plist;
	make_prime_list(rR, plist);

	ll ans = 1;  // Lは常にカウント
	ll dif = R-L;

	// [L+1,R]の素数を列挙
	vector<bool> isp(dif+10, true);  // isp[i-L]: iが素数ならtrue i-Lでアクセスする
	for(auto &p : plist)
	{
		ll st = ceil_div(L,p)*p;
		ll ed = (R/p)*p;
		for(ll a = st; a <= ed; a+=p)
		{
			if(a == p) continue;  // コーナーケース対応 [L,R]=[1,4]のとき、2は素数なのに除外されてしまう
			isp[a-L] = false;
		}
	}
	for(i = L+1; i <= R; i++)  // i=Lは除く
	{
		if(isp[i-L]) ans++;
	}

	// √Rまでの素数pに対してp^n (n>=2)  [L+1,R]の範囲
	for(auto &p : plist)
	{
		ll now = p*p;  // p^2から見始める
		while(now <= R)
		{
			if(now >= L+1) ans++;  // nowは[L+1,R]の範囲で、かつp^nで表せる
			now = limitmul(now, p, INF64);
		}
	}

	cout << ans << endl;

	return 0;
}
