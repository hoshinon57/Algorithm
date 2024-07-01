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
const ll MOD = 998244353;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC360 https://atcoder.jp/contests/abc360

/*
 * 自力で解けず、解説を見た。基本的には公式解説の通り。
 *   https://atcoder.jp/contests/abc360/editorial/10310
 * 
 * 黒いボールが1番目にある確率をdpとする。
 * 黒いボールが2,3,4,...,N番目にある確率は同一であり、それぞれ (1-dp)/(N-1) となる。
 * また求める期待値は、2,3,...,Nの中央値が(2+N)/2であることから、
 *   dp + (1-dp)/(N-1)*((2+N)/2) * (N-1)    ※末尾の *(N-1) は同じ値がN-1個あるので積算する
 *   = dp + (1-dp) * ((2+N)/2)   ※N-1が消える
 * となる。
 * ※Nが大きいので、各位置ごとに愚直に計算するのは間に合わない
 * 
 * dpの遷移を考える。
 * ・黒いボールが1番目にあり、次も1番目にくる確率
 *   場合の数は (a,b) = (1,1)および(not 1, not1) であるので、全組み合わせはN*N通りより、
 *   dp * (1+(N-1)^2)/N^2 -> newdpに加算
 * ・黒いボールが2番目以降にあり、次に1番目にくる確率
 *   元々i番目にあったとして、場合の数は (a,b)=(1,i),(i,1) の2通りであるので、
 *   (1-dp)/(N-1) * 2/N^2 * (N-1)   ※末尾の *(N-1) は同じ値がN-1個あるので積算する
 *   = (1-dp)* 2/N^2 -> newdpに加算
 * となる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・コンテスト中、DP遷移が正しく求められずACできなかった。
 *   (a!=bの確率) * (a,bどちらかが1の確率) のように積で式を求めようとしていたが、上手くいかず。
 *   解説を見て「場合の数を求めればよいのか」となった。
 *   確率DPにて、場合の数から次のDP値を求めるという考え方も覚えておく。
 * ・また失敗点としてもう1つ、dp値を黒いボールが「1番目にある確率」と「2番目以降にある確率」の2つ作ってしまった。
 *   1つあればもう片方はすぐ計算できるので、DP遷移を考えるのを複雑にしてしまっただけだった。
 */

// mod mの世界におけるaの逆元を返す
// 以下URLのコードをそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modinv(long long a, long long m)
{
	long long b = m, u = 1, v = 0;
	while (b) {
        long long t = a / b;
        a -= t * b; swap(a, b);
        u -= t * v; swap(u, v);
    }
    u %= m; 
    if (u < 0) u += m;
    return u;
}

int main(void)
{
	ll i;
	ll N, K; cin >> N >> K;
	ll ninv_p2 = modinv(N*N%MOD, MOD);  // 1/N^2
	ll p = (1+(N-1)*(N-1)%MOD) * ninv_p2 % MOD;
	ll q = 2 * ninv_p2 % MOD;
	ll dp = 1;  // 黒いボールが左から1番目にある確率

	for(i = 0; i < K; i++)
	{
		ll ndp = 0;
		ndp += dp * p;
		ndp %= MOD;
		ndp += (1-dp+MOD)%MOD * q;
		ndp %= MOD;
		swap(dp, ndp);
	}

	ll ans = dp;
	ll mid = (2+N) * modinv(2, MOD) % MOD;  // (2+N)/2
	ans += (1-dp+MOD)%MOD * mid % MOD;  // (1-dp)*mid
	ans %= MOD;
	cout << ans << endl;

	return 0;
}
