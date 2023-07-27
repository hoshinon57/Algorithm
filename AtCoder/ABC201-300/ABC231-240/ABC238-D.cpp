#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;

// ABC238 https://atcoder.jp/contests/abc238

/*
 * 再帰を使った深さ優先探索で解く。
 * 
 * 2進数で、x,yを最下位桁から1つずつ増やしていき、
 * そこまでのAND/SUM計算に矛盾があるかどうかを判定する。
 * 矛盾があれば探索終了、無ければ探索継続。
 * 
 * x,yに1桁増やすときは、(0,0),(0,1),(1,1)の3パターンで良い。
 * 計算はANDおよびSUMのため、(1,0)のパターンは不要。
 */

ll a, s;
// 2進数表記でketa_2桁まで計算して、矛盾があるかどうかを再帰を使って探索する
bool func(ll x, ll y, int keta_2)
{
	if((x&y)==a && x+y==s) return true;

	// a,s < 2^60より、59桁まで計算すればOK
	// ただし上記のx&y, x+y判定を先にやる必要があることには注意
	if(keta_2 >= 60) return false;

	// (keta_2)桁まで計算し、矛盾があれば終了
	if(keta_2 != 0)   // 最初のketa_2=0時は省略
	{
		ll bitMask = (1LL<<keta_2)-1;  // keta=2なら(1<<2)-1=b'11  2桁のビットマスクになる
		ll tmpX = x&bitMask;
		ll tmpY = y&bitMask;
		if((tmpX & tmpY) != (a&bitMask) ) return false;
		if( ((x+y)&bitMask) != (s&bitMask) ) return false;
		// 現時点で矛盾は無し
	}

	// 計算する桁を1つ増やし、
	// x,yの先頭にそれぞれ(0,0),(0,1),(1,1)を付与して再帰
	keta_2++;
	if( func(x, y, keta_2) ) return true;
	if( func(x, y|(1LL<<(keta_2-1)), keta_2) ) return true;
	if( func(x|(1LL<<(keta_2-1)), y|(1LL<<(keta_2-1)), keta_2) ) return true;

	return false;
}

int main(void)
{
	int i;
	int T;
	cin >> T;
	for(i = 0; i < T; i++)
	{
		cin >> a >> s;
		if(func(0, 0, 0))
		{
			cout << "Yes" << endl;
		}
		else
		{
			cout << "No" << endl;
		}
	}

	return 0;
}
