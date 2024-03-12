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

// アルゴリズムと数学 演習問題集 068 https://atcoder.jp/contests/math-and-algorithm/tasks/math_and_algorithm_be

/*
 * 包除原理で解く。
 * 公式解説のソースコードとほぼ同じだった。
 *   https://github.com/E869120/math-algorithm-book/blob/main/editorial/chap5-4/prob5-4-5.c
 * 
 * 「V1,V2,... のいずれかの倍数」を直接求めるのは難しいが、
 * 「V1,V2,... それぞれの倍数」は簡単に求められる。
 * ⇒NをV1,V2,...のLCMで割った値
 * 
 * 包除原理より、選んでいるVの数が奇数なら加算し、偶数なら減算していく。
 * Vの選び方はbit全探索を用いる。
 */

// a,bの最大公約数を返す
// C++17から標準ライブラリに搭載されているとのこと
// 参考：https://cpprefjp.github.io/reference/numeric/gcd.html
long long gcd(long long a, long long b)
{
	if(b == 0) return a;
	else return gcd(b, a%b);
}

ll lcm(ll a, ll b)
{
	return a*b/gcd(a,b);
}

int main(void)
{
	ll i, j;
	ll N, K; cin >> N >> K;
	vector<ll> v(K); for(i = 0; i < K; i++) {cin >> v[i];}

	ll ans = 0;
	for(i = 1; i < (1<<K); i++)  // bit全探索
	{
		ll l = 1;  // 選んでいるVのLCM
		for(j = 0; j < K; j++)  // v[j]
		{
			if((i>>j)&1)
			{
				l = lcm(l, v[j]);
			}
		}
		ll cnt = N/l;
		// 選んでいるVが奇数なら加算、偶数なら減算
		if(__builtin_popcountll(i)%2 == 1) ans += cnt;
		else ans -= cnt;
	}
	cout << ans << endl;

	return 0;
}
