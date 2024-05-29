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

// ABC171 https://atcoder.jp/contests/abc171

/*
 * 自力で解けず、各種解説を見た。
 *   https://x.com/kyopro_friends/status/1274705953099669505
 *   https://x.com/kyopro_friends/status/1274710268262547456
 *   https://drken1215.hatenablog.com/entry/2020/06/27/125500
 *   https://blog.hamayanhamayan.com/entry/2020/06/23/193342
 * 
 * 解説の通りで、
 *   文字列Sを部分文字列として持つような、とある文字列Tを全て求めたい。
 *   S="ab"に1文字増やすとき、 a*b と *ab と単にやると重複してしまうので、
 *   「前から貪欲にSの文字列である」とすることで、重複なく1対1対応を作ることができる。
 * 
 * S="abc", K=5のとき、
 *   T=***a***b***c***
 * を考える。
 * 先頭3ブロックの"***"は、前述の貪欲により、*ごとに(26-1)通り選べる。
 * 最後の"***"ブロックは、*ごとに26通り選べる。
 * 最後の"***"に使う文字数をxとすると、以下となる。
 *   c*** は固定で、***a***b*** の組み合わせを考える。
 *   この文字列長は (|S|-1)+(K-x) であるので、abの置き場所は C(|S|-1+K-x, |S|-1) 通り。
 *   ***の置き方は (26-1)^(K-x) 通り。
 *   最後の***の置き方は 26^x 通り。
 *   これらの積がxのときの組み合わせなので、x=0～Kまでの総和が答。
 * 
 * [補足]
 * 自身が最初に考えた案は以下。
 *   まずSから1文字足したS'の組み合わせを考える。
 *   次にS'から1文字足したS''を考える。
 *   以降同様にしてK回処理する。
 * ただしこの場合、aab -> aaba, aaab の次に aaaba が重複カウントされるのでNG.
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・Sを部分列に持つ文字列を考えるときは、前や後ろから貪欲に「Sの文字列であった」とすると
 *   うまく1体1対応が作れることがあるらしい。
 *   https://x.com/kyopro_friends/status/1274706935028150273
 * ・本問の場合、前から貪欲に「Sの文字列であった」とすると、
 *   Sの最後の文字より手前に文字を追加する場合、25通り。Sの最後の文字以降なら、26通り。
 *   これとSの「最後の文字以外」の置き方を考えていくと、解ける。
 */

const ll MOD = 1e9+7;
const ll NCK_SIZE = 2000000+5;  // K,|S|は10^6
ll fact[NCK_SIZE], inv_fact[NCK_SIZE], inv[NCK_SIZE];

// Combination用の事前計算
void nCk_init(void)
{
	fact[0] = fact[1] = 1;
	inv[0] = inv[1] = 1;
	inv_fact[0] = inv_fact[1] = 1;

	for(int i = 2; i < NCK_SIZE; i++)
	{
		// 階乗
		fact[i] = fact[i - 1] * i % MOD;
		// 逆元
		inv[i] = MOD - inv[MOD%i] * (MOD / i) % MOD;
		// 逆元の階乗
		inv_fact[i] = inv_fact[i - 1] * inv[i] % MOD;
	}
}

// CombinationのnCkを求める
ll nCk(ll n, ll k)
{
	if (n < k) return 0;
	if (n < 0 || k < 0) return 0;

	ll x = fact[n];  // n!の計算
	ll y = inv_fact[n-k];  // (n-k)!の計算
	ll z = inv_fact[k];  // k!の計算
	
	return x * ((y * z) % MOD) % MOD;  //nCkの計算
}

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
	nCk_init();
	ll i;
	ll K; cin >> K;
	string s; cin >> s;
	ll len = s.size();
	ll ans = 0;
	for(i = 0; i <= K; i++)
	{
		ll tmp;
		// C(|S|-1+K-i, |S|-1) * 25^(K-i) * 26^i
		tmp = nCk(len-1+K-i, len-1);
		tmp *= modpow(25, K-i, MOD);
		tmp %= MOD;
		tmp *= modpow(26, i, MOD);
		tmp %= MOD;
		ans += tmp;
		ans %= MOD;
	}
	cout << ans << endl;

	return 0;
}
