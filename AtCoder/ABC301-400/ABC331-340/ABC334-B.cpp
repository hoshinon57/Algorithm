#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC334 https://atcoder.jp/contests/abc334

/*
 * B問題に大苦戦した…
 * 
 * まず基点を0にしたいので、L,RそれぞれにAを減算する。
 * これで ...,-2M,-M,0,M,2M,... にツリーが立つことになる。木の番号を ...,-2,-1,0,1,2,... とする。
 * 
 * (1)Rを含み、Rより左にある最大のツリーの番号：floor(R/M)
 * (2)Lを含み、Lより右にある最小のツリーの番号：ceil(L/M)
 * とすると、 (1)-(2)+1 が答となる。
 * 
 * [反省点]
 * ・基点が0でないのは面倒なので、0に調整する。
 * ・除算の切り上げ切り捨ての処理は、値の正負で異なる。ライブラリ化しておこう。
 * ※コンテスト時はL,Rを正の数に調整する処理も入れた。
 *   floor/ceilのライブラリを用意しておけば不要だが、考え方として覚えておく。
 */

// a/b以下の最大の整数(床関数) floor(5,2)=2, floor(-5,2)=-3
template <typename T> T floor_div(T a, T b){ if(b<0){a=-a; b=-b;} if(a>0){return a/b;} else {return (a-b+1)/b;}}
// a/b以上の最小の整数(天井関数) ceil(5,2)=3, ceil(-5,2)=-2
template <typename T> T ceil_div(T a, T b){ if(b<0) {a=-a; b=-b;} if(a>0){return (a+b-1)/b;} else {return a/b;}}

int main(void)
{
	ll a, m, l, r; cin >> a >> m >> l >> r;
	l -= a;
	r -= a;
	cout << floor_div(r,m) - ceil_div(l,m) + 1 << endl;

	return 0;
}
