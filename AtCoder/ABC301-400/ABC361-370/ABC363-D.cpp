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

// ABC363 https://atcoder.jp/contests/abc363

/*
 * コンテスト時に解けず、解説を見た。
 *   https://atcoder.jp/contests/abc363/editorial/10464
 * 
 * まず、回文数=0(N=1)のときだけ先に処理しておくと、後の処理が楽。
 * 方針は、
 * ・まず求める回文数の桁数を求める
 * ・桁数dのzan番目の回文数を求める
 * となる。
 * 
 * d桁の回文数がいくつあるかを計算すると、d=1より順に 9, 9, 90, 90, 900, 900, 9000, ... となる。
 * これをketa[d]とする。
 * 出力例3より、35桁まで計算しておけば十分。
 * これを使うことで、N番目の回文数の桁数dおよび、その桁で何番目か？(=zan)をまず求める。
 * 
 * d桁のzan番目が答とする。
 * 例えばd=5,zan=5なら10401.
 * 10401の前半部分(=104)は
 *   100を基点として5番目 -> 104
 * として求まるので、
 *  10^((d-1)/2)+zan-1
 * にて求められる。詳細は実装も参照。
 * 
 * 最大35桁なので、前半部分は最大18桁。
 * つまり9*10^17とかなので、数値型llで計算しても範囲内に収まる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・コンテスト時、桁数ごとの回文数の計算が誤っていた。これがまず大失敗。
 * ・また「まず桁数を決める」は考えたものの、
 *   その後は「DFSなどで先頭から値を確定させていく？」とか考えてしまっていた、これも大失敗。
 * ・先頭1000個の回文数を愚直に出力するぐらいはすぐにできるので、
 *   まず試しに出力して簡単な法則を決めることが大事か。
 * ・桁数ごとの回文数を求めたとき、0などの扱いが面倒なら最初に除外することで実装を簡単にする。
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
	int i;
	ll N; cin >> N;
	// N=1だけ特別処理
	if(N == 1)
	{
		cout << 0 << endl;
		return 0;
	}
	N--;

	const int KETAM = 35;  // 回文数の桁数の最大
	vector<ll> keta(KETAM+1);  // keta[d]:d桁の回文数の数
	keta[1] = keta[2] = 9;
	for(i = 3; i <= KETAM; i++)
	{
		keta[i] = keta[i-2] * 10;
	}

	// N番目の回文数は何桁か？
	ll d;
	ll zan = N;
	for(i = 1; i <= KETAM; i++)
	{
		if(zan <= keta[i])
		{
			d = i;
			break;
		}
		else zan -= keta[i];
	}

	// d桁のzan番目が答
	// -> 1001001など7桁の場合、(1000+zan-1)が前半部分となる
	//    1000の部分は 10^((d-1)/2) にて計算できる ⇒ modpow()でMODをINFにして計算
	ll n = modpow(10, (d-1)/2, INF64) + zan - 1;
	string s = to_string(n);  // 前半部分
	string s_rev = s;  // 後半部分
	reverse(s_rev.begin(), s_rev.end());
	if(d%2 == 1) s.pop_back();  // 奇数桁の場合、折り返し部分を削除しておく
	cout << s+s_rev << endl;

	return 0;
}
