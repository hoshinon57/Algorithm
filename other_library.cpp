#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>  // numeric_limits
#include <cassert>
using namespace std;
typedef long long ll;

/*
 * その他のライブラリ
 * 
 */

// a/b以下の最大の整数(床関数) floor(5,2)=2, floor(-5,2)=-3
template <typename T> T floor_div(T a, T b){ if(b<0){a=-a; b=-b;} if(a>0){return a/b;} else {return (a-b+1)/b;}}
// a/b以上の最小の整数(天井関数) ceil(5,2)=3, ceil(-5,2)=-2
template <typename T> T ceil_div(T a, T b){ if(b<0) {a=-a; b=-b;} if(a>0){return (a+b-1)/b;} else {return a/b;}}

// 以下、bは0-indexed
bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)
void setbit(ll &x, int b) { x |= ((ll)1<<b); }  // xのbビット目を立てる
void unbit(ll &x, int b) { x &= (~((ll)1<<b)); }  // xのbビット目を落とす

// 要素がN*Nであるaについて、右に90度回転させる
void rotate(vector<vector<int>> &a)
{
	int n = a.size();
	auto a_bk = a;
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			a[i][j] = a_bk[n-1-j][i];
		}
	}
}

// [verify]ABC336-C, ABC234-C
// nをbase進法で表したときの値を返す (n=0が変換後も0に対応する)
// 戻り値は、一番下の桁から順に[0],[1],... と格納される
vector<int> chg_base(ll n, int base)
{
	if(n == 0) return {0};  // 0のときだけ特別に処理
	vector<int> a;
	while(n > 0) {
		a.push_back(n%base);
		n /= base;
	}
	return a;
}

// x,yについての一次不定方程式 ax+by=c が整数解を持つ条件：
//   cがgcd(a,b)で割り切れること
// よって ax+by=c の整数解を求めたい場合、以下の手順で求められる。
//   まず上記条件を確かめる (整数解を持たなければ終了)
//   以下のextgcd()にて ax+by=gcd(a,b) での{x,y}を求める
//   c/gcd(a,b) をx,yそれぞれにかける

// extgcd()にてx,yが1つ決まったら、g=gcd(a,b)として、
//   x'=x+(b/g), y'=y-(a/g)
// となる{x',y'}も式を満たす。
// 1つ求めたxがx<0のとき、(x/(b/g))+1 回だけ(b/g)を足してやれば、正である最小のxが求まる、はず。

// [verify]ABC340-F
// 一次不定方程式 ax+by=gcd(a,b) を満たす {x,y} を格納する
// 戻り値：gcd(a,b)
// 以下URLのコードをほぼそのまま持ってきている
//   https://qiita.com/drken/items/b97ff231e43bce50199a
long long extgcd(long long a, long long b, long long &x, long long &y) {
	if (b == 0) { x = 1; y = 0; return a; }
	long long d = extgcd(b, a%b, y, x);
	y -= a/b * x;
	return d;
}

int main(void)
{
	assert(floor_div( 5,  2) ==  2);
	assert(floor_div(-5,  2) == -3);
	assert(floor_div(-5, -2) ==  2);
	assert(floor_div( 5, -2) == -3);
	assert(floor_div( 5,  1) ==  5);
	assert(floor_div( 5, -1) == -5);
	assert(floor_div( 1000000000000000000, 60000000000) ==  16666666);  // 10^18 / 6^10
	assert(floor_div(-1000000000000000000, 30000000000) == -33333334);  // -10^18 / 3^10
	assert(ceil_div( 5,  2) ==  3);
	assert(ceil_div( 5, -2) == -2);
	assert(ceil_div(-5, -2) ==  3);
	assert(ceil_div(-5,  2) == -2);
	assert(ceil_div( 5,  1) ==  5);
	assert(ceil_div( 5, -1) == -5);
	assert(ceil_div( 1000000000000000000, 60000000000) ==  16666667);  // 10^18 / 6^10
	assert(ceil_div(-1000000000000000000, 30000000000) == -33333333);  // -10^18 / 3^10

	ll x = 0b1000;
	assert(!isbiton(x, 4));
	assert( isbiton(x, 3));
	assert(!isbiton(x, 2));
	x = numeric_limits<ll>::max();
	assert( isbiton(x,  0));
	assert( isbiton(x, 62));
	assert(!isbiton(x, 63));
	x = 0;
	setbit(x, 1); assert(isbiton(x, 1)); assert(!isbiton(x, 0));
	setbit(x, 62); assert(isbiton(x, 62));
	setbit(x, 1);  // 既に立っているビット
	assert(x == ((ll)1<<62 | (ll)1<<1));
	unbit(x, 62);
	unbit(x, 62);  // 既に落ちているビット
	assert(!isbiton(x, 62));
	assert(x == ((ll)1<<1));

	vector<vector<int>> a(4, vector<int>(4)), b(4, vector<int>(4));
	a = {{1,0,0,0},
	     {1,1,0,0},
		 {1,0,2,0},
		 {1,2,2,2}};
	b = {{1,1,1,1},
	     {2,0,1,0},
		 {2,2,0,0},
		 {2,0,0,0}};
	rotate(a);
	assert(a == b);

	return 0;
}
