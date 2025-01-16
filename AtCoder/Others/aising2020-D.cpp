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

// https://atcoder.jp/contests/aising2020/tasks/aising2020_d

/*
 * コーナーケースにやられてしまった、反省。
 * 各種解説の通り。
 *   公式解説のPDF
 *   https://x.com/kyopro_friends/status/1281949470100353024
 *   https://blog.hamayanhamayan.com/entry/2020/07/12/002237
 * 
 * まず、操作をイメージしてみる。
 * Xは莫大な値になるが、f(X)はpopcountの剰余になることから、f(X)はN未満になる。
 * すると、Xは大きくともf(X)はO(logN)ぐらいになるんじゃない？となる。
 * 
 * しかし、Xiを10進数で毎度求めるのはO(N^2)となり間に合わない。
 * とはいえ、差分に着目すると[典型]、一度Xを10進で求めておけば、各XiはO(1)で求めることができる。
 * 
 * Xのpopcountをpcとすると、
 * Xiの（最初の）popcountは、0->1に置き換わるケースでは(pc+1),
 * 1->0のケースでは(pc-1)となる。
 * 
 * コーナーケースとしてpopcount=1のケースがある。
 * このとき（実装によっては）剰余計算で0除算が発生してしまうので注意。
 * 自分の実装では処理分岐させている。
 * 
 * [ACまでの思考の流れ]
 * ・f(X)の処理をイメージすると、Xが莫大でもそんなに計算回数は多くならないんじゃない？となった。
 * ・むしろ各Xiの計算が間に合わない。
 *   Xとの差異が小さいことから、差分に着目してO(1)でXiを求める。[典型]
 * ・popcountは、popcount(X)の±1だけになりそう。
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

// 2進数のxを10進に直す %mod
ll conv(string &x, ll mod)
{
	ll ret = 0;
	for(auto &e : x)
	{
		ret <<= 1;
		ret %= mod;
		if(e == '1')
		{
			ret++;
			ret %= mod;
		}
	}
	return ret;
}

// n%mを計算し、非負つまり[0,m)の範囲で返す
// 参考：https://atcoder.jp/contests/abc376/submissions/58995270 (ecottea氏のテンプレート)
template<typename T>
T smod(T n, T m) {
	n %= m;
	if(n < 0) n += m;
	return n;
}

// 問題文のf(X)
ll f(ll x)
{
	ll ret = 0;
	while(x != 0)
	{
		x %= __builtin_popcountll(x);
		ret++;
	}
	return ret;
}

int main(void)
{
	ll i;
	ll N; cin >> N;
	string x; cin >> x;

	vector<ll> ans(N);
	ll pc = count(x.begin(), x.end(), '1');

	// 0->1のケース
	ll num = conv(x, pc+1);
	for(i = 0; i < N; i++)  // 上の桁から
	{
		if(x[i] != '0') continue;
		// numに、0->1に置き換えたときの変化量を加算
		ll now = modpow(2, N-i-1, pc+1);  // 2^(N-i-1) %mod
		now = smod(num+now, pc+1);
		ans[i] = f(now) + 1;  // f(Xi)の操作ぶん+1する
	}

	// 1->0のケース
	if(pc != 1)
	{
		num = conv(x, pc-1);
		for(i = 0; i < N; i++)
		{
			if(x[i] != '1') continue;
			// numに、0->1に置き換えたときの変化量を減算
			ll now = modpow(2, N-i-1, pc-1);
			now = smod(num-now, pc-1);
			ans[i] = f(now) + 1;
		}
	}
	else  // popcount=1のときだけ、剰余計算でREになるので別途計算必要
	{
		// このケースで1->0になるbitは、Xi=0になるので答は0
		for(i = 0; i < N; i++)
		{
			if(x[i] != '1') continue;
			ans[i] = 0;
		}
	}

	for(auto &e : ans) cout << e << endl;

	return 0;
}
