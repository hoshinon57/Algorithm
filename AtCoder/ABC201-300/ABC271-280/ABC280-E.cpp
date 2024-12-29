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

#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;

// ABC280 https://atcoder.jp/contests/abc280

/*
 * 期待値DPで解く。
 * 
 * dp[n]を残HP=nからの攻撃回数の期待値とする。
 * 問題文より、p=P/100, q=1-P/100とすると、
 * dp[n] = (dp[n-2]+1)*p + (dp[n-1]+1)*q
 * となるので、これをメモ化再帰で解いた。
 */

mint p, q;
vector<bool> memo;  // メモ化済みならtrue
vector<mint> dp;

// 残HP=nからの回数の期待値を返す
mint func(ll n)
{
	if(memo[n]) return dp[n];
	if(n <= 0) return 0;

	mint &d = dp[n];
	d = (func(n-2)+1)*p + (func(n-1)+1)*q;

	memo[n] = true;
	return d;
}

int main(void)
{
	ll N, P; cin >> N >> P;
	p = (mint)P/100;
	q = (mint)1-p;
	memo.resize(N+1, false);
	dp.resize(N+1, 0);
	cout << func(N).val() << endl;

	return 0;
}
