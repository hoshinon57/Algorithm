#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
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

// Library Checker
// https://judge.yosupo.jp/problem/number_of_subsequences

/*
 * 部分列DP.
 * 
 * dp[i]:i番目まで見て、i番目の文字を使うときの部分列の総数
 * として定義する。
 * dp[*]の総和が答。
 * 
 * A[i]=xまで見たとき、xが前回登場した位置をpre[x]とすると、
 *   dp[i] = dp[pre[x]]～dp[i-1]までの総和
 * となる。
 * これはdpの累積和を逐次求めていくことでO(1)で計算できる。
 * S="abac"から"ac"は2通りの方法で作れるが、
 * 「ある文字列を作りたいとき、先頭から貪欲に選んでいく」
 * という方針を取ることで重複なく数え上げることができる、という考え方。
 * 
 * 本問ではAiは大きいためmapを用いた。
 * 実装では1-indexedとし、0番目が "" に対応するものとして扱うと処理が楽。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	map<int,int> pre_pos;  // pos[x]:値xが前回出た位置
	vector<mint> dp(N+5), dp_sum(N+5);
	dp[0] = 1;  // 先頭に空文字列があるとみなす
	dp_sum[1] = 1;
	for(i = 1; i <= N; i++)
	{
		int a; cin >> a;
		// dp[pre_pos[a]]～dp[i-1] までの総和  [pre_pos[a], i)
		dp[i] = dp_sum[i] - dp_sum[pre_pos[a]];

		dp_sum[i+1] = dp_sum[i] + dp[i];
		pre_pos[a] = i;
	}
	mint ans = dp_sum[N+1] - dp_sum[1];  // [1,N]の総和 -> [1,N+1)
	cout << ans.val() << endl;

	return 0;
}
