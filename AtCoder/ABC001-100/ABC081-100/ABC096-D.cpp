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
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC096 https://atcoder.jp/contests/abc096

/*
 * 選ぶ素数を末尾が3のものにしておけば、どの5個を選んでも3*5で末尾が5になり、合成数じゃんという考え。
 * 50000以下の素数は5000個とかあるので、末尾が3のものに絞っても余裕やろの精神。
 * 
 * 公式解説などでは、「5で割った余りが1」となる個数に絞っていた。なるほど。
**/

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
	int N; cin >> N;
	vector<ll> pl;
	make_prime_list(55555, pl);
	vector<ll> ans;

	for(auto &e : pl)
	{
		if(e%10 == 3)
		{
			ans.push_back(e);
			if((int)ans.size() == N) break;
		}
	}

	int sz_ = (int)ans.size();
	for(int i = 0; i < sz_; i++) {
		cout << ans[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;

	return 0;
}
