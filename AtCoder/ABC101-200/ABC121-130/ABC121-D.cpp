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

// ABC121 https://atcoder.jp/contests/abc121

/*
 * 想定解はO(1)らしいが、O(logN)実装で解いた。
 * 以下記事が近い。
 *   https://emtubasa.hateblo.jp/entry/2019/03/09/225127
 *   https://betrue12.hateblo.jp/entry/2019/03/09/224330
 * 
 * 区間[A,B]について何らか計算したい場合、まず[0,X]で計算する処理を実装し、
 * [0,A-1]と[0,B]を使って[A,B]を計算するという典型。
 * またbit計算はbitごとに考える典型。
 * 
 * g(A)を0,1,2,...,Aの閉区間の総xorとする。
 * 例えばA=6とすると、
 *  0 1 2 3 4 5 6
 * bit1について考えたとき、pow(2,1+1)=4ごとにループし、1ループの総xorは0になる。
 * ループの残りは(6+1)%4個となる。このうち前半は0で後半は1.
 * 後半の個数が奇数ならこのbitは1になる。
 * 
 * 0bit目だけ「ループ単位での総xor」が0ではないので、別処理とする。
 */

// 以下、bは0-indexed
bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)
void setbit(ll &x, int b) { x |= ((ll)1<<b); }  // xのbビット目を立てる
void unbit(ll &x, int b) { x &= (~((ll)1<<b)); }  // xのbビット目を落とす

ll g(ll n)
{
	if(n <= 0) return 0;

	ll ret = 0;
	// bit0のみ別処理
	if((n%4==1) || (n%4==2)) setbit(ret, 0);

	for(ll i = 1; i < 50; i++)  // 2の50乗 > 10の12乗
	{
		ll p2 = 1LL<<(i+1);  // ibit目について、2の(i+1)乗でループ
		ll rem = (n+1)%p2;  // ループの残り
		rem -= p2/2;  // ループの後半部分
		if(rem <= 0) {;}
		else if(rem%2 == 1)
		{
			setbit(ret, i);
		}
	}
	return ret;
}

int main(void)
{
	ll A, B; cin >> A >> B;
	cout << (g(B) ^ g(A-1)) << endl;

	return 0;
}
