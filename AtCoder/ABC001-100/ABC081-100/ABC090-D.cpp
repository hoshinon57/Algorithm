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

// ABC090 https://atcoder.jp/contests/abc090

/*
 * 数え上げ問題（に属するのかな？）
 * 
 * aかbで全探索するのだろう、となる。
 * bの方が周期性が見やすいので、bで全探索する。
 * 
 * いったん、a=0も許容して考える。
 * 例えばb=5,K=3のとき、a=0,1,2,...で余りがK以上か否かを見ると
 *   xxxooxxxooxx...
 * となっている。
 * (1)周期5の部分と(2)その残りについて考えると、
 * (1)周期ごとにOKが(K-b)個あり、周期数はN/bである。
 *   よってこれらの積が答。
 * (2)残りの個数は(N%b)+1個。
 *   max((N%b)+1-K, 0) が答となる。
 * これをbについて全探索すれば良い。
 * 
 * この実装だと、K=0のときがコーナーケース。
 * 各bについてa=0の場合も答にカウントされるが、a,bは正整数という条件なので不適。よって-1しておく。
 * ※もしくは、K=0のときはN^2を出力して終わらせても良い。各所の解説はこれが多かった。
**/

int main(void)
{
	ll N, K; cin >> N >> K;
	ll ans = 0;
	for(ll b = K+1; b <= N; b++)
	{
		ans += (N/b)*(b-K);
		ans += max((N%b)+1-K, 0LL);
		if(K == 0) ans--;  // K=0のとき、a=0も余分にカウントしてしまってる(a,bは正整数)ので、引く
	}
	cout << ans << endl;

	return 0;
}
