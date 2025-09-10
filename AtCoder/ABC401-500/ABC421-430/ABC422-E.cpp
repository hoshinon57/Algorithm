#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <random>
#include <chrono>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC422 https://atcoder.jp/contests/abc422

/*
 * 自力で解けず、解説を見た。乱択で解く。
 * 
 * 基本的には公式解説の通り。
 *   https://atcoder.jp/contests/abc422/editorial/13820
 * 過半数を通る直線がある場合、ランダムに2点を取ると、
 * その2点が答となる直線上にある確率は、およそ 1/2*1/2=1/4 となる。
 * 50回試行して、すべてハズレとなる確率は
 *   (1-(1/4))^50
 * であり、たぶん 0.0001% ぐらい。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・乱択アルゴリズム、という手法を覚えておく。
**/

// [使用した問題]ABC367-F
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

// a,bの最大公約数を返す
// gcd(x,0)=x, gcd=(0,x)=x
// 引数が負のときの挙動は不安定(戻り値が負になったりならなかったりする)。負がありえるなら絶対値を取って渡した方が良い。
// C++17から標準ライブラリに搭載されているとのこと
// 参考：https://cpprefjp.github.io/reference/numeric/gcd.html
long long gcd(long long a, long long b)
{
	if(b == 0) return a;
	else return gcd(b, a%b);
}

// 2点(x1,y1),(x2,y2)を通る直線 ax+by+c=0 を計算し、参照渡しの引数a,b,cに設定する
// [制約]
// (1)2点は異なる位置にあること (呼び出し元でチェックすること)
// (2)|x|,|y|<=10^9程度 途中計算にて乗算するため
void calcLineFromPoints(long long x1, long long y1, long long x2, long long y2,
	long long &a, long long &b, long long &c) {
	a = y2 - y1;
	b = x1 - x2;
#if 1  // a>=0に正規化 必要な場合は有効化してgcd()をmy_cheatsheet.cppから持ってくる
	if(a == 0) b = 1;
	else {
		long long g = gcd(abs(a), abs(b));
		if(a < 0) g = -g;
		a /= g;
		b /= g;
	}
#endif
	c = -(a*x1 + b*y1);
}

int main(void)
{
	ll i;
	ll N; cin >> N;
	vector<ll> x(N), y(N); for(i = 0; i < N; i++) {cin >> x[i] >> y[i];}

	for(ll _ = 0; _ < 50; _++)  // 試行回数
	{
		ll p = rng_64(0, N-1);
		ll q = rng_64(0, N-1);
		if(p == q) q = (p+1)%N;  // 異なる2点にする(おおざっぱに)

		// ax+by+c=0
		ll a, b, c;
		calcLineFromPoints(x[p], y[p], x[q], y[q], a, b, c);

		// 作った直線上に過半数の点があるかチェック
		ll cnt = 0;
		for(i = 0; i < N; i++)
		{
			ll tmp = a*x[i] + b*y[i] + c;
			if(tmp == 0) cnt++;
		}
		if(cnt > N/2)
		{
			cout << "Yes" << endl;
			cout << a << " " << b << " " << c << endl;
			return 0;
		}
	}
	cout << "No" << endl;

	return 0;
}
