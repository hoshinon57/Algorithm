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

// ABC174 https://atcoder.jp/contests/abc174

/*
 * 最終形を考えると、
 *   RR...RWW...W
 * にするのが正解っぽい。
 * 
 * よってまず赤い石の個数を数えて(=red)、
 * 先頭からred個の間にある白い石の個数が答となる。
 * 例：RRWRWWRR であれば、red=5であり、先頭5個に白は2つあるため答は2.
 * 
 * ※結局、石を1つ選んで色を変える操作は使わなかった。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * 「最終状態はどのような形になるか」「その状態へはどう操作したらよいか」を何パターン化手で書いてみる。
 * 操作が2種類以上ある場合、操作をどう組み合わせるか、そもそも2種以上使う必要があるかを考える。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	string s; cin >> s;
	int red = 0;
	for(auto &e : s) if(e == 'R') red++;

	int ans = 0;
	for(i = 0; i < red; i++)
	{
		if(s[i] == 'W') ans++;
	}
	cout << ans << endl;

	return 0;
}
