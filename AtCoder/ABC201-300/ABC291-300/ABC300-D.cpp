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

// ABC300 https://atcoder.jp/contests/abc300

/*
 * N=10^12であり、またa<b<cかつc^2という記載より、
 * cの最大は√N=10^6程度となる。
 * まずは10^6までの素数をエラトステネスの篩で求める。(78498個だった)
 * 
 * また入力例2より、Nが最大の場合でも答は10^6程度であるため、
 * 適切に枝刈りをすればa,b,cの3重ループでも間に合うと推測できる。
 * 
 * 枝刈りとして、
 *   aを決めたときに、b=a+1, c=a+2として a^2*b*c^2>N ならばaのループは終了してよい
 *   a,bを決めたときに、c=b+1として a^2*b*c^2>N ならばbのループは終了してよい
 * という実装を行った。
 */

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
	vector<ll> p;
	ll N;
	cin >> N;
	make_prime_list(1000000, p);  // 10^6までの素数 78498個

	ll a, b, c;
	ll size = p.size();
	ll answer = 0;

	// a^2*b*c^2 を返す
	auto func = [&](ll p_a, ll p_b, ll p_c) -> ll
	{
		return p_a*p_a*p_b*p_c*p_c;
	};
	for(a = 0; a < size; a++)
	{
		if(func(p[a], p[a+1], p[a+2]) > N) break;  // 枝刈り aが決まった時に、b,cが最小でもNを超えるなら計算しても無駄
		for(b = a+1; b < size; b++)
		{
			if(func(p[a], p[b], p[b+1]) > N) break;  // 枝刈り a,bが決まった時に、cが最小でもNを超えるなら計算しても無駄
			for(c = b+1; c < size; c++)
			{
				if(func(p[a], p[b], p[c]) > N) break;
				answer++;
			}
		}
	}
	cout << answer << endl;

	return 0;
}
