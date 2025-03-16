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

// ABC397 https://atcoder.jp/contests/abc397

/*
 * コンテスト時に解けず、解説を見た。
 * 
 * 1～10^6の範囲でxを全探索して、3乗根を求めるライブラリでyも～～～とか最初考えたが、
 * xが10^6より大きい値でもy^3を引くことで解が存在するな…となった。
 * (コンテスト時も終了後も正答率が低いのは、この方針の人が多かったのかなーと推測)
 * 
 * 因数分解すると (x-y)(x^2+xy+y^2)=N となる。
 * N=10^18として、例えばx-y=10^7とかのケースを考えると、このときx>=10^7になるので、
 * 左辺は10^7^3=10^21とかで成立しない。
 * (x-y)は10^6以下っぽいな、となるので、
 * k=x-y としてkについて全探索する。
 * 
 * さらにx=y+kとして x^3-y^3 をがっつり整理すると
 *   3k*y^2 + 3k^2*y + k^3 - N = 0
 *   y^2 + k*y + (k^3-N)/3k = 0
 * となり、yについての二次方程式になる。
 * このグラフ、下に凸で最小値となるのはy=-k/2の部分。
 * つまり二次方程式の解の方式で±が出てくるが、+の方を求めればよい、となる。
 * 
 * ※上記で 3k で割らない場合、解の方式でll型をオーバーフローするので、
 *   二分探索するとかint128_tとやらを使うか、とかが必要みたい。
 * 
 * √の中身は (k^2 - 4(k^3-N)/3k) であり、除算の部分が整数でないなら除外。
 * あとは解の方式を元に、sqrt(),roundll()などを用いて解いた。
 * 
 * ※実装だとkを無条件で10^6までやってるので、√の中身が負になるケースがある。
 *   cpprefjpを見るとこのとき「定義域エラー」が出るようだが、例外とかは投げられないみたいだし、
 *   func()内で検算もしているので、問題にはなってない模様。
**/

int main(void)
{
	ll N; cin >> N;

	// ax^2+bx+c=0のうち、±の+側について解を返す
	// 整数解が無ければ-1を返す
	auto func = [](ll a, ll b, ll c) -> ll
	{
		ll x = ( -b + llround(sqrt(b*b-4*a*c)) ) / (2*a);
		if(a*x*x+b*x+c == 0) return x;
		else return -1;
	};

	for(ll k = 1; k <= 1e6; k++)
	{
		ll tmp = 4*(k*k*k-N);
		if(tmp%(3*k) != 0) continue;
		ll y = func(1, k, (k*k*k-N)/(3*k));
		if(y == -1) continue;  // 整数解無し

		ll x = k+y;
		if(x <= 0 || y <= 0) continue;
		cout << x << " " << y << endl;
		return 0;
	}
	cout << -1 << endl;

	return 0;
}
