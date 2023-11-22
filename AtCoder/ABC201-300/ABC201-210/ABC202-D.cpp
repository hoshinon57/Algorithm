#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC202 https://atcoder.jp/contests/abc202

/*
 * 実装しきれず、解説を見た。
 * 先頭の文字から順に確定させていく。(典型：辞書順は前から貪欲)
 * 
 * まず、base=1としておく。またa,bの残り文字数をそれぞれa,bとする(同じ)。
 * (次の)先頭文字にaを使って良いかを考える。
 * 以降の文字数は a-1,b であり、組み合わせはCombinationの (a-1+b)C(a-1) となる。
 * これをnとすると、[base,base+n) の半開区間にKがあればaを採用、無ければbを採用すればよい。
 * bを採用する場合はbaseにnを加算する。
 * 
 * Combinationの計算方法につまづいた。「MOD 998244353で計算する」という問題では無いため。
 * 公式解説の
 * ＞これは (0,0) からスタートし、
 * ＞x軸の正の方向あるいはy軸の正の方向に1進むことを繰り返して(i,j)に至る方法の総数に等しい
 * を参考に実装した。
 * 本問では最大が 60C30=10^17 であるため、範囲内に収まる。
 *   https://atcoder.jp/contests/abc202/editorial/1860
 * 
 * もしくは、hamayanhamayan氏を参考に
 *   nCr = (n-1)Cr + (n-1)C(r-1)    ※パスカルの法則
 * から再帰等で実装してもよさそう。
 *   https://blog.hamayanhamayan.com/entry/2021/05/22/225424
 *   https://manabitimes.jp/math/588
 */

ll tbl[61][61];
void nCk_Init(void)
{
	int i, j;
	for(i = 0; i <= 60; i++)
	{
		for(j = 0; j <= 60; j++)
		{
			if(i == 0 || j == 0) { tbl[i][j] = 1; continue; }
			tbl[i][j] = tbl[i-1][j] + tbl[i][j-1];
		}
	}
}

ll nCk(int n, int k)
{
	// 添え字チェックはしない
	return tbl[n-k][k];
}

int main(void)
{
	nCk_Init();

	ll a, b, K; cin >> a >> b >> K;
	ll base = 1;
	string ans;
	while(a+b > 0)
	{
		if(a==0) { ans+='b'; b--; continue; }
		if(b==0) { ans+='a'; a--; continue; }

		// 試しに、先頭にaを採用してみる
		ll n = nCk(a-1+b, a-1);

		// Kが半開区間 [base, base+n) の間にあるか
		if(K < base+n)  // aを採用
		{
			a--;
			ans += 'a';
		}
		else  // bを採用
		{
			b--;
			ans += 'b';
			base += n;
		}
	}
	cout << ans << endl;
	return 0;
}
