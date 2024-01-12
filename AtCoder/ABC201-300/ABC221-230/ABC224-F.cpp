#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC224 https://atcoder.jp/contests/abc224

/*
 * DPで解いた。
 * 以下の公式解説が近そうな感覚を受けた。
 *   https://atcoder.jp/contests/abc224/editorial/2830
 * 
 * "1234"の"12"まででまず考えてみると、12, 1+2 の2通り。
 * 数値の右側に+が無いものは、以降で「数値が来て桁が1つ増える」「+が来て値が確定する」の2択になる。
 * 数値の右側に+があるものは、以降でも値は変わらない。
 * 前者の和を"未確定"としてdp_a[]とし、後者の和を"FIX"としてdp_b[]とする。
 * 
 * 上記で"123"までを考えると、
 *   12 -> 123, 12+3
 *   1+2 -> 1+23, 1+2+3
 * の4パターンとなり、ここから
 *   dp_a[i] = dp_a[i-1]*10 + (Sのi桁目)*2^i
 *     前回の"未確定"は桁が1つ増えて10倍、またSのi桁目(=3)は2^i回登場するため。
 *   dp_b[i] = dp_a[i-1] + dp_b[i-1]*2
 *     前回の"未確定"の次に+が来ることでFIXになり、
 *     また前回の"FIX"は、次に 3 と +3 の2パターンがあるので、それらの和。
 * という遷移になる。
 * これをそのまま実装する。
 * 
 * [ACまでの思考の流れ]
 * ・それぞれの数値の間に「+を入れない」「+を入れる」の2パターンがある。
 *   式の作り方は 2^(|S|) 通りになる。
 *   ここで、先頭からDPが思い浮かんだ。
 * ・右側に+が無いものは、次に「+を入れない」場合は値が10倍になり、「+を入れる」場合はそこで値が確定する。
 *   未確定と確定で値を別管理にすると、DPできるかも。
 */

// a^n (mod) を計算する
// 例えば3^5=3^1*3^4であり、5をビットごとに見ていって最下位ビットが1のときに積算する
// ※modは素数といった制約は無い
// 以下URLのコードをほぼそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modpow(long long a, long long n, long long mod)
{
	// a %= mod;  // a*aにてオーバーフローしそうな場合に有効化
	long long ans = 1;
	while(n > 0)
	{
		if((n&1) != 0)  // nの最下位ビットが立っているなら、答に掛ける
			ans = (ans * a) % mod;
		a = (a * a) % mod;
		n >>= 1;
	}
	return ans;
}

int main(void)
{
	ll i;
	string s; cin >> s;
	ll len = s.size();
	vector<ll> num(len);
	for(i = 0; i < len; i++)
	{
		num[i] = (ll)(s[i] - '0');
	}
	vector<ll> dp_a(len), dp_b(len);
	dp_a[0] = num[0];
	dp_b[0] = 0;

	for(i = 1; i < len; i++)
	{
		dp_a[i] = dp_a[i-1] * 10 + modpow(2, i, MOD) * num[i];
		dp_a[i] %= MOD;
		dp_b[i] = dp_a[i-1] + dp_b[i-1]*2;
		dp_b[i] %= MOD;
	}
	ll ans = dp_a[len-1] + dp_b[len-1];
	ans %= MOD;
	cout << ans << endl;

	return 0;
}
