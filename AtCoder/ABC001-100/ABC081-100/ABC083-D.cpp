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

// ABC083 https://atcoder.jp/contests/abc083

/*
 * 自力で解けず、解説を見た。数学パズルみたいな感じ。
 *   公式解説
 *   https://emtubasa.hateblo.jp/entry/2019/02/14/000000_2
 * 
 * 以下、len = |S|とする。
 * 
 * 長さK"以上"の区間について操作、というのがポイントでもあり厄介な点でもあり。
 * "以上"なので、操作で"11111...11"といったALL 1ができてもOK. 全体に対して操作すればALL 0になるので。
 * 
 * ということは、"01"や"10"のような両隣で異なる値の部分をどうにかしたい、となりそう。
 * i文字目と(i+1)文字目が異なっていて、これを一致させるためには
 * [0,i]か[i+1,len-1]のどちらかで操作すればよい。
 * 最大のKを求めたいので、長い方を採用することになる。
 * ⇒[0,i+1)と[i+1,len)なので、i+1とlen-(i+1)の大きい方。
 * 
 * これを全ての"01" or "10"ポイントで計算し、その最小値が答。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・長さK"以上"で操作、何回でもやって良い、という条件より、
 *   "01"や"10"を1つ1つどうにかしたい、となりそう。
**/

int main(void)
{
	int i;
	string s; cin >> s;
	int len = (int)s.size();
	int ans = len;
	for(i = 0; i < len-1; i++)  // i,i+1文字目を比較
	{
		if(s[i] == s[i+1]) continue;
		int tmp = max(i+1-0, len-(i+1));
		chmin(ans, tmp);
	}
	cout << ans << endl;

	return 0;
}
