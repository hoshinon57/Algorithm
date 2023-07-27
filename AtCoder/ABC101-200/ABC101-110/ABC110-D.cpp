#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC110 https://atcoder.jp/contests/abc110

/*
 * 素因数分解と組み合わせ(Combination)で解く。
 * 
 * M=b1^c1 * b2^c2 * ... * bk^ck と素因数分解したときに、
 * biがci個あるので、それをa1～aNに振り分けることになる。
 *   例：2^4なら、4つある2をa1,a2, ... , aNに振り分ける方法は何通りか、になる。
 * これは重複組み合わせで求めることができ、(N-1+ci)C(N-1)となる。
 * これを各bi,ciについて掛け算していった結果が答。
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

const ll MOD = 1000000007;
const ll NCK_SIZE = 100030;  // N<=2*10^5 など、問題文の制約に合わせる
// -> N+指数 になり、指数は最大でも30程度 (M=10^9 < 2^30) なので、10^5+30程度で十分
ll fact[NCK_SIZE], inv_fact[NCK_SIZE], inv[NCK_SIZE];

// Combination用の事前計算
void nCk_init(void)
{
	fact[0] = fact[1] = 1;
	inv[0] = inv[1] = 1;
	inv_fact[0] = inv_fact[1] = 1;

	for(int i = 2; i < NCK_SIZE; i++)
	{
		// 階乗
		fact[i] = fact[i - 1] * i % MOD;
		// 逆元
		inv[i] = MOD - inv[MOD%i] * (MOD / i) % MOD;
		// 逆元の階乗
		inv_fact[i] = inv_fact[i - 1] * inv[i] % MOD;
	}
}

// CombinationのnCkを求める
ll nCk(ll n, ll k)
{
	if (n < k) return 0;
	if (n < 0 || k < 0) return 0;

	ll x = fact[n];  // n!の計算
	ll y = inv_fact[n-k];  // (n-k)!の計算
	ll z = inv_fact[k];  // k!の計算
	
	return x * ((y * z) % MOD) % MOD;  //nCkの計算
}

int main(void)
{
	ll N, M;
	cin >> N >> M;
	vector<pair<ll,ll>> prime_list;
	prime_factorize(M, prime_list);

	nCk_init();

	ll answer = 1;
	for(auto e : prime_list)
	{
		answer *= nCk(N-1+e.second, N-1);
		answer %= MOD;
	}
	cout << answer << endl;

	return 0;
}
