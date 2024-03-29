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

// ABC165 https://atcoder.jp/contests/abc165

/*
 * [ACまでの思考の流れ]
 * Mが最大のときが最も厳しいはずなので、そのときにどう割り振るかを考える。
 * これが決まれば、それより小さいMもそのまま求まる。
 * 
 * 例えばN=7,M=3を考えると、
 *   a1=1, b1=3
 *   b2=4, b2=6
 * というのは、1,4ラウンド目に同じ相手になってしまう。
 * どうやら bx-ax および ax-bx の値が全ペアで異なる値でることが必要っぽい。 ⇒以降これをdiffと書く
 * 
 * するとN=7,M=3のケースでは、a,bの組み合わせが
 *   1,7  (diff:1,6)
 *   2,6  (diff:3,4)
 *   3,5  (diff:5,2)
 * のときにOKになる。
 * つまり両端から1つずつ取っていくのが良さそう。
 * 
 * ただしこれをNが偶数のときにも適用すると、途中で値がダブってしまいNG.
 * 色々試してみると、
 *   半分ぐらい対戦場を作ったら、右側を1回だけ追加で-1する
 * とすれば上手く調整できた。
 *   
 * [どう思考すれば解法にたどり着けるか]
 * ・紙にいくつか例を書き出してみて、どういうケースで成立するかを考える。
 *   手でガチャガチャやってると見えてくることも多々ある。
 * ・パラメータが偶数/奇数それぞれで同じ手順で求まるのか、
 *   それとも偶奇で手順を変える必要があるのかを考える。
 * ・制約が最も厳しいケースで考えてみる。
 */

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	int l = 1, r = N;
	for(i = 0; i < M; i++)
	{
		// Nが偶数で、場を半分作ったタイミングでrを1ずらしておく
		if(N%2 == 0 && i == (M+1)/2)
		{
			r--;
		}
		cout << l << " " << r << endl;
		l++;
		r--;
	}

	return 0;
}
