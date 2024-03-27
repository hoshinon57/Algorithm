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

// 競プロ典型90問:46 https://atcoder.jp/contests/typical90/tasks/typical90_at

/*
 * 46で割った余りでまとめて考えて、その登場回数をカウントする。
 * (A+B+C)%46=0 となるようなケースにて、A,B,Cのカウントをかけた値が答に加算される。
 */

int main(void)
{
	int i, j, k;
	int N; cin >> N;
	const ll MOD = 46;
	vector<ll> a_cnt(MOD), b_cnt(MOD), c_cnt(MOD);  // cnt[i]:入力値を46で割った余りiが、登場する回数
	for(i = 0; i < N; i++)
	{
		int n; cin >> n;
		n %= MOD;
		a_cnt[n]++;
	}
	for(i = 0; i < N; i++)
	{
		int n; cin >> n;
		n %= MOD;
		b_cnt[n]++;
	}
	for(i = 0; i < N; i++)
	{
		int n; cin >> n;
		n %= MOD;
		c_cnt[n]++;
	}

	ll ans = 0;
	for(i = 0; i < MOD; i++)
	{
		for(j = 0; j < MOD; j++)
		{
			k = (MOD + MOD + MOD - i - j) % MOD;  // i+j+kが46の倍数になるように
			ans += a_cnt[i]*b_cnt[j]*c_cnt[k];
		}
	}
	cout << ans << endl;	

	return 0;
}
