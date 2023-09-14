#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1000000007;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC168 https://atcoder.jp/contests/abc168

/*
 * 自力で解けず、解説を見た。
 * 
 * いったん、A,Bともに非0であるケースで考えると、不仲になるのは
 *   Ai/Bi = -Bj/Aj
 * となる場合であることが分かる。
 * A,BをGCDで割っておき、連想配列で{A,B}となるイワシをカウントすると、
 * {Ai,Bi}の不仲であるイワシは{Bi,-Ai}の個数に等しくなる。
 * ※ここで{A,B}は、{+,+}, {-,+} のどちらかになるよう調整しておく。
 * 
 * 不仲でないイワシ同士は自由に選べるので、
 * 各{Ai,Bi}および{Bi,-Ai}のグループについて、それぞれ個数をX,Yとすると
 *   2^X + 2^Y -1
 * がこのグループでの選び方の個数となる。
 * 「1匹も選ばない」ケースがX,Y側それぞれでカウントされるため、-1が必要。
 * これらの積が答となる。
 * 
 * 次にA,Bに0が含まれるケースを考える。
 * (1)A,Bのどちらか片方が0のケース
 *   {0,B}は全パターンの{A,0}と不仲、{A,0}は全パターンの{0,B}と不仲になる。
 *   よって前者を{0,1}, 後者を{-1,0}に割り当てておけば、前述の計算に組み込める。
 * (2)A,Bともに0のケース
 *   {0,0}は他のどのイワシとも組めない。これ単体で選ぶ形となる。
 *   よって{0,0}の数を別途カウントしておき、最後に加算する。積にはできない点に注意。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・条件の式を変形して扱いやすくできないかを考える。
 * ・選べないペア/グループがあり、それ以外は自由に選べるのであれば、
 *   「選べないグループ間での選び方の総数」を求めて、他のそれと積を取っていけば答になる。
 */

long long gcd(long long a, long long b)
{
	if(b == 0) return a;
	else return gcd(b, a%b);
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
	int i;
	ll N; cin >> N;
	map<pair<ll,ll>,ll> cnt;  // cnt[{A,B}] 美味しさA, 香りBであるイワシの個数
	ll zero_all = 0;  // a=0, b=0の個数
	for(i = 0; i < N; i++)
	{
		ll a, b; cin >> a >> b;
		if(a == 0 && b == 0)
		{
			// {0,0}は他のどのイワシとも組めない。これ単体でのみ選べる
			zero_all++;
			continue;
		}
		else if(a == 0)  // {0,B}
		{
			// 全パターンの{A,0}いずれとも不仲であり、{0,1}を割り当てる
			cnt[{0, 1}]++;
			continue;
		}
		else if(b == 0)  // {A,0}
		{
			// 全パターンの{0,B}いずれとも不仲であり、{-1,0}を割り当てる
			cnt[{-1, 0}]++;
			continue;
		}
		ll g = gcd(abs(a), abs(b));
		a /= g;
		b /= g;
		if(b < 0) { a=-a; b=-b; }  // A,Bともに正にするか、負があればA側にする
		cnt[{a, b}]++;
	}

	map<pair<ll,ll>, bool> used;  // ダブリ計算をしない用
	ll ans = 1;
	for(auto &e : cnt)
	{
		ll a = e.first.first;
		ll b = e.first.second;
		if(used[{a, b}]) continue;  // {b,-a}側ですでに計算済み

		ll x = e.second;
		ll y = cnt[{b, -a}];
		ll tmp = (modpow(2, x, MOD) +  modpow(2, y, MOD) - 1 + MOD) % MOD;
		ans = ans * tmp % MOD;
		used[{b, -a}] = true;
	}
	
	// {0,0}はこれ単体でのみ選べるため独立して加算
	// 「何も選ばない」ケースが含まれているため-1
	ans = (ans + zero_all - 1 + MOD)%MOD;
	cout << ans << endl;

	return 0;
}
