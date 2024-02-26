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
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:42 https://atcoder.jp/contests/typical90/tasks/typical90_ap

/*
 * まず9の倍数という条件があるので、Kが9で割り切れない場合は答は0.
 * 逆に9で割り切れる場合、桁和がKになる総数を求めればよい。
 * 
 * f(n)を桁和がnになる総数とすると、f(K)が答。
 * f(n)=f(n-1)+f(n-2)+...+f(n-9) より、再帰で解ける。
 * 
 * ※最初、桁DPで考えててうまく解けなかった。
 *   「N以下の整数である」といった制約が無いので、桁DPでは解けないっぽいね。
 */

vector<ll> memo;  // memo[i]:桁和がiとなる整数の総数

// 数値の桁和がnとなる数の総数を返す
ll func(ll n)
{
	if(n < 0) return 0;
	if(n == 0) return 1;
	if(memo[n] != -1) return memo[n];

	ll m = 0;
	for(int i = 1; i <= 9; i++)
	{
		m += func(n-i);
	}
	m %= MOD;

	return memo[n] = m;
}

int main(void)
{
	ll K; cin >> K;
	if(K%9 != 0)
	{
		cout << 0 << endl;
		return 0;
	}
	memo.resize(K+1, -1);
	cout << func(K) << endl;

	return 0;
}
