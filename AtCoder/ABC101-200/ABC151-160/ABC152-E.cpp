#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1e9+7;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC152 https://atcoder.jp/contests/abc152

/*
 * B1+B2+...+Bnの最小値を求めたいので、
 * L=Ai*BiとなるLは、A1～Anの最小公倍数にすればよさそう。
 * 
 * ただし桁あふれの観点から
 *   A1*A2/GCD(A1,A2) = X
 *   X*A3/GCD(X,A3)
 *   ...
 * と単純に先頭から計算してもダメだろう、と考えた。(たぶんGCDが求められない)
 * 
 * よって最小公倍数を p1^e1 * p2^e2 * ... のように素因数分解した形でまず求める。
 * これは各Aについて素因数分解し、eの部分の最大値を保持していくことで求められる。
 * 
 * 後は上記をMOD付きで愚直に計算したものをLとし(実装ではlcm_tot)、
 * 各AについてB=L/Aとなるので、これを加算していったものが答となる。
 * 
 * 計算量は、素因数分解の部分が支配的で、O(N√(maxA))となる。
 */

// Nを素因数分解し、{素因数, 指数}のpairの形でprime_listに入れる
// 以下URLをほぼそのまま持ってきている
// https://qiita.com/drken/items/a14e9af0ca2d857dad23
// その他、参考URL：
//   https://algo-method.com/descriptions/119
void prime_factorize(long long N, vector<pair<long long, long long>> &prime_list)
{
	long long i;
	// 入力Nに対して、2から√Nの各整数で割れるだけ割っていく
	for(i = 2; i*i <= N; i++)
	{
		if(N%i != 0) continue;  // iが素因数でなければ次へ

		long long ex = 0;  // 指数
		while(N%i == 0)  // 割れる限り割る
		{
			N /= i;
			ex++;
		}
		prime_list.push_back({i, ex});  // iで割り切ったら結果を保存
	}
	if(N != 1)  // 割り切った後に1以外の値で残っていれば、それも素因数
	{
		prime_list.push_back({N, 1});
	}

	return;
}

// mod mの世界におけるaの逆元を返す
// 以下URLのコードをそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modinv(long long a, long long m)
{
	long long b = m, u = 1, v = 0;
	while (b) {
        long long t = a / b;
        a -= t * b; swap(a, b);
        u -= t * v; swap(u, v);
    }
    u %= m; 
    if (u < 0) u += m;
    return u;
}

// a^n (mod) を計算する
// 例えば3^5=3^1*3^4であり、5をビットごとに見ていって最下位ビットが1のときに積算する
// 以下URLのコードをほぼそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modpow(long long a, long long n, long long mod)
{
	long long ans = 1;
	while(n > 0)
	{
		if((n&1) != 0)  // nの最下位ビットが立っているなら、答に掛ける
			ans = (ans * a) % mod;
		a = (a * a) % mod;
		n >>= 1;
	}
	return ans;
}

int main(void)
{
	int i;
	ll N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> lcm(1e6+5);  // lcm[i]=a：A1～AnのLCMにはi^aが含まれる
	for(i = 0; i < N; i++)
	{
		vector<pair<ll, ll>> prime_list;
		prime_factorize(a[i], prime_list);
		for(auto &e : prime_list)
		{
			lcm[e.first] = max(lcm[e.first], e.second);
		}
	}

	ll lcm_tot = 1;
	for(i = 0; i < (int)lcm.size(); i++)
	{
		if(lcm[i] == 0) continue;
		lcm_tot *= modpow(i, lcm[i], MOD);
		lcm_tot %= MOD;
	}

	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		// LCM/a[i]
		ans += lcm_tot * modinv(a[i], MOD) % MOD;
		ans %= MOD;
	}
	cout << ans << endl;

	return 0;
}
