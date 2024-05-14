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
const ll MOD = 998244353;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC353 https://atcoder.jp/contests/abc353

/*
 * 各Aiが答にどれだけ寄与するかを計算していく。
 * 主客転倒みたいな形か。
 * 
 * f(x,y)!=f(y,x)のため、Aはソート不可。
 * Aiについて固定したときに、以下2パターンを考える。
 * (1)f(Ai,x)
 *   Ai*10^(xの桁数) だけ答に加算される。
 *   よって「これまでに**桁の値が何回登場したか」をカウントして
 *   Aiを右側から見ていけば計算できる。
 * (2)f(x,Ai)
 *   Aiだけ答に加算される。
 *   (1)にて右から見ていく場合、Ai*iとなる。
 * 
 * [ACまでの思考の流れ]
 * ・数え上げだし、主客転倒っぽいなあ。
 * ・Aの各要素がどれぐらい答に寄与するかを考えるときに、
 *   f(Ai,x)とf(x,Ai)のそれぞれを計算すればよさそう。
 */

// n(>=0)の桁数を返す dig(0)=1
int cal_digit(long long n)
{
	if(n == 0) return 1;
	int c = 0;
	while(n > 0) {
		c++;
		n /= 10;
	}
	return c;
}

// p^0～p^(n-1)を事前計算して返す
// mが0以外の場合、mで割った余りを設定する
template<typename T>
vector<T> cal_pow_inadv(T p, int n, T m = 0)
{
	vector<T> ret(n);
	ret[0] = (T)1;
	for(int i = 1; i < n; i++) {
		ret[i] = ret[i-1] * p;
		if(m != 0) ret[i] %= m;
	}
	return ret;
}

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	auto p = cal_pow_inadv<ll>(10, 11, MOD);
	vector<ll> cnt(11);  // cnt[i]:桁数がiである値の数

	ll ans = 0;
	for(i = N-1; i >= 0; i--)
	{
		// f(Ai,x)のパターン
		for(j = 1; j < 11; j++)
		{
			ans += a[i] * cnt[j] % MOD * p[j] % MOD;
			ans %= MOD;
		}
		// f(x,Ai)のパターン
		ans += a[i] * i;
		ans %= MOD;

		cnt[cal_digit(a[i])]++;
	}
	cout << ans << endl;

	return 0;
}
