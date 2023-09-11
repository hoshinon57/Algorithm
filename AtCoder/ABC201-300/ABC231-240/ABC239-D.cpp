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

// ABC239 https://atcoder.jp/contests/abc239

// ある数xについて、x+C～x+Dの間に素数が1つも存在しないような値があるかを考える。
// そのようなxがA～Bの範囲に1つでも存在すれば、高橋くんの勝ち。1つも存在しなければ青木くんの勝ち。となる。
// 
// 素数テーブルは事前にライブラリを用いて求めておく。

int a, b, c, d;

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

// 高橋くんがxを選んだときに、高橋くんが勝つならtrue
bool solve(int x, vector<ll> &p_list)
{
	int y;
	for(y = c; y <= d; y++)
	{
		if(std::find(p_list.begin(), p_list.end(), x+y) != p_list.end())
		{
			return false;  // 和を素数にできたので、青木くんの勝ち
		}
	}

	return true;
}

int main(void)
{
	int x;
	cin >> a >> b >> c >> d;

	vector<ll> p_list;
	make_prime_list(200, p_list);

	for(x = a; x <= b; x++)
	{
		if(solve(x, p_list))
		{
			cout << "Takahashi" << endl;
			return 0;
		}
	}
	cout << "Aoki" << endl;

	return 0;
}
