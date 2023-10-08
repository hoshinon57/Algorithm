#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC323 https://atcoder.jp/contests/abc323

/*
 * コンテスト中に解けず、解説を見た。(実装方針が悪く、TLEした)
 * シミュレーションで解く。
 * 
 * 合成によってサイズは大きくなるので、
 * サイズが小さい方から順にシミュレーションしていく。
 * サイズsのスライムがc匹とすると、
 *   cが奇数なら、サイズsのスライムは1匹残る⇒ansに1加算
 *   サイズs*2のスライムが、c/2匹追加される(連想配列にて要素が無駄に作られないよう、c/2>0のときのみ処理する)
 * となる。
 * 
 * 
 * [コンテスト結果の反省点]
 * コンテスト時も上記の方針は立ったが、
 * 「イテレータ破壊が起きるかも？」と心配してpriority_queueやsetを組み合わせた実装にしていまい、
 * 定数倍が悪くTLEしてしまった。
 * 
 * とはいえその後試したら、mapではイテレータ破壊は起きなさそう。
 * また小さい方から順に操作したいのであれば、
 *   begin()にて先頭の要素を取得し、erase()にてその要素を削除
 * とすれば、小さい方から順にアクセスしつつ、要素数の増加も抑えられる。…というテクニックを他の方の実装から学んだ。
 */

int main(void)
{
	ll i;
	ll N; cin >> N;
	map<ll,ll> slm;  // slm[i]=サイズiのスライムの数
	for(i = 0; i < N; i++)
	{
		ll s, c; cin >> s >> c;
		slm[s] = c;
	}

	ll ans = 0;
	while(slm.size() > 0)
	{
		// サイズが小さい順からシミュレーションしていく
		// (先頭の要素を取って削除することで、要素数の増加を抑えるテクニック)
		auto [s,c] = *slm.begin();
		slm.erase(s);

		ans += c%2;  // 奇数ならサイズsのスライムは1匹残る
		if(c/2 > 0) slm[s*2] += c/2;  // サイズs*2のスライムへ合体
	}
	cout << ans << endl;

	return 0;
}
