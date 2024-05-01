#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// Educational DP Contest / DP まとめコンテスト https://atcoder.jp/contests/dp

/*
 * 自力で解けず、解説を見た。
 *   https://kyopro-friends.hatenablog.com/entry/2019/01/12/231106
 *   https://blog.hamayanhamayan.com/entry/2019/01/12/154729
 *   https://qiita.com/Series_205/items/05bd22a4e78527cb29dc
 * 
 * dp[i][j]を以下のように定義する。
 *   i番目のブロックまで見て、 (i=1～N)
 *   それまでの重さの総和がjとなるときの、 (j=0～WMAX 後述)
 *   価値の最大値
 * 
 * ただし丈夫さという条件があることから、先頭から順に置くことができない、ように見える。
 * ここで、「DPで置いていく順序は最適なものが存在するケースがある」というテクニックがある。
 * これまで置いたブロックの総重さをWとし、その下にブロックx or yのどちらかを置くことを考える。
 * xを上(=先)に置く場合、
 *   W<=s[x]
 *   W+w[x]<=s[y] -> W<=s[y]-w[x]
 * をともに満たす必要があり、 W<=min(s[x], s[y]-w[x]) となる。
 * yを上(=先)に置く場合、
 *   W<=s[y]
 *   W<=s[x]-w[y]
 * から、 W<=min(s[y], s[x]-w[y]) となる。
 * Wが大きい方が最適であるため、
 *   min(s[x], s[y]-w[x]) > min(s[y], s[x]-w[y])  であれば、 x->y の順に下に置いていく
 * のが最適だと分かる。
 * これをソートの条件とすればよい。
 * ※解説によっては上記式を展開しているが、上記式のままソートしても問題ない。
 *   hamayanhamayan氏の解説がこれ。
 * 
 * あとは先頭のブロックから順に「置くか、置かないか」の遷移でDP計算していく。
 * dp[N][*]の最大値が答。
 * 
 * [補足]
 * 最初、WMAX=10^4としてWAとなった。
 *   先頭からN-1個のブロックを置いたときの、重さの総和：10^4以下 (sの制約より)
 *   N個目のブロックの重さ：10^4以下 (wの制約より)
 * から、WMAXは2*10^4が正解。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・要素を使う順番が任意であるときに、
 *   実は最適な順序が一意に決まるケースがある、という考え方。
 */

int main(void)
{
	int i, j;
	ll N; cin >> N;
	const ll WMAX = 20000;  // 最後のブロックxについて、sx(=それまでの重さ)+wx(自分の重さ) が総和の最大になりうるため
	vector<ll> w(N), s(N), v(N);
	for(i = 0; i < N; i++) cin >> w[i] >> s[i] >> v[i];
	vector<vector<ll>> dp(N+1, vector<ll>(WMAX+1, -INF64));  // dp[N+1][WMAX+1]
	dp[0][0] = 0;

	vector<int> idx(N);
	iota(idx.begin(), idx.end(), 0);
	sort(idx.begin(), idx.end(), [&](int x, int y){ return min(s[x],s[y]-w[x]) > min(s[y], s[x]-w[y]);});

	for(i = 1; i <= N; i++)  // idx[i-1]でアクセス
	{
		ll ww = w[idx[i-1]];
		ll ss = s[idx[i-1]];
		ll vv = v[idx[i-1]];
		for(j = 0; j <= WMAX; j++)
		{
			dp[i][j] = dp[i-1][j];  // ブロックiを選ばない場合

			ll prev_w = j - ww;
			if(prev_w < 0 || prev_w > ss) continue;
			chmax(dp[i][j], dp[i-1][prev_w] + vv);
		}
	}

	ll ans = -INF64;
	for(j = 0; j <= WMAX; j++) chmax(ans, dp[N][j]);
	cout << ans << endl;

	return 0;
}
