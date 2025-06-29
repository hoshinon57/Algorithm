#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <cstdint>
#include <unordered_map>
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
 * コンテスト時に解けず、解説を見た。
 * 
 * Anは単調増加していく。
 * どこでA(i-1)<Aiになるかを実験して調べてみると、素数pに対して
 *   Ai=p^n
 * となるAiにてAiが増える、つまり整数の種類が増える。
 * 例えばA=13といった素数や、A=16=2^4といった部分。
 * 
 * つまり素因数分解したときに素因数が1つの箇所、これを探したい。
 * 
 * (ここでいったん、R-L<=10^7という制約は忘れる。R<=10^14のみ意識する)
 * 素因数の数>=2のとき、a=x*yと表せる。つまり1～√Rまでで必ず割り切れることから、
 *   (1)1～√Rまで割り切れないとき、aは素数。
 *   (2)1～√Rまで2回以上割り切れるとき、その時点でa=x*yと表せるので答からは除外。
 *   (3)1～√Rまでちょうど1回割り切れたとき。
 *      ここがもう1つ考察必要で、割り切れた値をpとすると
 *        a=p^n
 *        a=p*q (p<=√R, q>√R)
 *      の2パターンがありえる。前者は答になり、後者は除外。
 *      これはaをpで割れるだけ割って、最後に1になれば前者。そうでなければ後者となる。
 * 
 * 区間[L,R]の値について1～√Rまで割り切れるかどうかは、
 * R-L<=10^7という制約より愚直にやって十分に間に合う。
 * 割り切る素数の数をcnt[]としたとき、cnt[a]とするとMLEするので、Lのゲタをはかせる。
 * 
 * [L,R]を小さい方から見ていったとき、Lは常に「新種の整数」になる点に注意。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・(幅の狭い)区間[L,R]について素数を列挙するアルゴリズムとして「エラトステネスの区間篩」というものがある。
 *   これをそもそも不勉強だった。
 *   https://algo-method.com/tasks/332/editorial
 * ・実験などで、小さい方から見ていったときにどんな値で「新種の整数」が登場するかを調べる。
**/

// a/b以上の最小の整数(天井関数) ceil(5,2)=3, ceil(-5,2)=-2
template <typename T> T ceil_div(T a, T b){ if(b<0) {a=-a; b=-b;} if(a>0){return (a+b-1)/b;} else {return a/b;}}

// a,bの最大公約数を返す
// C++17から標準ライブラリに搭載されているとのこと
// 参考：https://cpprefjp.github.io/reference/numeric/gcd.html
long long gcd(long long a, long long b)
{
	if(b == 0) return a;
	else return gcd(b, a%b);
}

// 1～nのLCM
ll nlcm(ll n)
{
	ll i;
	ll now = 1;
	for(i = 2; i <= n; i++)
	{
		ll g = gcd(now, i);
		now = now * i / g;
	}
	return now;
}

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

int main(void)
{
#if 0  // test
	for(ll i = 1; i <= 40; i++)
	{
		cout << i << ":" << nlcm(i) << endl;
	}
#endif
	ll L, R; cin >> L >> R;
	ll rR = kth_root_integer(R, 2);  // √R
	vector<ll> plist;
	make_prime_list(rR, plist);

	ll dif = R-L;
	vector<ll> cnt(dif+10);  // cnt[a-L]:値aを割り切る素数の数 
	vector<ll> pp(dif+10);// pp[a-L]:値aを割り切る素数のうち、最小のもの (後から考えたら別に最小でなくても良かったか)
	for(auto &p : plist)
	{
		ll st = ceil_div(L,p)*p;
		ll ed = (R/p)*p;
		for(ll a = st; a <= ed; a+=p)  // aはpで割り切れる
		{
			cnt[a-L]++;
			if(cnt[a-L] == 1)
			{
				pp[a-L] = p;
			}
		}
	}

	ll ans = 0;
	for(ll a = L; a <= R; a++)
	{
		if(a == L)  // 「含まれる整数の種類」なので、先頭のLは常にカウントされる
		{
			ans++;
			continue;
		}

		if(cnt[a-L] == 0)  // aは素数
		{
			ans++;
		}
		else
		{
			if(cnt[a-L] >= 2) continue;

			// aは1～√Rまででちょうど1回割り切れた 割った値はpa
			ll n = a;
			ll pa = pp[a-L];
			while(n > 1)
			{
				if(n%pa == 0) n /= pa;
				else break;
			}
			if(n == 1) ans++;
		}
	}
	cout << ans << endl;

	return 0;
}
