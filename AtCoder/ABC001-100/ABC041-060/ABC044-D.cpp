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

// ABC044 https://atcoder.jp/contests/abc044

/*
 * 自力で解ききれず、解説を見た。
 * √Nで場合分けする問題。
 *   https://img.atcoder.jp/data/arc/060/editorial.pdf
 *   https://drken1215.hatenablog.com/entry/2024/09/14/115435
 * 
 * まず、答bが小さいケースについて考えてみる。
 * bを決め打ちしたとき、f(b,n)=s判定は高速にできる。
 * bが2～10^6辺りまでなら十分に全探索できる。
 * 
 * b>10^6のケースをどうするか。
 * このとき、制約より(n/b)<b, つまりb進法でnは2桁である。
 * 上位の桁をp,下位の桁をqとするとn=pb+qとなり、p+q=sと合わせて式整理すると
 * b=(n-s)/p+1 となる。
 * また、n=pb+q >= pb > p^2 になるので、p<√n.
 * ここからpを1～√nの範囲で全探索すればよい。
 *   (n-s)がpで割り切れないケース
 *   b<=10^6になったケース
 *   p<b, q<bを満たさないケース
 * などを除外するのに注意。
 * もしくは、実際にf(b,n)を計算しなおすのも良いらしい。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・基本は全探索。
 * ・√nで処理を場合分けする方針を覚える。
 *   ⇒これは思いついたが、b>10^6のときにどう全探索するかが解けなかった。
**/

int main(void)
{
	ll i;
	ll n, s; cin >> n >> s;
	
	if(n == s)
	{
		cout << n+1 << endl;
		return 0;
	}

	// f(b,n)を返す
	auto f = [&](ll b) -> ll
	{
		ll ret = 0;
		ll nn = n;
		while(nn > 0)
		{
			ret += nn%b;
			nn /= b;
		}
		return ret;
	};

	const ll BD = 1e6;  // ここで区切る
	// 2<=b<=BDについて、bで全探索
	for(i = 2; i <= BD; i++)
	{
		if(f(i) == s)
		{
			cout << i << endl;
			return 0;
		}
	}

	// b>BDについて
	// nをb進法で表したとき、上位の桁をp, 下位の桁をqとする
	// n=pb+q, p+q=s
	// 式整理すると b=(n-s)/p+1
	if(n-s < 0)
	{
		cout << -1 << endl;
		return 0;
	}
	ll ans = INF64;
	for(ll p = 1; p*p <= n && p <= s; p++)
	{
		// p=0だと除算になるのでp=1から。p=0のときn=sになり、先頭で特出しで処理済み。
		if((n-s)%p != 0) continue;
		ll b = (n-s)/p+1;

		if(b <= BD) continue;

		// b進法なので、p,qはb未満
		ll q = s-p;
		if(p >= b || q >= b) continue;
		// if(f(b) != s) continue;  // こっちでもOKだった

		chmin(ans, b);
	}
	if(ans == INF64) ans = -1;
	cout << ans << endl;

	return 0;
}
