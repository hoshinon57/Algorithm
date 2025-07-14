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

#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;

// ABC414 https://atcoder.jp/contests/abc414

/*
 * コンテスト時に解けず、解説を見た。これで水DIFF下位か…
 * 以下を参考にした。
 *   https://x.com/kyopro_friends/status/1944032970802966969
 *   https://atcoder.jp/contests/abc414/editorial/13450
 *   https://atcoder.jp/contests/abc414/submissions/67566287 snuke氏の実装
 * 類題：ABC230-E (Fraction Floor Sum)
 * 
 * まず色々と整理。
 * ・a,bが決まればcは一意に決まる。
 *   よって条件を満たす(a,b)の組み合わせを考えてみる。
 * ・a,b,cは相異なることから、 3%7=3 といったケースはNG.これはa<bのとき。
 *   つまりa>bとなる。
 * ・1<=cより、6%3=0 といったケースはNG. これはaがbの倍数のとき。
 *   つまりaがbの倍数のときは除外する必要がある。
 * 以上より、bをある値に固定したときに
 *   a:[b+1,N], かつbの倍数ではない
 *   ⇒a:[b,N], かつb,2b,3b,... を除外
 *   ⇒(N-b+1) - (N/b)
 * がaの通り数となる。
 * bは制約より2<=b<=N-1となるが(a>b>cなので)、b=1やNのときも上記式に上手くはまる。よって
 *   Σ{k:1～N}((N-k+1) - (N/k))
 * が答となる。
 * 
 * Σ(N-k+1) = N+(N-1)+(N-2)+...+1 = N*(N+1)/2 と展開できる。
 * あとはΣ{k:1～N}(N/k)の部分。
 * しかしポイントとして、N/kは取りうる値の種類がせいぜい2√N個である。
 *   (1)k=1～√Nまで：√N通り
 *   (2)k=√N～Nまで：
 *      k=√NのときN/k=√N, k=NのときN/k=1 なので、せいぜい√N通り。
 *   以上、2√N通りしか存在しない。
 * よって、N/kが同じ値になるようなkの範囲でまとめて計算すれば、O(√N)で解ける。
 * [k,k2]で同じ値になるとすると、kからk2は以下で求められる。
 *   N/k=y
 *   N/y=K2
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・bに着目して全探索、までは思いついた。
 *   ただしそこから「√N以下と√N以上でそれぞれ処理する」的な思考にはまってしまった。
 *   丁寧に立式をするとか、つまづいたら思考のリセットをするとか、そういうのを意識する。
 * ・Σ{k:1～N}(N/k) は、取りうる値の種類が2√N個程度しか無い。
 *   N/kが同じ値になるようなkでまとめて計算すれば、O(√N)で解ける。
**/

int main(void)
{
	ll b;
	ll N; cin >> N;
	mint ans = (mint)N*(N+1)/2;
	for(b = 1; b <= N; )  // forでbインクリメントはしない
	{
		ll y = N/b;
		ll br = N/y;
		// b:[b,br]が N/b 同じ
		ans -= (br-b+1)*y;
		b = br+1;  // 次のb
	}
	cout << ans.val() << endl;

	return 0;
}
