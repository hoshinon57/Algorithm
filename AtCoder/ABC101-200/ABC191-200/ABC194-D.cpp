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

// ABC194 https://atcoder.jp/contests/abc194

/*
 * 数学問題。hamayanhamayan氏の以下フレーズが全て。
 * ＞有効なのが来るまでカードを引く期待値は、有効なカードを引く確率の逆数になる。
 *   https://blog.hamayanhamayan.com/entry/2021/03/07/000733
 * 
 * 本問における「有効なのが来るまで」は、
 *   N頂点のうち、まだ選ばれていないi個の頂点が新たに選ばれるまで
 * となる。
 * この確率はi/Nなので、回数の期待値はN/iとなる。
 * これをi=N-1～1まで計算して加算したものが答。
 * 
 * [ACまでの思考の流れ]
 * ・「選ばれた頂点に移動する」というのはあまり気にしなくてよくて、
 *   全ての頂点が1回以上選ばれたら終了になりそう。
 * ・初期状態にて「有効なのが来る」確率は (N-1)/N であり、回数の期待値はこの逆数。
 *   次の状態は (N-2)/N.
 *   回数の期待値を求めるので、これを加算すれば良さそう。
 */

int main(void)
{
	int i;
	int n; cin >> n;
	double ans = 0.0;
	for(i = n-1; i > 0; i--)
	{
		ans += (double)n / i;
	}
	cout << std::fixed << std::setprecision(10);
	cout << ans << endl;

	return 0;
}
