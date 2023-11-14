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

// ABC210 https://atcoder.jp/contests/abc210

/*
 * 自力で解けず、解説を見た。
 * クラスカル法の考え方でMSTを求める。
 *   https://drken1215.hatenablog.com/entry/2023/08/07/001608
 *   https://note.com/momomo0214/n/nf95deeaddd75
 *   https://blog.hamayanhamayan.com/entry/2021/07/17/233253
 * 
 * 値がAである操作を行うと、グラフはgcd(N,A)個の連結成分に変化する。
 * N2=gcd(N,A)とし、次の操作をA2とすると、同じくgcd(N2,A2)個の連結成分になる。
 * 
 * よってCostの小さい順に操作をソートしておき、
 * 操作ごとに辺を可能なだけ(=無駄にならない分だけ)追加していった結果が答。
 * このとき使用する辺の数は N-gcd(N,A) 個。
 * 
 * N!=1で終了したら全域木は作れていないため-1を出力する。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・全域木を作る問題と言える。
 *   さらには最小値を求めるので、MSTを作る問題を言える。
 * ・costの小さい方から操作していった方が良さそう。
 * ・NやAをいくつか設定して手作業をすると法則が見つけられそう。ここを頑張る。
 */

long long gcd(long long a, long long b)
{
	if(b == 0) return a;
	else return gcd(b, a%b);
}

int main(void)
{
	ll N, M; cin >> N >> M;
	vector<pair<ll,ll>> ope(M);  // {cost,A}の順
	for(auto& [c,a] : ope)
	{
		cin >> a >> c;
	}
	sort(ope.begin(), ope.end());  // cost順にソート

	ll ans = 0;
	for(auto [c, a] : ope)
	{
		ll nn = gcd(N, a);  // GCDが次のNになる
		ans += (N-nn)*c;  // N-nn:使う辺の数
		N = nn;
	}
	if(N != 1) ans = -1;  // 最終的に1にならなければ、全域木は作れない
	cout << ans << endl;

	return 0;
}
