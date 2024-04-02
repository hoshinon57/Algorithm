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

// ABC347 https://atcoder.jp/contests/abc347

/*
 * X^Y=Cのため、Cのbitが
 *   1のとき：XかYに1を振り分ける
 *   0のとき：X,Yともに1か、ともに0
 * となる。
 * 
 * 以下の流れでX,Yを決めていく。
 * (1)Cでbitが1のものについて、a,b大きい方に振り分けていく。
 *    これが終わった段階で、a!=b, a,b<0 はNGとして終了。
 * (2)a,bの残りを、Cでbitが0のものに振り分けていく。
 *    これが終わった段階で、a,b>0はNGとして終了。
 * (3)ここまで来たら、X,Yに解が入っている。
 * 
 * [ACまでの思考の流れ]
 * ・どのようにX,Yが決まっていくかを考える。
 * ・Cにて1のbit：X,Yの片方に振り分ける。
 *   これでもa,bが残っている場合、Cにて0のbitに対してX,Yともに1にすることで振り分ける。
 * ・各フェーズにて矛盾するような状況になるか？を確認していく。
 */

bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)
void setbit(ll &x, int b) { x |= ((ll)1<<b); }  // xのbビット目を立てる

void NG(void)
{
	cout << -1 << endl;
	exit(0);
}

int main(void)
{
	ll i;
	ll a, b, C; cin >> a >> b >> C;
	ll X = 0, Y = 0;
	// Cでbitが1のものについて、a,b大きい方に振り分けていく。
	for(i = 0; i < 60; i++)
	{
		if(isbiton(C, i))
		{
			if(a > b)
			{
				a--;
				setbit(X, i);
			}
			else
			{
				b--;
				setbit(Y, i);
			}
		}
	}
	if(a != b || a < 0 || b < 0) NG();
	// a,bの残りを、Cでbitが0のものに振り分けていく。
	for(i = 0; i < 60; i++)
	{
		if(a == 0 && b == 0) break;
		if(!isbiton(C, i))
		{
			a--; b--;
			setbit(X, i);
			setbit(Y, i);
		}
	}
	if(a > 0) NG();
	cout << X << " " << Y << endl;
	// cout << "dbg:" << (X^Y) << "," << __builtin_popcountll(X) << "," << __builtin_popcountll(Y) << endl;

	return 0;
}
