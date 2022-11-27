#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ABC279 https://atcoder.jp/contests/abc279

/*
 * 3分探索で解く。
 * 参考：https://qiita.com/ganyariya/items/1553ff2bf8d6d7789127
 * 
 * 下に凸の関数であり、l,rの範囲内で3分探索を行って最小値を探していく。
 * 3分探索の実装については特筆する点は無し。
 * 
 * 以下、解の決め方について考える。
 * 2案あり、ソースコードでは(1)を採用している。
 * 
 * (1)
 * 解がある範囲を (l,r) で考える。
 * while(r - l > 2) という条件とl,rの更新式より、
 * ループを抜けた時点で r-l=2, つまり l,l+1,r という並びになっている。
 * よってcalc(l+1)が解となる。
 * 
 * ただしこの場合、l=0が解となるケースでWAにならないよう注意が必要。(l,r内に凸部分が無いケース)
 * lの初期値を-1にしておくことで、3分探索後に l+1=0 となり正しく求められる。
 * ★l,rの初期値は、3分探索のc1,c2に選ばれることは無い。
 * 
 * (2)
 * 解がある範囲を [l,r] で考える。
 * 3分探索のwhileループを抜けた後、l～rまでそれぞれcalc()を呼び出し、その最小値を答とする。
 * 
 * この場合、lの初期値は-1ではなく0にする必要がある。
 * calc(-1)の形式で呼び出さないようにするため。
 * 
 * 
 * doubleの誤差や、lの初期値を-1の範囲外にしても大丈夫か、などを考えると、
 * (2)の「最後に周辺計算して最小値を解とする」の方が、精神的には安心かな。。。
 */

int main(void)
{
	ll A, B;
	cin >> A >> B;

	// 地面に到達するまでの時間を返す
	// c:超能力を行う回数
	auto calc = [&](ll c) -> double
	{
		double ret;
		ret = (double)A / sqrt(c+1) + (double)c * B;
		return ret;
	};

	ll l, r;  // 超能力を行う回数が(l,r)の間に、かかる時間の最小値が存在する
	l = -1;
	r = INF64;
	while(r - l > 2)
	{
		ll c1, c2;  // 3分探索 超能力を行う回数
		c1 = (l*2+r)/3;
		c2 = (l+r*2)/3;

		if(calc(c1) > calc(c2)) l = c1;
		else r = c2;
	}
	// ループを抜けた時点で r-l=2, つまり l,l+1,r という並びになっている

	// calc(l+1)が解となる
	cout << std::fixed << std::setprecision(10);
	cout << calc(l+1) << endl;

	// 先頭コメントの方針(2)を採用する場合、
	// 3分探索前の l の初期値を0に変更した上で、こちらを有効化する
#if 0
	double answer = calc(l);
	for(ll i = l; i <= r; i++)
	{
		answer = min(answer, calc(i));
	}
	cout << std::fixed << std::setprecision(10);
	cout << answer << endl;
#endif

	return 0;
}
