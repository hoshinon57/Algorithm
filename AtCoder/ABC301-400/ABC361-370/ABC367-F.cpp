#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <random>  // rng_64
#include <chrono>  // rng_64
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC367 https://atcoder.jp/contests/abc367

/*
 * コンテスト時に解けず、解説を見た。
 * Zobrist Hashの多重集合版で解く。
 *   https://atcoder.jp/contests/abc367/editorial/10692
 *   https://x.com/chokudai/status/875112616523218944/quotes
 * 
 * 基本は公式解説の通り。
 * 通常のZobrist Hashではxorを使うことから、多重集合では使えない。(同じ要素が0,2,4,6,... 回登場したときの区別がつかないため)
 * xorではなく加算とすることで、多重集合に対応する。
 * 
 * A,Bが取りうる値(1～N)に対してそれぞれ乱数を割り当て、集合のハッシュはその総和とする。
 * クエリは連続区間であるため、ハッシュに対して累積和を取っておくことで、区間[l1,r1]におけるハッシュをO(1)で計算できる。
 * A,Bとでハッシュが一致すれば集合が一致していると(高確率で)判断できる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・集合の一致判定はZobrist Hashを考えてみる。 [典型]
 * ・Zobrist Hashにてxorではなく加算を用いることで、多重集合の一致判定が可能。
 */

// [lo,hi]の区間の乱数を返す(閉区間)
// 必要なincludeは以下2つ
//   #include <random>
//   #include <chrono>
// 型はlong longで決め打ちしている。(mt19937_64を使っており、生成される値は64bit)
// 参考：
//   https://betrue12.hateblo.jp/entry/2019/09/07/171628
//   https://zenn.dev/mafafa/articles/f1030c3a014d4e
//   https://ei1333.github.io/luzhiled/snippets/other/random-number-generator.html
//   https://yosupo.hatenablog.com/entry/2024/06/14/064913
//   https://rsk0315.hatenablog.com/entry/2020/09/02/164428
//   https://cpprefjp.github.io/reference/random/mt19937_64.html
//   https://cpprefjp.github.io/reference/random/uniform_int_distribution.html
long long rng_64(long long lo, long long hi) {
	// static std::random_device rd;  // random_deviceを使うのは避けた方が良い環境があるらしい
	// static std::mt19937_64 rng(rd());
	static auto seed = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
	static std::mt19937_64 rng(seed);
	return std::uniform_int_distribution<long long>(lo, hi)(rng);  // [lo,hi]の範囲での乱数を生成
}

int main(void)
{
	int i;
	int N, Q; cin >> N >> Q;
	vector<ll> h(N+5);  // h[i]:A=iに対するハッシュ値
	for(auto &e : h) e = rng_64(0, INF64);
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<int> b(N); for(i = 0; i < N; i++) {cin >> b[i];}
	vector<ll> a_sum(N+1), b_sum(N+1);
	for(i = 0; i < N; i++) a_sum[i+1] = a_sum[i] + h[a[i]];  // 一致判定が目的のため、オーバーフローは気にしなくて良い
	for(i = 0; i < N; i++) b_sum[i+1] = b_sum[i] + h[b[i]];

	while(Q > 0)
	{
		Q--;
		int l1, r1, l2, r2; cin >> l1 >> r1 >> l2 >> r2;
		l1--; r1--; l2--; r2--;
		r1++; r2++;  // [l1,r1)
		YesNo(a_sum[r1]-a_sum[l1] == b_sum[r2]-b_sum[l2]);  // l1～r1とl2～r2の区間長比較はやらなかったが、ACした
	}

	return 0;
}
