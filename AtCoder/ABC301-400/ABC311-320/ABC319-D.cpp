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

// ABC319 https://atcoder.jp/contests/abc319

/*
 * コンテスト中にバグらせて解けなかった。大反省。
 * 二分探索で解く。
 * 
 * ウィンドウの横幅を大きくしていくと、どこかで「M行に収まらない」と「M行に収まる」が切り替わる。
 * それを二部探索で求める。
 * 判定関数は、ある数wをウィンドウの横幅として単語を貪欲に入れていき、
 * M行以内に収まるか否かでO(N)で求められる。
 * 
 * 今回バグらせたのは、以下2つのどちらかを入れるべきが、どちらも入れていなかったため。
 * (1)l側(NG側)の初期値を0ではなく、L[i]の(最大値-1)とする。
 * (2)判定関数にて横幅wよりも長い単語があれば、即NGと判断する。
 * 
 * (1)(2)のどちらも入れていなかったことで、例えば
 *   2 2
 *   99 100
 * のように「Lの最後が最大値」のケースにて正しく判定できなかった。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・WAとなった場合、WAになるような入力例を何としても作る。
 * ・二分探索のとき、l,rの初期値は適切か、判定関数は適切かを考える。
 *   ⇒初期値は特にlを見直す。rはINFでまあ大体問題ないけれど、lの初期値で今回ハマった。
 */

int main(void)
{
	int i;
	ll N, M; cin >> N >> M;
	vector<ll> l(N); for(i = 0; i < N; i++) {cin >> l[i];}

	ll ng = 0;
	ll ok = INF64;
	while(ok - ng > 1)
	{
		ll mid = (ng+ok)/2;
		// 横幅がwのとき、M行に収まるならtrue
		auto check = [&](ll w) -> bool
		{
			ll pos = 0;
			ll r_cnt = 1;
			for(i = 0; i < N; i++)
			{
				if(l[i] > w) return false;  // そもそも1単語すら入らない

				if(pos + l[i] > w)  // はみ出すので次行へ
				{
					r_cnt++;
					pos = l[i];
				}
				else  // 今の行に入れられる
				{
					pos += l[i];
				}
				pos++;  // 空白文字	
			}
			return (r_cnt <= M);
		};
		if(check(mid)) ok = mid;
		else ng = mid;
	}
	cout << ok << endl;

	return 0;
}
